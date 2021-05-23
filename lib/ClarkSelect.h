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
              const std::shared_ptr<JacobsonRank>& rank_mgr) {
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
    uint sqrtlogn = uint(sqrt(BitmaskUtility::int_log(logn) + 1)) + 1;
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
  std::shared_ptr<JacobsonRank> rank_manager_ptr;

  void setup_big(uint n, uint logn, uint log2n) {
    big_block_weight = log2n;
    big_block_threshold = big_block_weight * big_block_weight;
    uint n_big_blocks = 1 + n / big_block_weight;

    big_block_select.resize(n_big_blocks,
                            logn);  // O(n/logn) = o(n)

    const uint big_sparse_size = 1 + n / big_block_threshold;  // O(n/log^4)
    big_sparse_lookup = new FixedSizeArray[big_sparse_size];
    for (uint i = 0; i < big_sparse_size; i++) {
      big_sparse_lookup[i].resize(big_block_weight, logn);  // O(log^3)
    }
    big_block_sparse.resize(n_big_blocks);
  }

  void setup_small(uint n, uint logn, uint sqrtlogn, uint loglogn) {
    small_block_weight = sqrtlogn;
    small_block_threshold = 1 + logn / 2;
    uint n_small_blocks = 1 + n / small_block_weight;
    uint rel_val_bit_size = std::min(logn, 4 * loglogn);
    small_block_select.resize(
        n_small_blocks,
        rel_val_bit_size);  // O(loglogn*n/sqrtlogn) = o(n)

    const uint small_sparse_size = 1 + n / small_block_threshold;  // O(n/logn)
    small_sparse_lookup = new FixedSizeArray[small_sparse_size];
    for (uint i = 0; i < small_sparse_size; i++) {
      small_sparse_lookup[i].resize(small_block_weight,
                                    rel_val_bit_size);  // O(sqrtlog*loglog)
    }
    small_block_sparse.resize(n_small_blocks);
  }

  // TODO
  void build_blocks() {}

  uint get_big_block(uint i) const { return i / big_block_size; }

  uint get_small_block(uint i) const { return i / small_block_size; }

  bool is_big_block_sparse(uint b) const { return big_block_sparse[b]; }

  bool is_small_block_sparse(uint b) const { return small_block_sparse[b]; }
};

}  // namespace lib
}  // namespace compact
