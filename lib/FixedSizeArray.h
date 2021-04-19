#include <math.h>

#include "Array.h"

class FixedSizeArray : public Array {
 public:
  FixedSizeArray(const unsigned int& n, const unsigned int& length) {
    sz = n;
    this->length = length;
    arraySize = std::ceil(1.0 * (n * length) / kWordSize);
    array = new unsigned int[arraySize];
  }

  unsigned int size() const override { return sz; }

  unsigned int read(const unsigned int& idx) const override {
    auto bitInterval = get_interval(idx);
    return read_interval(bitInterval.first, bitInterval.second);
  }

  // TODO
  void write(const unsigned int& idx, const unsigned int& val) override {
    auto bitInterval = get_interval(idx);
    write_interval(bitInterval.first, bitInterval.second, val);
  }

  unsigned int operator[](const unsigned int& idx) const override {
    return read(idx);
  }

 private:
  static const unsigned int kWordSize = 8 * sizeof(int);
  unsigned int sz;
  unsigned int length;
  unsigned int arraySize;
  unsigned int* array;

  std::pair<unsigned int, unsigned int> get_interval(
      const unsigned int& idx) const {
    return {idx * length, int((idx + 1) * length) - 1};
  }

  unsigned int read_interval(const unsigned int& startBit,
                             const unsigned int& endBit) const {
    unsigned int startPos = startBit / kWordSize;     // position in array
    unsigned int startOffset = startBit % kWordSize;  // position in array cell

    unsigned int endPos = endBit / kWordSize;     // position in array
    unsigned int endOffset = endBit % kWordSize;  // position in array cell

    if (startPos == endPos) {
      return read_incell_interval(startPos, startOffset, endOffset);
    }
    unsigned int leftPart =
        read_incell_interval(startPos, startOffset, kWordSize - 1);
    unsigned int rightPart = read_incell_interval(endPos, 0, endOffset);

    return (leftPart << (endOffset + 1)) | rightPart;
  }

  unsigned int read_incell_interval(const unsigned int& idx,
                                    const unsigned int& l,
                                    const unsigned int& r) const {
    unsigned int val = array[idx];
    return (val >> (kWordSize - r - 1)) & ((1 << (r - l + 1)) - 1);
  }

  void write_interval(const unsigned int& startBit, const unsigned int& endBit,
                      const unsigned int& val) const {
    unsigned int startPos = startBit / kWordSize;     // position in array
    unsigned int startOffset = startBit % kWordSize;  // position in array cell

    unsigned int endPos = endBit / kWordSize;     // position in array
    unsigned int endOffset = endBit % kWordSize;  // position in array cell

    if (startPos == endPos) {
      write_incell_interval(startPos, startOffset, endOffset, val);
      return;
    }

    unsigned int leftPart = val >> (endOffset + 1);
    unsigned int rightPart = val & ((1 << (endOffset + 1)) - 1);
    write_incell_interval(startPos, startOffset, kWordSize - 1, leftPart);
    write_incell_interval(endPos, 0, endOffset, rightPart);
  }

  void write_incell_interval(const unsigned int& idx, const unsigned int& l,
                             const unsigned int& r,
                             const unsigned int& val) const {
    unsigned int croppedVal =
        get_mask_interval(val, kWordSize - length, kWordSize - 1);
    unsigned int v = clear_mask_interval(array[idx], l, r);
    array[idx] = v | (croppedVal << (kWordSize - r - 1));
  }

  static unsigned int clear_mask_interval(const unsigned int& val,
                                          const unsigned int& l,
                                          const unsigned int& r) {
    return val ^ get_mask_interval(val, l, r);
  }

  static unsigned int get_mask_interval(const unsigned int& val,
                                        const unsigned int& l,
                                        const unsigned int& r) {
    unsigned int rmask = val & ((1 << (kWordSize - r - 1)) - 1);
    unsigned int lmask = val & ((1 << (kWordSize - l)) - 1);
    return lmask ^ rmask;
  }
};