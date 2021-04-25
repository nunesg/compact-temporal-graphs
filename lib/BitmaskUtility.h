#pragma once

#include "glog/logging.h"

namespace compact {
namespace lib {

class BitmaskUtility {
 public:
  // sizes measured in bits
  static const uint kMaxLength = 31;
  static const uint kWordSize = sizeof(uint) * 8;

  // return the amount of bits until the most-significant 1
  static uint count_bits(uint mask) {
    uint count = 0;
    while (mask > 0) {
      count++;
      mask >>= 1;
    }
    return count;
  }

  static uint clear_mask_interval(uint mask, uint l, uint r) {
    return mask ^ get_mask_interval(mask, l, r);
  }

  static uint get_mask_interval(uint mask, uint l, uint r) {
    return ((mask >> l) & ((1 << (r - l + 1)) - 1)) << l;
  }
};

}  // namespace lib
}  // namespace compact
