#pragma once

#include <math.h>

#include <utility>

#include "Array.h"
#include "FixedSizeArray.h"
#include "glog/logging.h"

namespace compact {
namespace lib {

class GamaUtility {
 public:
  using BitArray = FixedSizeArray<1>;

  static const uint kMaxLength = 31;

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

  static BitArray get_code(const FixedSizeArray<kMaxLength>& values) {
    uint totalSize = get_code_length(values);
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

 private:
  // return the amount of bits until the most-significant 1
  static uint count_bits(uint val) {
    uint count = 0;
    while (val > 0) {
      count++;
      val >>= 1;
    }
    return count;
  }

  static uint get_code_length(uint val) { return (count_bits(val) << 1) - 1; }

  static uint get_code_length(const FixedSizeArray<kMaxLength>& arr) {
    uint sum = 0;
    for (uint i = 0; i < arr.size(); i++) {
      sum += get_code_length(arr[i] + 1);
    }
    return sum;
  }
};

}  // namespace lib
}  // namespace compact
