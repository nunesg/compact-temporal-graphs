#pragma once

#include <math.h>

#include <utility>

#include "BitmaskUtility.h"
#include "FixedSizeArray.h"
#include "glog/logging.h"

namespace compact {
namespace lib {

class GamaUtility {
 public:
  using BitArray = FixedSizeArray<1>;

  /*
    Get the Gama code of a single value.
    Example: 3 -> 00100 (binary_length(4)-1 zeroes + 4 representation in binary
  */
  static BitArray get_code(uint val) {
    val++;
    uint sz = get_code_length(val);
    BitArray arr;
    arr.assign(sz, 0);
    for (uint i = sz >> 1; i < sz; i++) {
      arr.write(i, (val >> (sz - i - 1)) & 1);
    }
    return arr;
  }

  /*
    Get the bit array representing the binary stream in gama-compression format
    for a given array of values. Example: {2,3,4} -> {0110010000101}
  */
  template <typename ArrayType>
  static BitArray get_array_code(const ArrayType& values) {
    uint totalSize = get_array_code_length(values);
    BitArray arr;
    arr.assign(totalSize, 0);
    for (uint i = 0, arridx = 0; i < values.size(); i++) {
      uint val = values[i] + 1;
      uint sz = get_code_length(val);

      arridx += sz >> 1;
      for (uint j = sz >> 1; j < sz; j++) {
        arr.write(arridx, (val >> (sz - j - 1)) & 1);
        arridx++;
      }
    }
    return arr;
  }

  /*
    Given a bit array (considered being in gama format) and a starting point,
    return the value decoded from the given position, updating the given pointer
    to the starting position of the next value.
    Example: for bitArray = {0110010000101}

    uint start = 0;
    decode_next(bitArray, start) -> returned value = 2, start = 3

    bit chunk decoded: "011"
  */
  static uint decode_next(const BitArray& arr, uint& start) {
    uint result = 0;
    int count_zeroes = 0;
    int delta = 1;

    for (uint& i = start; i < arr.size(); i++) {
      if (count_zeroes < 0) break;
      if (arr[i] == 1) delta = -1;
      count_zeroes += delta;

      result = (result << 1) | arr[i];
    }
    if (count_zeroes != -1) {
      throw std::runtime_error("Decoding failed, invalid starting point.");
    }
    return result - 1;
  }

 private:
  /*
    Return the length of the gama code representing the given value
    Example: code length of 3 is 5
    3 -> 00100
  */
  static uint get_code_length(uint val) {
    return (BitmaskUtility::count_bits(val) << 1) - 1;
  }

  /*
    Return the sum of code lengths for an array of values
  */
  template <typename ArrayType>
  static uint get_array_code_length(const ArrayType& arr) {
    uint sum = 0;
    for (uint i = 0; i < arr.size(); i++) {
      sum += get_code_length(arr[i] + 1);
    }
    return sum;
  }
};

}  // namespace lib
}  // namespace compact
