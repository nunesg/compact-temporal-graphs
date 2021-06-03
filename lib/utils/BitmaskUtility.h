#pragma once

#include "glog/logging.h"

namespace compact {
namespace lib {

class BitmaskUtility {
 public:
  // sizes measured in bits
  static const uint kWordSize;

  static uint popcount(uint mask) { return __builtin_popcount(mask); }

  static uint clz(uint mask) { return __builtin_clz(mask); }

  static uint ctz(uint mask) { return __builtin_ctz(mask); }

  static uint rank(uint mask, uint pos, uint bit_value = 1) {
    bit_value %= 2;
    if (pos == 0) return 0;
    uint rank1 = popcount(get_mask_prefix(mask, pos - 1));
    if (bit_value)
      return rank1;
    else
      return pos - rank1;
  }

  static uint select(uint mask, uint i, uint prefix_size = kWordSize,
                     uint bit_value = 1) {
    if (!prefix_size) {
      return 0;
    }
    bit_value %= 2;
    prefix_size = std::min(prefix_size, kWordSize);
    if (!bit_value) {
      mask = get_mask_prefix(~mask, prefix_size - 1);
    }
    if (i >= popcount(mask)) return kWordSize;
    while (i--) {
      mask -= mask & (-mask);  // remove LSB
    }
    return ctz(mask);
  }

  // return the amount of bits until the most-significant 1
  static uint count_bits(uint mask) { return kWordSize - clz(mask); }

  static uint int_log(uint mask) { return mask ? count_bits(mask) - 1 : 0; }

  static uint clear_mask_interval(uint mask, uint l, uint r) {
    check_interval(l, r);
    return mask ^ get_mask_interval(mask, l, r);
  }

  static uint get_most_significant(uint i) {
    check_index(i);
    return kWordSize - i - 1;
  }

  /*
    get only the bits on the interval [l,r] active
  */
  static uint get_mask_interval(uint mask, uint l, uint r) {
    check_interval(l, r);
    if (l == 0) return get_mask_prefix(mask, r);
    return get_mask_prefix(mask, r) ^ get_mask_prefix(mask, l - 1);
  }

  /*
    get the mask bits until index i
  */
  static uint get_mask_prefix(uint mask, uint i) {
    check_index(i);
    return mask & get_full_ones(i + 1);
  }

  static uint get_full_ones(uint n) { return uint((1LL << n) - 1LL); }

  static void check_interval(uint l, uint r) {
    if (l > r) {
      throw std::runtime_error(
          std::string() + "Invalid mask interval, l = " + std::to_string(l) +
          " should be < than r = " + std::to_string(r));
    }
    if ((r - l + 1) > kWordSize) {
      throw std::runtime_error(std::string() + "Invalid mask interval (" +
                               std::to_string(l) + "," + std::to_string(r) +
                               "). Should fit in a computer 32bit word");
    }
  }

  static void check_index(uint i) {
    if (i >= kWordSize) {
      throw std::runtime_error(std::string() + "Invalid mask index " +
                               std::to_string(i) + ", should be < than " +
                               std::to_string(kWordSize));
    }
  }
};

const uint BitmaskUtility::kWordSize = sizeof(uint) * 8;

}  // namespace lib
}  // namespace compact
