#include <stdlib.h>
#include <time.h>

#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "lib/BitArray.h"
#include "lib/BitVector.h"
#include "lib/ClarkSelect.h"
#include "lib/JacobsonRank.h"

namespace compact {
namespace lib {
namespace test {

std::shared_ptr<BitArray> get_random_bitarray(uint n, uint p1 = 50) {
  std::shared_ptr<BitArray> arr_ptr(new BitArray(n));
  srand((unsigned)time(NULL));
  for (uint i = 0; i < n; i++) arr_ptr->write(i, (uint(rand()) % 100) < p1);
  return arr_ptr;
}

uint linear_select(const std::shared_ptr<BitArray>& arr_ptr, uint idx) {
  for (uint i = 0; i < arr_ptr->size(); i++) {
    if (arr_ptr->read(i) && !idx) return i;
    idx -= arr_ptr->read(i);
  }
  return arr_ptr->size();
}

uint linear_rank(const std::shared_ptr<BitArray>& arr_ptr, uint idx) {
  uint ans = 0;
  for (uint i = 0; i < std::min(idx, arr_ptr->size()); i++) {
    ans += arr_ptr->read(i);
  }
  return ans;
}

// test BitVector
TEST(BitVectorTest, bitVectorTest) {
  /*
    init
  */
  BitVector bitv;
  BitVector bitv2{1, 1, 0, 1, 1};

  EXPECT_EQ(bitv.size(), 0);
  EXPECT_EQ(bitv2.size(), 5);
  EXPECT_EQ(bitv2[0], 1);
  EXPECT_EQ(bitv2[1], 1);
  EXPECT_EQ(bitv2[2], 0);

  LOG(INFO) << "bitstream: " << bitv2.to_string();

  LOG(INFO) << "rank(0) = " << bitv2.rank(0);
  EXPECT_EQ(bitv2.rank(0), 0);
  LOG(INFO) << "rank(1) = " << bitv2.rank(1);
  EXPECT_EQ(bitv2.rank(1), 1);
  LOG(INFO) << "rank(2) = " << bitv2.rank(2);
  EXPECT_EQ(bitv2.rank(2), 2);
  LOG(INFO) << "rank(3) = " << bitv2.rank(3);
  EXPECT_EQ(bitv2.rank(3), 2);
  LOG(INFO) << "rank(4) = " << bitv2.rank(4);
  EXPECT_EQ(bitv2.rank(4), 3);
  LOG(INFO) << "rank(5) = " << bitv2.rank(5);
  EXPECT_EQ(bitv2.rank(5), 4);
}

// test rank
TEST(BitVectorTest, jacobsonRankTest) {
  /*
    init
  */
  // std::shared_ptr<BitArray> bitv = get_random_bitarray(100);
  // std::shared_ptr<BitArray> bitv(
  //     new BitArray({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  //                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}));
  // std::shared_ptr<BitArray> bitv(new BitArray{1, 1, 1, 1, 0, 1, 0, 1, 1, 0});
  // std::shared_ptr<BitArray> bitv(new BitArray{0, 1, 0, 1, 0, 0, 0, 0, 1, 1});
  // std::shared_ptr<BitArray> bitv(new BitArray{1, 1, 1, 1, 1, 1, 0, 1, 1, 1});
  // std::shared_ptr<BitArray> bitv(
  //     new BitArray{1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0,
  //     1,
  //                  0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
  //                  1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0,
  //                  1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
  //                  0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0,
  //                  0, 1, 0, 1});
  std::shared_ptr<BitArray> bitv;
  std::shared_ptr<JacobsonRank<BitArray>> jacob;

  uint iterations = 20;
  for (uint i = 0; i < iterations; i++) {
    uint p = i * (100 / iterations);
    bitv = get_random_bitarray(1000, p);
    jacob.reset(new JacobsonRank<BitArray>(bitv));
    jacob->build();
    for (uint i = 0; i < bitv->size(); i++) {
      EXPECT_EQ(jacob->rank(i), linear_rank(bitv, i));
    }
    // LOG(INFO) << "p = " << p << ", bitarray = " << bitv->to_string();
  }
}

// test BitVector
TEST(BitVectorTest, clarkSelectTest) {
  /*
    init
  */
  // std::shared_ptr<BitArray> bitv(
  //     new BitArray({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  //                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}));
  // std::shared_ptr<BitArray> bitv(new BitArray{1, 1, 1, 1, 0, 1, 0, 1, 1, 0});
  // std::shared_ptr<BitArray> bitv(new BitArray{0, 1, 0, 1, 0, 0, 0, 0, 1, 1});
  // std::shared_ptr<BitArray> bitv(new BitArray{1, 1, 1, 1, 1, 1, 0, 1, 1, 1});
  // std::shared_ptr<BitArray> bitv(
  //     new BitArray{1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0,
  //     1,
  //                  0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
  //                  1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0,
  //                  1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
  //                  0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0,
  //                  0, 1, 0, 1});
  std::shared_ptr<BitArray> bitv;
  std::shared_ptr<ClarkSelect<BitArray>> clark;

  // bitvector of size 1
  bitv.reset(new BitArray{0});
  clark.reset(new ClarkSelect<BitArray>(bitv));
  clark->build();
  for (uint i = 0; i < bitv->size(); i++) {
    EXPECT_EQ(clark->select(i), linear_select(bitv, i));
  }
  bitv.reset(new BitArray{1});
  clark.reset(new ClarkSelect<BitArray>(bitv));
  clark->build();
  for (uint i = 0; i < bitv->size(); i++) {
    EXPECT_EQ(clark->select(i), linear_select(bitv, i));
  }

  uint iterations = 20;
  for (uint i = 0; i < iterations; i++) {
    uint p = i * (100 / iterations);
    bitv = get_random_bitarray(1000, p);
    clark.reset(new ClarkSelect<BitArray>(bitv));
    clark->build();
    for (uint i = 0; i < bitv->size(); i++) {
      EXPECT_EQ(clark->select(i), linear_select(bitv, i));
    }
    // LOG(INFO) << "p = " << p << ", bitarray = " << bitv->to_string();
  }
  for (uint i = 0; 4 * i < iterations; i++) {
    uint p = 4 * i * (100 / iterations);
    bitv = get_random_bitarray(5000, p);
    clark.reset(new ClarkSelect<BitArray>(bitv));
    clark->build();
    for (uint i = 0; i < bitv->size(); i++) {
      EXPECT_EQ(clark->select(i), linear_select(bitv, i));
    }
    // LOG(INFO) << "bitarray = " << bitv->to_string();
  }
}

}  // namespace test
}  // namespace lib
}  // namespace compact