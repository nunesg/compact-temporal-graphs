#include <math.h>

#include <utility>

#include "Array.h"
#include "glog/logging.h"

namespace compact {
namespace lib {

template <uint length>
class FixedSizeArray : public Array {
 public:
  FixedSizeArray() : sz(0), arraySize(0), array(NULL) {}

  FixedSizeArray(const uint& n) {
    sz = n;
    arraySize = std::ceil(1.0 * (n * length) / kWordSize);
    array = new uint[arraySize];
  }

  uint size() const override { return sz; }

  void resize(const uint& n) { *this = FixedSizeArray(n); }

  void assign(const uint& n, const uint& val) {
    resize(n);
    for (uint i = 0; i < n; i++) {
      write(i, val);
    }
  }

  uint read(const uint& idx) const override {
    if (!is_index_valid(idx)) {
      throw std::runtime_error(
          "Read failed! Index " + std::to_string(idx) +
          " is out of bounds! Array size is: " + std::to_string(sz));
    }
    auto bitInterval = get_interval(idx);
    return read_interval(bitInterval.first, bitInterval.second);
  }

  void write(const uint& idx, const uint& val) override {
    if (!is_index_valid(idx)) {
      throw std::runtime_error(
          "Write failed! Index " + std::to_string(idx) +
          " is out of bounds! Array size is: " + std::to_string(sz));
    }
    auto bitInterval = get_interval(idx);
    write_interval(bitInterval.first, bitInterval.second, val);
  }

  uint operator[](const uint& idx) const override { return read(idx); }

 private:
  static const uint kWordSize = 8 * sizeof(uint);
  uint sz;
  uint arraySize;
  uint* array;

  bool is_index_valid(const uint& idx) const { return idx >= 0 && idx < sz; }

  std::pair<uint, uint> get_interval(const uint& idx) const {
    return {idx * length, int((idx + 1) * length) - 1};
  }

  uint read_interval(const uint& startBit, const uint& endBit) const {
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

  uint read_incell_interval(const uint& idx, const uint& l,
                            const uint& r) const {
    uint val = array[idx];
    return (val >> l) & ((1 << (r - l + 1)) - 1);
  }

  void write_interval(const uint& startBit, const uint& endBit,
                      const uint& val) const {
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

  void write_incell_interval(const uint& idx, const uint& l, const uint& r,
                             const uint& val) const {
    uint croppedVal = get_mask_interval(val, 0, length - 1);
    uint v = clear_mask_interval(array[idx], l, r);
    array[idx] = v | (croppedVal << l);
  }

  static uint clear_mask_interval(const uint& val, const uint& l,
                                  const uint& r) {
    return val ^ get_mask_interval(val, l, r);
  }

  static uint get_mask_interval(const uint& val, const uint& l, const uint& r) {
    return ((val >> l) & ((1 << (r - l + 1)) - 1)) << l;
  }
};

}  // namespace lib
}  // namespace compact
