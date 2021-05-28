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

  ClarkSelect(const std::shared_ptr<BitArrayType>& bits) {
    bit_stream_ptr = bits;
  }

  void build() {
    assert(bit_stream_ptr != NULL);
    uint n = bit_stream_ptr->size();
    uint logn = BitmaskUtility::int_log(n) + 1;
    uint log2n = logn * logn;
    uint loglogn = BitmaskUtility::int_log(logn) + 1;
    uint sqrtlogn = uint(sqrt(logn)) + 1;
    LOG(INFO) << "n = " << n;
    LOG(INFO) << "logn = " << logn;
    LOG(INFO) << "log2n = " << log2n;
    LOG(INFO) << "loglogn = " << loglogn;
    LOG(INFO) << "sqrtlogn = " << sqrtlogn;
    setup_big(n, logn, log2n);
    setup_small(n, logn, sqrtlogn, loglogn);
    build_blocks();
  }

  // TODO
  uint select(uint idx) const { return 0; }

 private:
  uint total_rank;
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
  // answer whether big blocks are sparse
  std::shared_ptr<BitArrayType> big_block_sparse_ptr;
  // rank manager for big_block_sparse_ptr bitarray
  JacobsonRank<BitArrayType> big_sparse_rank_manager;
  // answer whether small blocks are sparse
  std::shared_ptr<BitArrayType> small_block_sparse_ptr;
  // rank manager for big_block_sparse_ptr bitarray
  JacobsonRank<BitArrayType> small_sparse_rank_manager;

  std::shared_ptr<BitArrayType> bit_stream_ptr;

  void setup_big(uint n, uint logn, uint log2n) {
    big_block_weight = log2n;
    big_block_threshold = big_block_weight * big_block_weight;
    uint n_big_blocks = 1 + n / big_block_weight;

    big_block_select.resize(n_big_blocks,
                            logn);  // O(n/logn) = o(n)
    LOG(INFO) << "big_block_weight: " << big_block_weight;
    LOG(INFO) << "big_block_threshold: " << big_block_threshold;
    LOG(INFO) << "n_big_blocks: " << n_big_blocks;
    const uint big_sparse_size = 1 + n / big_block_threshold;  // O(n/log^4)
    big_sparse_lookup = new FixedSizeArray[big_sparse_size];
    for (uint i = 0; i < big_sparse_size; i++) {
      big_sparse_lookup[i].resize(big_block_weight, logn);  // O(log^3)
      LOG(INFO) << "i = " << i;
    }
    big_block_sparse_ptr.reset(new BitArrayType(n_big_blocks));
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
    LOG(INFO) << "small_per_big: " << small_per_big;
    LOG(INFO) << "small_block_threshold: " << small_block_threshold;

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
      total_rank += (*bit_stream_ptr)[i];
    build_big_blocks();
    build_small_blocks();
  }

  void build_big_blocks() {
    LOG(INFO) << "build_big_blocks";
    uint n = bit_stream_ptr->size();
    uint total_sum = 0;
    uint previous_block = 0xffffffff;
    for (uint i = 0; i < n; i++) {
      uint block = get_big_block(total_sum);
      if (block != previous_block) {
        LOG(INFO) << "write(pos = " << get_big_block(total_sum)
                  << ", offset = " << i;
        big_block_select.write(block, i);
      }
      previous_block = block;
      uint bit = (*bit_stream_ptr)[i];
      total_sum += bit;
    }
    build_big_sparse();
    build_big_sparse_lookup();
    LOG(INFO) << "bit_stream: " << bit_stream_ptr->to_string();
    LOG(INFO) << "big_block_weight: " << big_block_weight;
    LOG(INFO) << "big_block_select: " << big_block_select.to_string();
    LOG(INFO) << "big_block_sparse_ptr: " << big_block_sparse_ptr->to_string();
  }

  void build_big_sparse() {
    LOG(INFO) << "build big sparse";
    uint n = bit_stream_ptr->size();
    uint last_block = get_big_block(total_rank);

    for (uint i = 0; i <= last_block; i++) {
      uint nxt_val = i == last_block ? n : big_block_select[i + 1];
      uint delta = nxt_val - big_block_select[i];
      big_block_sparse_ptr->write(i, delta >= big_block_threshold);
    }
    big_sparse_rank_manager.reset(big_block_sparse_ptr);
    big_sparse_rank_manager.build();
  }

  void build_big_sparse_lookup() {
    LOG(INFO) << "build big sparse lookup";
    uint n = bit_stream_ptr->size();
    uint last_block = 0xffffffff;
    uint sum_block = 0;
    uint total_sum = 0;
    uint count_sparse = 0;
    uint inblock_offset = 0;
    for (uint i = 0; i < n; i++) {
      LOG(INFO) << "i = " << i;
      if ((*bit_stream_ptr)[i] == 0) continue;
      uint block = get_big_block(total_sum);
      if (block != last_block) {
        sum_block = 0;
        inblock_offset = 0;
      }
      if (is_big_block_sparse(block)) {
        big_sparse_lookup[get_big_sparse_index(block)].write(sum_block,
                                                             inblock_offset);
        LOG(INFO) << "big_sparse_lookup[" << get_big_sparse_index(block)
                  << "].write(" << sum_block << "," << inblock_offset << ")";
        count_sparse += block != last_block;
      }
      last_block = block;
      total_sum++;  // sum after to be 0-indexed
      sum_block++;
      inblock_offset++;
    }
    for (uint i = 0; i < count_sparse; i++) {
      LOG(INFO) << "big_sparse_lookup[" << i
                << "]: " << big_sparse_lookup[i].to_string();
    }
  }

  void build_small_blocks() {
    LOG(INFO) << "build_small_blocks";
    uint n = bit_stream_ptr->size();
    uint big_block_sum = 0;
    uint total_sum = 0;
    uint previous_big_block = 0xffffffff;
    uint previous_small_block = 0xffffffff;
    uint inblock_offset = 0;
    for (uint i = 0; i < n; i++) {
      uint small_block = get_small_block(total_sum);
      uint big_block = get_big_block(total_sum);
      if (big_block != previous_big_block) {
        LOG(INFO) << "beginning of the big block " << get_big_block(total_sum)
                  << ". reset local select, offset = " << i;
        small_block_select.write(small_block, i);
        big_block_sum = 0;
        inblock_offset = 0;
      }
      if (small_block != previous_small_block) {
        LOG(INFO) << "beginning of a small block " << get_small_block(total_sum)
                  << ". Write offset: " << inblock_offset;
        small_block_select.write(small_block, inblock_offset);
      }
      previous_big_block = big_block;
      previous_small_block = small_block;
      uint bit = (*bit_stream_ptr)[i];
      total_sum += bit;
      big_block_sum += bit;
      inblock_offset++;
    }
    build_small_sparse();
    build_small_sparse_lookup();

    LOG(INFO) << "bit_stream: " << bit_stream_ptr->to_string();
    LOG(INFO) << "small_block_weight: " << small_block_weight;
    LOG(INFO) << "small_block_select: " << small_block_select.to_string();
    LOG(INFO) << "small_block_sparse_ptr: "
              << small_block_sparse_ptr->to_string();
  }

  void build_small_sparse() {
    uint n = bit_stream_ptr->size();
    uint last_block = get_small_block(total_rank);

    for (uint i = 0; i <= last_block; i++) {
      uint nxt_val = i == last_block ? n : get_small_block_offset(i + 1);
      uint delta = nxt_val - get_small_block_offset(i);
      small_block_sparse_ptr->write(i, delta >= small_block_threshold);
      LOG(INFO) << "small_block_sparse.write(" << i << ", " << delta
                << ">=" << small_block_threshold << " = "
                << (delta >= small_block_threshold) << ")";
    }
    small_sparse_rank_manager.reset(small_block_sparse_ptr);
    small_sparse_rank_manager.build();
  }

  void build_small_sparse_lookup() {
    LOG(INFO) << "build small sparse lookup";
    uint n = bit_stream_ptr->size();
    uint last_block = 0xffffffff;
    uint sum_block = 0;
    uint total_sum = 0;
    uint count_sparse = 0;
    uint inblock_offset = 0;
    for (uint i = 0; i < n; i++) {
      if ((*bit_stream_ptr)[i] == 0) continue;
      uint block = get_small_block(total_sum);
      LOG(INFO) << "i = " << i << ", small_block = " << block;
      if (block != last_block) {
        sum_block = 0;
        inblock_offset = 0;
      }
      if (is_small_block_sparse(block)) {
        small_sparse_lookup[get_small_sparse_index(block)].write(
            sum_block, inblock_offset);
        LOG(INFO) << "small_sparse_lookup[" << get_small_sparse_index(block)
                  << "].write(" << sum_block << "," << inblock_offset << ")";
        count_sparse += block != last_block;
      }
      last_block = block;
      total_sum++;  // sum after to be 0-indexed
      sum_block++;
      inblock_offset++;
    }
    LOG(INFO) << "count_sparse = " << count_sparse;
    for (uint i = 0; i < count_sparse; i++) {
      LOG(INFO) << "small_sparse_lookup[" << i
                << "]: " << small_sparse_lookup[i].to_string();
    }
  }

  uint get_big_block(uint i) const { return i / big_block_weight; }

  uint get_small_block(uint i) const {
    uint small_per_big = ceil(1.0 * big_block_weight / small_block_weight);
    uint bblock = get_big_block(i);
    return bblock * small_per_big + (i % big_block_weight) / small_block_weight;
  }

  bool is_big_block_sparse(uint b) const { return (*big_block_sparse_ptr)[b]; }

  bool is_small_block_sparse(uint b) const {
    return (*small_block_sparse_ptr)[b];
  }

  uint get_small_sparse_index(uint b) const {
    // LOG(INFO) << "small_sparse_index(" << b
    //           << ") = " << small_sparse_rank_manager.rank(b);
    return small_sparse_rank_manager.rank(b);
  }

  uint get_big_sparse_index(uint b) const {
    return big_sparse_rank_manager.rank(b);
  }

  uint get_small_block_offset(uint b) const {
    uint small_per_big = ceil(1.0 * big_block_weight / small_block_weight);
    uint bblock = b / small_per_big;
    return big_block_select[bblock] + small_block_select[b];
  }
};

}  // namespace lib
}  // namespace compact
