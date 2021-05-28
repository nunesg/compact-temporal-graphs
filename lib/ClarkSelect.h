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
class ClarkSelect {
 public:
  ClarkSelect() : bit_stream_ptr(NULL) {}

  ClarkSelect(const std::shared_ptr<BitArrayType>& bits,
              const std::shared_ptr<JacobsonRank<BitArrayType>>& rank_mgr) {
    bit_stream_ptr = bits;
    rank_manager_ptr = rank_mgr;
  }

  void build() {
    assert(bit_stream_ptr != NULL);
    assert(rank_manager_ptr != NULL);
    uint n = bit_stream_ptr->size();
    uint logn = BitmaskUtility::int_log(n) + 1;
    uint log2n = logn * logn;
    uint loglogn = BitmaskUtility::int_log(logn) + 1;
    uint sqrtlogn = uint(sqrt(logn)) + 1;
    // LOG(INFO) << "n = " << n;
    // LOG(INFO) << "logn = " << logn;
    // LOG(INFO) << "log2n = " << log2n;
    // LOG(INFO) << "loglogn = " << loglogn;
    // LOG(INFO) << "sqrtlogn = " << sqrtlogn;
    setup_big(n, logn, log2n);
    setup_small(n, logn, sqrtlogn, loglogn);
    build_blocks();
  }

  // TODO
  uint select(uint idx) const { return 0; }

 private:
  // (logn)^2 1 bits per big block
  uint big_block_weight;
  // (logn)^4
  uint big_block_threshold;
  // sqrt(logn) 1 bits per small block
  uint small_block_weight;
  // logn/2
  uint small_block_threshold;
  // offsets of the beginning of each block
  FixedSizeArray big_block_select;
  FixedSizeArray small_block_select;
  // sparse big block lookup table (block, idx_on_block) -> answer
  FixedSizeArray* big_sparse_lookup;  // O(n/logn)
  // sparse small block lookup table (subblock, idx_on_subblock) -> answer
  FixedSizeArray* small_sparse_lookup;  // O(n*loglogn/sqrtlogn)
  // answer whether blocks are sparse
  BitArrayType big_block_sparse;
  BitArrayType small_block_sparse;

  std::shared_ptr<BitArrayType> bit_stream_ptr;
  std::shared_ptr<JacobsonRank<BitArrayType>> rank_manager_ptr;

  void setup_big(uint n, uint logn, uint log2n) {
    big_block_weight = log2n;
    big_block_threshold = big_block_weight * big_block_weight;
    uint n_big_blocks = 1 + n / big_block_weight;

    big_block_select.resize(n_big_blocks,
                            logn);  // O(n/logn) = o(n)
    // LOG(INFO) << "big_block_weight: " << big_block_weight;
    // LOG(INFO) << "big_block_threshold: " << big_block_threshold;
    // LOG(INFO) << "n_big_blocks: " << n_big_blocks;
    const uint big_sparse_size = 1 + n / big_block_threshold;  // O(n/log^4)
    big_sparse_lookup = new FixedSizeArray[big_sparse_size];
    for (uint i = 0; i < big_sparse_size; i++) {
      big_sparse_lookup[i].resize(big_block_weight, logn);  // O(log^3)
      LOG(INFO) << "i = " << i;
    }
    big_block_sparse.resize(n_big_blocks);
  }

  void setup_small(uint n, uint logn, uint sqrtlogn, uint loglogn) {
    small_block_weight = sqrtlogn;
    small_block_threshold = 1 + logn / 2;
    uint rel_val_bit_size = std::min(logn, 4 * loglogn);
    uint n_big_blocks = big_block_select.size();
    uint small_per_big = ceil(1.0 * big_block_weight / small_block_weight);
    uint n_small_blocks = small_per_big * n_big_blocks;
    small_block_select.resize(
        n_small_blocks,
        rel_val_bit_size);  // O(loglogn*n/sqrtlogn) = o(n)
    // LOG(INFO) << "small_per_big: " << small_per_big;
    // LOG(INFO) << "small_block_threshold: " << small_block_threshold;

    const uint small_sparse_size = 1 + n / small_block_threshold;  // O(n/logn)
    small_sparse_lookup = new FixedSizeArray[small_sparse_size];
    for (uint i = 0; i < small_sparse_size; i++) {
      small_sparse_lookup[i].resize(small_block_weight,
                                    rel_val_bit_size);  // O(sqrtlog*loglog)
    }
    small_block_sparse.resize(n_small_blocks);
  }

  // TODO: build sparse lookups
  void build_blocks() {
    build_big_blocks();
    build_small_blocks();
  }

  void build_big_blocks() {
    // LOG(INFO) << "build_big_blocks";
    uint total_sum = 0;
    uint n = bit_stream_ptr->size();
    big_block_select.write(0, 0);
    for (uint i = 1; i <= n; i++) {
      uint last_bit = (*bit_stream_ptr)[i - 1];
      total_sum += last_bit;
      if (last_bit == 1 && total_sum % big_block_weight == 0) {
        // LOG(INFO) << "write(pos = " << get_big_block(total_sum)
        //           << ", offset = " << i;
        big_block_select.write(get_big_block(total_sum), i);
      }
    }
    uint last_block = get_big_block(total_sum);

    for (uint i = 0; i <= last_block; i++) {
      uint nxt_val = i == last_block ? n : big_block_select[i + 1];
      uint delta = nxt_val - big_block_select[i];
      big_block_sparse.write(i, delta >= big_block_threshold);
    }

    // LOG(INFO) << "bit_stream: " << bit_stream_ptr->to_string();
    // LOG(INFO) << "big_block_weight: " << big_block_weight;
    // LOG(INFO) << "big_block_select: " << big_block_select.to_string();
    // LOG(INFO) << "big_block_sparse: " << big_block_sparse.to_string();
  }

  void build_small_blocks() {
    // LOG(INFO) << "build_small_blocks";
    uint big_block_sum = 0;
    uint total_sum = 0;
    uint n = bit_stream_ptr->size();
    small_block_select.write(0, 0);
    for (uint i = 1; i <= n; i++) {
      uint last_bit = (*bit_stream_ptr)[i - 1];
      total_sum += last_bit;
      big_block_sum += last_bit;
      if (last_bit == 1 && total_sum % big_block_weight == 0) {
        // LOG(INFO) << "beginning of the big block " <<
        // get_big_block(total_sum)
        //           << ". reset local select, offset = " << i;
        small_block_select.write(get_small_block(total_sum), i);
        big_block_sum = 0;
      }
      if (last_bit == 1 && big_block_sum % small_block_weight == 0) {
        // LOG(INFO) << "beginning of a small block " <<
        // get_small_block(total_sum)
        //           << ". Write offset: " << i;
        small_block_select.write(get_small_block(total_sum), i);
      }
    }
    uint last_block = get_small_block(total_sum);
    // LOG(INFO) << "last_small_block: " << last_block;

    for (uint i = 0; i <= last_block; i++) {
      uint nxt_val = i == last_block ? n : small_block_select[i + 1];
      uint delta = nxt_val - small_block_select[i];
      small_block_sparse.write(i, delta >= small_block_threshold);
    }

    // LOG(INFO) << "bit_stream: " << bit_stream_ptr->to_string();
    // LOG(INFO) << "small_block_weight: " << small_block_weight;
    // LOG(INFO) << "small_block_select: " << small_block_select.to_string();
    // LOG(INFO) << "small_block_sparse: " << small_block_sparse.to_string();
  }

  uint get_big_block(uint i) const { return i / big_block_weight; }

  uint get_small_block(uint i) const {
    uint small_per_big = ceil(1.0 * big_block_weight / small_block_weight);
    uint bblock = get_big_block(i);
    return bblock * small_per_big + (i % big_block_weight) / small_block_weight;
  }

  bool is_big_block_sparse(uint b) const { return big_block_sparse[b]; }

  bool is_small_block_sparse(uint b) const { return small_block_sparse[b]; }
};

}  // namespace lib
}  // namespace compact
