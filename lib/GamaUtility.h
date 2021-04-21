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

  static BitArray get_code(uint val) {
    if (val < 0) {
      throw std::runtime_error(
          "GamaUtility only supports non-negative integers for now.");
    }

    val++;
    int nbits = count_bits(val), sz = (nbits << 1) - 1;
    BitArray arr;
    arr.assign(sz, 0);
    for (int i = nbits - 1; i < sz; i++) {
      arr.write(i, (val >> (sz - i - 1)) & 1);
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
};

}  // namespace lib
}  // namespace compact
