#pragma once

#include <math.h>

#include <algorithm>
#include <utility>
#include <vector>

#include "glog/logging.h"
#include "lib/Array.h"
#include "lib/FixedSizeArray.h"
#include "lib/utils/DensePointersUtility.h"

namespace compact {
namespace lib {

class VariableSizeDenseArray : public Array {
 public:
  using BitArray = DensePointersUtility::BitArray;

  VariableSizeDenseArray() : sz(0), bitStream() {}

  ~VariableSizeDenseArray() {}

  VariableSizeDenseArray(const std::vector<uint>& values) { setup(values); }

  uint size() const override { return sz; }

  uint read(uint idx) const override {
    if (!is_index_valid(idx)) {
      throw std::runtime_error("Invalid index!");
    }

    uint offset = get_offset(idx);
    uint offsetNext = get_offset(idx + 1);
    uint decodedValue =
        DensePointersUtility::decode(bitStream, offset, offsetNext);
    return decodedValue - valueOffset;
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
  int valueOffset;
  BitArray bitStream;
  FixedSizeArray<BitmaskUtility::kMaxLength> offsets;
  // 12 = log2(w*w*2), where w = wordSize = 32
  FixedSizeArray<12> inBlockOffsets;

  void setup(std::vector<uint> values) {
    sz = values.size();
    int minVal = (int)*std::min_element(values.begin(), values.end());
    valueOffset = 1 - minVal;
    shift_values(values);
    bitStream.reset(DensePointersUtility::get_array_code(values));
    offsets.assign(std::ceil(1.0 * sz / blockSize), 0);
    inBlockOffsets.assign(sz, 0);

    uint offset = 0;
    uint inBlockOffset = 0;
    for (uint i = 0; i < sz; i++) {
      if (get_position_in_block(i) == 0) {
        offsets.write(get_block(i), offset);
        inBlockOffset = 0;
      }
      offset += DensePointersUtility::get_code_length(values[i]);
      inBlockOffsets.write(i, inBlockOffset);
      inBlockOffset += DensePointersUtility::get_code_length(values[i]);
    }
  }

  void shift_values(std::vector<uint>& values) {
    for (uint i = 0; i < sz; i++) {
      values[i] += valueOffset;
    }
  }

  uint get_offset(uint idx) const {
    if (idx == sz) return bitStream.size();
    return offsets[get_block(idx)] + inBlockOffsets[idx];
  }

  static uint get_block(uint idx) { return idx / blockSize; }

  static uint get_position_in_block(uint idx) { return idx % blockSize; }

  bool is_index_valid(uint idx) const { return idx >= 0 && idx < sz; }
};

}  // namespace lib
}  // namespace compact