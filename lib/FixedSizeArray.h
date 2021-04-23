#pragma once

#include <math.h>

#include <utility>
#include <vector>

#include "Array.h"
#include "glog/logging.h"

namespace compact {
namespace lib {

template <uint length>
class FixedSizeArray : public Array {
 public:
  FixedSizeArray() : sz(0), arraySize(0), array(NULL) {}

  ~FixedSizeArray() {
    if (array) {
      delete[] array;
    }
  }

  FixedSizeArray(uint n) {
    array = NULL;
    setup_array(n);
  }

  FixedSizeArray(const std::vector<uint>& values) {
    array = NULL;
    setup_array(values.size());
    for (uint i = 0; i < values.size(); i++) {
      write(i, values[i]);
    }
  }

  uint size() const override { return sz; }

  void resize(uint n) { setup_array(n); }

  void assign(uint n, uint val) {
    resize(n);
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
    return read_interval(bitInterval.first, bitInterval.second);
  }

  void write(uint idx, uint val) override {
    if (!is_index_valid(idx)) {
      throw std::runtime_error(
          "Write failed! Index " + std::to_string(idx) +
          " is out of bounds! Array size is: " + std::to_string(sz));
    }
    auto bitInterval = get_interval(idx);
    write_interval(bitInterval.first, bitInterval.second, val);
  }

  uint operator[](uint idx) const override { return read(idx); }

  bool operator==(const FixedSizeArray<length>& other) const {
    if (other.size() != size()) return false;
    for (uint i = 0; i < sz; i++) {
      if (read(i) != other.read(i)) {
        return false;
      }
    }
    return true;
  }

 private:
  static const uint kWordSize = 8 * sizeof(uint);
  uint sz;
  uint arraySize;
  uint* array;

  void setup_array(uint n) {
    sz = n;
    arraySize = std::ceil(1.0 * (n * length) / kWordSize);
    if (array) {
      delete[] array;
    }
    array = new uint[arraySize];
  }

  bool is_index_valid(uint idx) const { return idx >= 0 && idx < sz; }

  std::pair<uint, uint> get_interval(uint idx) const {
    return {idx * length, int((idx + 1) * length) - 1};
  }

  uint read_interval(uint startBit, uint endBit) const {
    uint startPos = startBit / kWordSize;     // position in array
    uint startOffset = startBit % kWordSize;  // position in array cell

    uint endPos = endBit / kWordSize;     // position in array
    uint endOffset = endBit % kWordSize;  // position in array cell

    if (startPos == endPos) {
      return read_incell_interval(startPos, startOffset, endOffset);
    }
    uint leftPart = read_incell_interval(startPos, startOffset, kWordSize - 1);
    uint rightPart = read_incell_interval(endPos, 0, endOffset);

    return (leftPart << (endOffset + 1)) | rightPart;
  }

  uint read_incell_interval(uint idx, uint l, uint r) const {
    uint val = array[idx];
    return (val >> l) & ((1 << (r - l + 1)) - 1);
  }

  void write_interval(uint startBit, uint endBit, uint val) const {
    uint startPos = startBit / kWordSize;     // position in array
    uint startOffset = startBit % kWordSize;  // position in array cell

    uint endPos = endBit / kWordSize;     // position in array
    uint endOffset = endBit % kWordSize;  // position in array cell

    if (startPos == endPos) {
      write_incell_interval(startPos, startOffset, endOffset, val);
      return;
    }

    uint leftPart = val >> (endOffset + 1);
    uint rightPart = val & ((1 << (endOffset + 1)) - 1);
    write_incell_interval(startPos, startOffset, kWordSize - 1, leftPart);
    write_incell_interval(endPos, 0, endOffset, rightPart);
  }

  void write_incell_interval(uint idx, uint l, uint r, uint val) const {
    uint croppedVal = get_mask_interval(val, 0, length - 1);
    uint v = clear_mask_interval(array[idx], l, r);
    array[idx] = v | (croppedVal << l);
  }

  static uint clear_mask_interval(uint val, uint l, uint r) {
    return val ^ get_mask_interval(val, l, r);
  }

  static uint get_mask_interval(uint val, uint l, uint r) {
    return ((val >> l) & ((1 << (r - l + 1)) - 1)) << l;
  }
};

}  // namespace lib
}  // namespace compact
