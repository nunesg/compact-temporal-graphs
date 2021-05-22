#pragma once

#include <math.h>

#include <utility>
#include <vector>

#include "glog/logging.h"
#include "lib/Array.h"
#include "lib/utils/BitmaskUtility.h"

namespace compact {
namespace lib {

class FixedSizeArray : public Array {
 public:
  FixedSizeArray() : sz(0), bit_size(1), array_size(0), array(NULL) {}

  ~FixedSizeArray() {
    if (array) {
      delete[] array;
    }
  }

  FixedSizeArray(uint n, uint bit_size = BitmaskUtility::kWordSize) {
    array = NULL;
    this->bit_size = bit_size;
    setup_array(n);
  }

  FixedSizeArray(const std::vector<uint>& values,
                 uint bit_size = BitmaskUtility::kWordSize) {
    array = NULL;
    this->bit_size = bit_size;
    setup_array(values.size());
    for (uint i = 0; i < values.size(); i++) {
      write(i, values[i]);
    }
  }

  uint size() const override { return sz; }

  void resize(uint n, uint bit_size = BitmaskUtility::kWordSize) {
    this->bit_size = bit_size;
    setup_array(n);
  }

  template <typename ArrayType>
  void reset(const ArrayType& values,
             uint bit_size = BitmaskUtility::kWordSize) {
    this->bit_size = bit_size;
    setup_array(values.size());
    for (uint i = 0; i < values.size(); i++) {
      write(i, values[i]);
    }
  }

  void assign(uint n, uint val, uint bit_size = BitmaskUtility::kWordSize) {
    resize(n, bit_size);
    for (uint i = 0; i < n; i++) {
      write(i, val);
    }
  }

  uint read(uint idx) const override {
    if (!is_index_valid(idx)) {
      throw std::runtime_error(
          "Read failed! Index " + std::to_string(idx) +
          " is out of bounds! Array size is: " + std::to_string(sz));
    }
    auto bitInterval = get_interval(idx);
    return read_bit_interval(bitInterval.first, bitInterval.second);
  }

  void write(uint idx, uint val) override {
    if (!is_index_valid(idx)) {
      throw std::runtime_error(
          "Write failed! Index " + std::to_string(idx) +
          " is out of bounds! Array size is: " + std::to_string(sz));
    }
    auto bitInterval = get_interval(idx);
    write_bit_interval(bitInterval.first, bitInterval.second, val);
  }

  uint read_bit_interval(uint startBit, uint endBit) const {
    check_bit_interval(startBit, endBit);
    uint startPos = startBit / BitmaskUtility::kWordSize;  // position in array
    uint startOffset =
        startBit % BitmaskUtility::kWordSize;  // position in array cell

    uint endPos = endBit / BitmaskUtility::kWordSize;  // position in array
    uint endOffset =
        endBit % BitmaskUtility::kWordSize;  // position in array cell

    if (startPos == endPos) {
      return read_incell_interval(startPos, startOffset, endOffset);
    }
    uint leftPart = read_incell_interval(startPos, startOffset,
                                         BitmaskUtility::kWordSize - 1);
    uint rightPart = read_incell_interval(endPos, 0, endOffset);
    return (leftPart << (endOffset + 1)) | rightPart;
  }

  uint get_bit_size() const { return this->bit_size; }

  std::string to_string() const {
    if (size() == 0) {
      return "[]";
    }
    std::string str("[");
    str += std::to_string(read(0));
    for (uint i = 1; i < size(); i++) {
      str += std::string(",") + std::to_string(read(i));
    }
    return str + "]";
  }

 private:
  uint sz;
  uint bit_size;
  uint array_size;
  uint* array;

  void setup_array(uint n) {
    sz = n;
    array_size = std::ceil(1.0 * (n * bit_size) / BitmaskUtility::kWordSize);
    if (array) {
      delete[] array;
    }
    array = new uint[array_size];
  }

  bool is_index_valid(uint idx) const { return idx >= 0 && idx < sz; }

  std::pair<uint, uint> get_interval(uint idx) const {
    return {idx * bit_size, int((idx + 1) * bit_size) - 1};
  }

  uint read_incell_interval(uint idx, uint l, uint r) const {
    return BitmaskUtility::get_mask_interval(array[idx], l, r) >> l;
  }

  void write_bit_interval(uint startBit, uint endBit, uint val) {
    check_bit_interval(startBit, endBit);
    uint startPos = startBit / BitmaskUtility::kWordSize;  // position in array
    uint startOffset =
        startBit % BitmaskUtility::kWordSize;  // position in array cell

    uint endPos = endBit / BitmaskUtility::kWordSize;  // position in array
    uint endOffset =
        endBit % BitmaskUtility::kWordSize;  // position in array cell

    if (startPos == endPos) {
      write_incell_interval(startPos, startOffset, endOffset, val);
      return;
    }

    uint leftPart = val >> (endOffset + 1);
    uint rightPart = val & BitmaskUtility::get_full_ones(endOffset + 1);
    write_incell_interval(startPos, startOffset, BitmaskUtility::kWordSize - 1,
                          leftPart);
    write_incell_interval(endPos, 0, endOffset, rightPart);
  }

  void write_incell_interval(uint idx, uint l, uint r, uint val) {
    uint croppedVal = BitmaskUtility::get_mask_prefix(val, bit_size - 1);
    uint v = BitmaskUtility::clear_mask_interval(array[idx], l, r);
    array[idx] = v | (croppedVal << l);
  }

  static void check_bit_interval(uint startBit, uint endBit) {
    if (startBit > endBit ||
        (endBit - startBit + 1) > BitmaskUtility::kWordSize) {
      throw std::runtime_error(
          "Invalid bit interval! Interval should fit in a word len");
    }
  }
};

}  // namespace lib
}  // namespace compact
