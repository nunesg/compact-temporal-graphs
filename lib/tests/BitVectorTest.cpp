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
#include "lib/VariableSizeArray.h"

namespace compact {
namespace lib {
namespace test {

std::shared_ptr<BitArray> get_random_bitarray(uint n, uint p1 = 50) {
  std::shared_ptr<BitArray> arr_ptr(new BitArray(n));
  srand((unsigned)time(NULL));
  for (uint i = 0; i < n; i++) arr_ptr->write(i, (uint(rand()) % 100) < p1);
  return arr_ptr;
}

uint linear_select(const std::shared_ptr<BitArray>& arr_ptr, uint idx,
                   uint bit_value = 1) {
  for (uint i = 0; i < arr_ptr->size(); i++) {
    if (arr_ptr->read(i) == bit_value && !idx) return i;
    idx -= arr_ptr->read(i) == bit_value;
  }
  return arr_ptr->size();
}

uint linear_rank(const std::shared_ptr<BitArray>& arr_ptr, uint idx,
                 uint bit_value = 1) {
  uint ans = 0;
  for (uint i = 0; i < std::min(idx, arr_ptr->size()); i++) {
    ans += arr_ptr->read(i) == bit_value;
  }
  return ans;
}

// test BitVector
TEST(BitVectorTest, bitVectorTest) {
  /*
    init
  */
  std::shared_ptr<BitVector> bitv;

  // test constructors
  bitv.reset(new BitVector());
  EXPECT_EQ(bitv->size(), 0);
  EXPECT_EQ(bitv->rank(1), 0);
  EXPECT_EQ(bitv->select(1), 0);

  bitv.reset(new BitVector(5));
  EXPECT_EQ(bitv->size(), 5);
  EXPECT_EQ(bitv->rank(5), 0);
  EXPECT_EQ(bitv->select(1), 5);

  VariableSizeArray v_arr{0, 1, 0, 1, 0};
  bitv.reset(new BitVector(v_arr));
  EXPECT_EQ(bitv->size(), 5);
  EXPECT_EQ(bitv->rank(5), 2);
  EXPECT_EQ(bitv->select(1), 3);

  std::vector<uint> vec{0, 1, 0, 1, 0};
  bitv.reset(new BitVector(vec));
  EXPECT_EQ(bitv->size(), 5);
  EXPECT_EQ(bitv->rank(5), 2);
  EXPECT_EQ(bitv->select(1), 3);

  bitv.reset(new BitVector{0, 1, 0, 1, 0});
  EXPECT_EQ(bitv->size(), 5);
  EXPECT_EQ(bitv->rank(5), 2);
  EXPECT_EQ(bitv->select(1), 3);

  // test operations
  uint iterations = 20;
  for (uint bit_value = 0; bit_value < 2; bit_value++) {
    for (uint i = 0; i < iterations; i++) {
      uint p = i * (100 / iterations);
      auto bit_stream_ptr = get_random_bitarray(1000, p);
      bitv.reset(new BitVector(*bit_stream_ptr));

      // test rank
      for (uint i = 0; i < bitv->size(); i++) {
        EXPECT_EQ(bitv->rank(i, bit_value),
                  linear_rank(bit_stream_ptr, i, bit_value));
      }

      // test select
      for (uint i = 0; i < bitv->size(); i++) {
        EXPECT_EQ(bitv->select(i, bit_value),
                  linear_select(bit_stream_ptr, i, bit_value));
      }

      // test read
      for (uint i = 0; i < bitv->size(); i++) {
        EXPECT_EQ((*bitv)[i], (*bit_stream_ptr)[i]);
      }
    }
  }
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
  std::shared_ptr<ClarkSelect<BitArray>> clark[2];

  for (uint bit_value = 0; bit_value < 2; bit_value++) {
    // bitvector of size 1
    bitv.reset(new BitArray{0});
    clark[bit_value].reset(new ClarkSelect<BitArray>(bitv, bit_value));
    clark[bit_value]->build();
    for (uint i = 0; i < bitv->size(); i++) {
      EXPECT_EQ(clark[bit_value]->select(i), linear_select(bitv, i, bit_value));
    }
    bitv.reset(new BitArray{1});
    clark[bit_value].reset(new ClarkSelect<BitArray>(bitv, bit_value));
    clark[bit_value]->build();
    for (uint i = 0; i < bitv->size(); i++) {
      EXPECT_EQ(clark[bit_value]->select(i), linear_select(bitv, i, bit_value));
    }

    uint iterations = 12;
    for (uint i = 0; i < iterations; i++) {
      uint p = i * (100 / iterations);
      bitv = get_random_bitarray(1000, p);
      clark[bit_value].reset(new ClarkSelect<BitArray>(bitv, bit_value));
      clark[bit_value]->build();
      for (uint i = 0; i < bitv->size(); i++) {
        EXPECT_EQ(clark[bit_value]->select(i),
                  linear_select(bitv, i, bit_value));
      }
      // LOG(INFO) << "p = " << p << ", bitarray = " << bitv->to_string();
    }
    for (uint i = 0; 4 * i < iterations; i++) {
      uint p = 4 * i * (100 / iterations);
      bitv = get_random_bitarray(5000, p);
      clark[bit_value].reset(new ClarkSelect<BitArray>(bitv, bit_value));
      clark[bit_value]->build();
      for (uint i = 0; i < bitv->size(); i++) {
        EXPECT_EQ(clark[bit_value]->select(i),
                  linear_select(bitv, i, bit_value));
      }
      // LOG(INFO) << "bitarray = " << bitv->to_string();
    }
  }
}

}  // namespace test
}  // namespace lib
}  // namespace compact