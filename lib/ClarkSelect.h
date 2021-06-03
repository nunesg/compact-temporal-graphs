#pragma once

#include <assert.h>

#include <memory>
#include <utility>

#include "glog/logging.h"
#include "lib/Array.h"
#include "lib/BitArray.h"
#include "lib/JacobsonRank.h"

namespace compact {
namespace lib {

template <typename BitArrayType = BitArray>
class ClarkSelect {
 public:
  ClarkSelect() : bit_stream_ptr(NULL), bit_value(1) {}

  ClarkSelect(const std::shared_ptr<BitArrayType>& bits, uint bit_val = 1) {
    bit_stream_ptr = bits;
    bit_value = bit_val;
  }

  void build() {
    assert(bit_stream_ptr != NULL);
    uint n = bit_stream_ptr->size();
    uint logn = BitmaskUtility::int_log(n) + 1;
    uint log2n = logn * logn;
    uint loglogn = BitmaskUtility::int_log(logn) + 1;
    uint sqrtlogn = uint(sqrt(logn)) + 1;
    setup_big(n, logn, log2n);
    setup_small(n, logn, sqrtlogn, loglogn);
    build_blocks();
  }

  uint select(uint idx) const {
    uint n = bit_stream_ptr->size();
    if (idx >= total_rank) return n;

    uint big_block = get_big_block(idx);
    if (is_big_block_sparse(big_block)) {  // big block sparse
      uint sparse_index = get_big_sparse_index(big_block);
      return big_block_select[big_block] +
             big_sparse_lookup[sparse_index][idx % big_block_weight];
    }
    // big block dense, check small block
    uint small_block = get_small_block(idx);
    idx %= big_block_weight;
    idx %= small_block_weight;

    // small block sparse, check lookup table
    if (is_small_block_sparse(small_block)) {
      uint sparse_index = get_small_sparse_index(small_block);
      return big_block_select[big_block] + small_block_select[small_block] +
             small_sparse_lookup[sparse_index][idx];
    }
    // small block dense, check bitmask
    uint start = get_small_block_offset(small_block);
    uint end =
        std::min(start + BitmaskUtility::kWordSize, bit_stream_ptr->size()) - 1;
    return big_block_select[big_block] + small_block_select[small_block] +
           BitmaskUtility::select(bit_stream_ptr->read_interval(start, end),
                                  idx, end - start + 1, bit_value);
  }

 private:
  // which kind of bit this select is about: 0s or 1s
  uint bit_value;
  // amount of bits equals to bit_value on the bit_stream
  uint total_rank;
  // (logn)^2 bit_value bits per big block
  uint big_block_weight;
  // number of big blocks
  uint n_big_blocks;
  // (logn)^4
  uint big_block_threshold;
  // sqrt(logn) bit_value bits per small block
  uint small_block_weight;
  // number of small blocks
  uint n_small_blocks;
  // logn/2
  uint small_block_threshold;
  // offsets of the beginning of each block
  FixedSizeArray big_block_select;
  FixedSizeArray small_block_select;
  // sparse big block lookup table (block, idx_on_block) -> answer
  FixedSizeArray* big_sparse_lookup;  // O(n/logn)
  // sparse small block lookup table (subblock, idx_on_subblock) -> answer
  FixedSizeArray* small_sparse_lookup;  // O(n*loglogn/sqrtlogn)
  // answer whether big blocks are sparse
  std::shared_ptr<BitArrayType> big_block_sparse_ptr;
  // rank manager for big_block_sparse_ptr bitarray
  JacobsonRank<BitArrayType> big_sparse_rank_manager;
  // answer whether small blocks are sparse
  std::shared_ptr<BitArrayType> small_block_sparse_ptr;
  // rank manager for big_block_sparse_ptr bitarray
  JacobsonRank<BitArrayType> small_sparse_rank_manager;
  // bit array
  std::shared_ptr<BitArrayType> bit_stream_ptr;

  void setup_big(uint n, uint logn, uint log2n) {
    big_block_weight = log2n;                                   // log^2
    big_block_threshold = big_block_weight * big_block_weight;  // log^4
    uint n_big_blocks = 1 + n / big_block_weight;

    big_block_select.resize(n_big_blocks,
                            logn);  // O(n/logn) = o(n)
    const uint big_sparse_size = 1 + n / big_block_threshold;  // O(n/log^4)
    big_sparse_lookup = new FixedSizeArray[big_sparse_size];
    for (uint i = 0; i < big_sparse_size; i++) {
      big_sparse_lookup[i].resize(big_block_weight, logn);  // O(log^3)
    }
    big_block_sparse_ptr.reset(new BitArrayType(n_big_blocks));
  }

  void setup_small(uint n, uint logn, uint sqrtlogn, uint loglogn) {
    small_block_weight = sqrtlogn;
    small_block_threshold = 1 + logn / 2;
    uint rel_val_bit_size = std::min(logn, 4 * loglogn);  // log log^4 = 4loglog
    uint n_big_blocks = big_block_select.size();
    uint n_small_blocks = small_per_big() * n_big_blocks;
    small_block_select.resize(
        n_small_blocks,
        rel_val_bit_size);  // O(loglogn*n/sqrtlogn) = o(n)

    const uint small_sparse_size = 1 + n / small_block_threshold;  // O(n/logn)
    small_sparse_lookup = new FixedSizeArray[small_sparse_size];
    for (uint i = 0; i < small_sparse_size; i++) {
      small_sparse_lookup[i].resize(small_block_weight,
                                    rel_val_bit_size);  // O(sqrtlog*loglog)
    }
    small_block_sparse_ptr.reset(new BitArrayType(n_small_blocks));
  }

  void build_blocks() {
    total_rank = 0;
    for (uint i = 0; i < bit_stream_ptr->size(); i++)
      total_rank += get_bit_weight((*bit_stream_ptr)[i]);
    build_big_blocks();
    build_small_blocks();
  }

  void build_big_blocks() {
    uint n = bit_stream_ptr->size();
    uint total_sum = 0;
    uint previous_block = 0xffffffff;
    for (uint i = 0; i < n; i++) {
      uint block = get_big_block(total_sum);
      if (block != previous_block) {
        big_block_select.write(block, i);
      }
      n_big_blocks = block + 1;
      previous_block = block;
      uint bit = get_bit_weight((*bit_stream_ptr)[i]);
      total_sum += bit;
    }
    build_big_sparse();
    build_big_sparse_lookup();
  }

  void build_big_sparse() {
    uint n = bit_stream_ptr->size();
    for (uint i = 0; i < n_big_blocks; i++) {
      uint nxt_val = i + 1 == n_big_blocks ? n : big_block_select[i + 1];
      assert(nxt_val >= big_block_select[i]);
      uint delta = nxt_val - big_block_select[i];
      big_block_sparse_ptr->write(i, delta >= big_block_threshold);
    }
    big_sparse_rank_manager.reset(big_block_sparse_ptr);
    big_sparse_rank_manager.build();
  }

  void build_big_sparse_lookup() {
    uint n = bit_stream_ptr->size();
    uint last_block = 0xffffffff;
    uint sum_block = 0;
    uint total_sum = 0;
    uint inblock_offset = 0;
    for (uint i = 0; i < n; i++) {
      uint block = get_big_block(total_sum);
      if (block != last_block) {
        sum_block = 0;
        inblock_offset = 0;
      }
      if (get_bit_weight((*bit_stream_ptr)[i])) {
        if (is_big_block_sparse(block)) {
          big_sparse_lookup[get_big_sparse_index(block)].write(sum_block,
                                                               inblock_offset);
        }
        total_sum++;  // sum after to be 0-indexed
        sum_block++;
      }
      last_block = block;
      inblock_offset++;
    }
  }

  void build_small_blocks() {
    uint n = bit_stream_ptr->size();
    uint big_block_sum = 0;
    uint total_sum = 0;
    uint previous_big_block = 0xffffffff;
    uint previous_small_block = 0xffffffff;
    uint inblock_offset = 0;
    for (uint i = 0; i < n; i++) {
      uint small_block = get_small_block(total_sum);
      uint big_block = get_big_block(total_sum);
      if (big_block != previous_big_block) {  // beginning of big block
        small_block_select.write(small_block, i);
        big_block_sum = 0;
        inblock_offset = 0;
      }
      if (small_block != previous_small_block) {  // beginning of small block
        small_block_select.write(small_block, inblock_offset);
      }
      n_small_blocks = small_block + 1;
      previous_big_block = big_block;
      previous_small_block = small_block;
      uint bit = get_bit_weight((*bit_stream_ptr)[i]);
      total_sum += bit;
      big_block_sum += bit;
      inblock_offset++;
    }
    build_small_sparse();
    build_small_sparse_lookup();
  }

  void build_small_sparse() {
    uint n = bit_stream_ptr->size();
    for (uint i = 0; i < n_small_blocks; i++) {
      uint nxt_val =
          i + 1 == n_small_blocks ? n : get_small_block_offset(i + 1);
      uint block_offset = get_small_block_offset(i);
      assert(nxt_val >= block_offset);
      uint delta = nxt_val - block_offset;
      small_block_sparse_ptr->write(i, delta >= small_block_threshold);
    }
    small_sparse_rank_manager.reset(small_block_sparse_ptr);
    small_sparse_rank_manager.build();
  }

  void build_small_sparse_lookup() {
    uint n = bit_stream_ptr->size();
    uint last_block = 0;
    uint sum_block = 0;
    uint total_sum = 0;
    uint inblock_offset = 0;
    for (uint i = 0; i < n; i++) {
      uint block = get_small_block(total_sum);
      if (block != last_block) {
        sum_block = 0;
        inblock_offset = 0;
      }
      if (get_bit_weight((*bit_stream_ptr)[i])) {
        if (is_small_block_sparse(block)) {
          small_sparse_lookup[get_small_sparse_index(block)].write(
              sum_block, inblock_offset);
        }
        total_sum++;  // sum after to be 0-indexed
        sum_block++;
      }
      last_block = block;
      inblock_offset++;
    }
  }

  bool is_big_block_sparse(uint b) const { return (*big_block_sparse_ptr)[b]; }

  bool is_small_block_sparse(uint b) const {
    return (*small_block_sparse_ptr)[b];
  }

  uint get_bit_weight(uint bit) const { return bit == bit_value; }

  uint get_big_block(uint i) const { return i / big_block_weight; }

  uint get_small_block(uint i) const {
    return get_big_block(i) * small_per_big() +
           (i % big_block_weight) / small_block_weight;
  }

  uint get_small_sparse_index(uint b) const {
    return small_sparse_rank_manager.rank(b);
  }

  uint get_big_sparse_index(uint b) const {
    return big_sparse_rank_manager.rank(b);
  }

  uint get_small_block_offset(uint b) const {
    uint bblock = b / small_per_big();
    return big_block_select[bblock] + small_block_select[b];
  }

  uint small_per_big() const {
    return ceil(1.0 * big_block_weight / small_block_weight);
  }
};

}  // namespace lib
}  // namespace compact
