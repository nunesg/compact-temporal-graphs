#pragma once

#include <math.h>

#include <utility>

#include "glog/logging.h"
#include "lib/BitArray.h"
#include "lib/utils/BitmaskUtility.h"

namespace compact {
namespace lib {

class DensePointersUtility {
 public:
  /*
   */
  static BitArray get_code(uint val) {
    if (val < 1) {
      throw std::runtime_error("Invalid value! must be positive.");
    }

    uint sz = get_code_length(val);
    BitArray arr;
    arr.assign(sz, 0);
    for (uint i = 0; i < sz; i++) {
      arr.write(i, (val >> (sz - i - 1)) & 1);
    }
    return arr;
  }

  /*
   */
  template <typename ArrayType>
  static BitArray get_array_code(const ArrayType& values) {
    uint totalSize = get_array_code_length(values);
    BitArray arr;
    arr.assign(totalSize, 0);
    for (uint i = 0, arridx = 0; i < values.size(); i++) {
      uint val = values[i];
      uint sz = get_code_length(val);

      for (uint j = 0; j < sz; j++) {
        arr.write(arridx, (val >> (sz - j - 1)) & 1);
        arridx++;
      }
    }
    return arr;
  }

  /*
   */
  static uint decode(const BitArray& arr, uint start, uint end) {
    if (start < 0 || start > arr.size() || end > arr.size()) {
      throw std::runtime_error("Invalid interval!");
    }

    uint result = 1;
    for (uint i = start; i < end; i++) {
      result = (result << 1) | arr[i];
    }
    return result;
  }

  /*
    Return the length of the code representing the given value without the most
    significant bit.
    Example: code length of 6 (110) is 2
  */
  static uint get_code_length(uint val) {
    return BitmaskUtility::count_bits(val) - 1;
  }

  /*
    Return the sum of code lengths for an array of values
  */
  template <typename ArrayType>
  static uint get_array_code_length(const ArrayType& arr) {
    uint sum = 0;
    for (uint i = 0; i < arr.size(); i++) {
      sum += get_code_length(arr[i]);
    }
    return sum;
  }
};

}  // namespace lib
}  // namespace compact
