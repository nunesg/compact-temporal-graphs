#pragma once

#include <assert.h>

#include <memory>
#include <utility>

#include "glog/logging.h"
#include "lib/Array.h"
#include "lib/BitArray.h"

namespace compact {
namespace lib {

template <typename BitArrayType = BitArray>
class JacobsonRank {
 public:
  JacobsonRank() : bit_stream_ptr(NULL) {}

  JacobsonRank(const std::shared_ptr<BitArrayType>& bits) { reset(bits); }

  void reset(const std::shared_ptr<BitArrayType>& bits) {
    bit_stream_ptr = bits;
  }

  void build() {
    assert(bit_stream_ptr != NULL);
    uint n = bit_stream_ptr->size();
    uint logn = BitmaskUtility::int_log(n) + 1;
    small_block_size = logn;
    big_block_size = logn * logn;
    uint bit_size = BitmaskUtility::int_log(big_block_size) + 1;  // 2*loglogn
    big_block_rank.resize(1 + n / big_block_size, logn);  // O(n/logn) = o(n)
    small_block_rank.resize(1 + n / small_block_size,
                            bit_size);  // O(loglogn*n/logn) = o(n)
    build_blocks();
  }

  uint rank(uint pos) const {
    if (pos >= bit_stream_ptr->size()) {
      return total_rank;
    }
    uint big_block_idx = get_big_block(pos);
    uint small_block_idx = get_small_block(pos);
    uint in_small_block_mask = bit_stream_ptr->read_interval(
        get_start_of_small_block(small_block_idx), pos);

    return big_block_rank[big_block_idx] + small_block_rank[small_block_idx] +
           BitmaskUtility::popcount(in_small_block_mask) -
           (*bit_stream_ptr)[pos];
  }

 private:
  uint total_rank;
  uint big_block_size;
  uint small_block_size;
  FixedSizeArray big_block_rank;
  FixedSizeArray small_block_rank;
  std::shared_ptr<BitArrayType> bit_stream_ptr;

  void build_blocks() {
    uint n = bit_stream_ptr->size();
    uint big_block_sum = 0;
    uint small_block_sum = 0;
    for (uint i = 0; i < n; i++) {
      uint big_block_idx = get_big_block(i);
      uint small_block_idx = get_small_block(i);
      if (get_start_of_big_block(big_block_idx) == i) {
        big_block_rank.write(big_block_idx, big_block_sum);
        small_block_sum = 0;
      }
      if (get_start_of_small_block(small_block_idx) == i) {
        small_block_rank.write(small_block_idx, small_block_sum);
      }
      big_block_sum += (*bit_stream_ptr)[i];
      small_block_sum += (*bit_stream_ptr)[i];
    }
    total_rank = big_block_sum;
  }

  uint get_big_block(uint i) const { return i / big_block_size; }

  uint get_small_block(uint i) const { return i / small_block_size; }

  uint get_start_of_big_block(uint b) const { return b * big_block_size; }

  uint get_start_of_small_block(uint b) const { return b * small_block_size; }
};

}  // namespace lib
}  // namespace compact
