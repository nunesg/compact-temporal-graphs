#pragma once

#include <math.h>

#include <utility>
#include <vector>

#include "Array.h"
#include "FixedSizeArray.h"
#include "GamaUtility.h"
#include "glog/logging.h"

namespace compact {
namespace lib {

class VariableSizeArray : public Array {
 public:
  using BitArray = GamaUtility::BitArray;

  VariableSizeArray() : sz(0), bitStream(), offsets() {}

  ~VariableSizeArray() {}

  VariableSizeArray(const std::vector<uint>& values) { setup(values); }

  uint size() const override { return sz; }

  uint read(uint idx) const override {
    if (!is_index_valid(idx)) {
      throw std::runtime_error("Invalid index!");
    }
    uint b = get_block(idx);
    uint targetPos = get_position_in_block(idx);
    uint offset = offsets[b];
    // LOG(INFO) << "block of position " << idx << " is: " << b
    //           << ", offset: " << offset;
    for (uint i = 0; i < targetPos; i++) {
      // LOG(INFO) << "pass through offset: " << offset;
      GamaUtility::decode_next(bitStream, offset);
    }
    LOG(INFO) << "get result at offset: " << offset;
    return GamaUtility::decode_next(bitStream, offset);
  }

  // TODO: make this class extend a "ReadableArray" class or s/th like this
  void write(uint idx, uint val) override {
    throw std::runtime_error(
        "Variable-size array does not support write operations!");
  }

  void reset(const std::vector<uint>& values) { setup(values); }

 private:
  // TODO: make this equal to BitmaskUtility::kWordSize * ln(2)
  static const uint blockSize = 2;  // k
  uint sz;
  BitArray bitStream;
  FixedSizeArray<BitmaskUtility::kMaxLength> offsets;

  void setup(const std::vector<uint>& values) {
    sz = values.size();
    bitStream.reset(GamaUtility::get_array_code(values));
    offsets.assign(std::ceil(1.0 * sz / blockSize), 0);

    uint offset = 0;
    for (uint i = 0; i < sz; i++) {
      if ((i % blockSize) == 0) {
        offsets.write(i / blockSize, offset);
      }
      GamaUtility::decode_next(bitStream, offset);
    }
  }

  uint get_block(uint idx) const { return idx / blockSize; }

  uint get_position_in_block(uint idx) const { return idx % blockSize; }

  bool is_index_valid(uint idx) const { return idx >= 0 && idx < sz; }
};

}  // namespace lib
}  // namespace compact
