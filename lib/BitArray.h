#pragma once

#include <math.h>

#include <utility>
#include <vector>

#include "glog/logging.h"
#include "lib/Array.h"
#include "lib/FixedSizeArray.h"

namespace compact {
namespace lib {

class BitArray : public Array {
 public:
  BitArray() : array() {}

  BitArray(uint n) { resize(n); }

  BitArray(const std::vector<uint>& values) { reset(values); }

  BitArray(const std::initializer_list<uint>& values)
      : BitArray(std::vector<uint>(values)) {}

  uint size() const override { return array.size(); }

  void resize(uint n) { array.resize(n, 1); }

  template <typename ArrayType>
  void reset(const ArrayType& values) {
    array.reset(values, 1);
  }

  void assign(uint n, uint val) { array.assign(n, val, 1); }

  uint read(uint idx) const override { return array.read(idx); }

  void write(uint idx, uint val) override { array.write(idx, val); }

  uint read_interval(uint startBit, uint endBit) const {
    uint start_word = startBit / BitmaskUtility::kWordSize;
    uint end_word = endBit / BitmaskUtility::kWordSize;
    if (start_word == end_word) {
      return array.read_bit_interval(startBit, endBit);
    }
    uint end_start_word = end_word * BitmaskUtility::kWordSize - 1;

    uint left_part = array.read_bit_interval(startBit, end_start_word);
    uint right_part = array.read_bit_interval(end_start_word + 1, endBit);
    return left_part | (right_part << (end_start_word - startBit + 1));
  }

  // TODO: add write_interval

  std::string to_string() const { return array.to_string(); }

 private:
  FixedSizeArray array;
};

}  // namespace lib
}  // namespace compact
