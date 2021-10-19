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

BitArray get_random_bitarray(uint n, uint p1 = 50) {
  BitArray arr(n);
  srand((unsigned)time(NULL));
  for (uint i = 0; i < n; i++) arr.write(i, (uint(rand()) % 100) < p1);
  return arr;
}

uint linear_select(const BitArray& arr, uint idx, uint bit_value = 1) {
  for (uint i = 0; i < arr.size(); i++) {
    if (arr.read(i) == bit_value && !idx) return i;
    idx -= arr.read(i) == bit_value;
  }
  return arr.size();
}

uint linear_rank(const BitArray& arr, uint idx, uint bit_value = 1) {
  uint ans = 0;
  for (uint i = 0; i < std::min(idx, arr.size()); i++) {
    ans += arr.read(i) == bit_value;
  }
  return ans;
}

void run_clark_linearly(uint n, uint bit_value, uint iterations, uint p = 50) {
  for (uint i = 0; i < iterations; i++) {
    BitArray bitv = get_random_bitarray(n, p);
    ClarkSelect<BitArray> clark(bitv, bit_value);
    clark.build();
    for (uint j = 0; j < bitv.size(); j++) {
      EXPECT_EQ(clark.select(j), linear_select(bitv, j, bit_value));
    }
    // LOG(INFO) << "p = " << p << ", bitarray = " << bitv->to_string();
  }
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
      auto bit_stream = get_random_bitarray(1000, p);
      bitv.reset(new BitVector(bit_stream));
      LOG(INFO) << "memory used: " << bitv->measure_memory();

      // test rank
      for (uint i = 0; i < bitv->size(); i++) {
        EXPECT_EQ(bitv->rank(i, bit_value),
                  linear_rank(bit_stream, i, bit_value));
      }

      // test select
      for (uint i = 0; i < bitv->size(); i++) {
        EXPECT_EQ(bitv->select(i, bit_value),
                  linear_select(bit_stream, i, bit_value));
      }

      // test read
      for (uint i = 0; i < bitv->size(); i++) {
        EXPECT_EQ((*bitv)[i], bit_stream[i]);
      }
    }
  }
}

// test rank
TEST(BitVectorTest, jacobsonRankTest) {
  /*
    init
  */
  uint iterations = 20;
  for (uint i = 0; i < iterations; i++) {
    uint p = i * (100 / iterations);
    BitArray bitv = get_random_bitarray(1000, p);
    JacobsonRank<BitArray> jacob(bitv);
    jacob.build();
    for (uint j = 0; j < bitv.size(); j++) {
      EXPECT_EQ(jacob.rank(j), linear_rank(bitv, j));
    }
    // LOG(INFO) << "p = " << p << ", bitarray = " << bitv->to_string();
  }
}

// test BitVector
TEST(BitVectorTest, clarkSelectTest) {
  /*
    init
  */

  for (uint bit_value = 0; bit_value < 2; bit_value++) {
    // bitvector of size 1
    BitArray bitv{0};
    ClarkSelect<BitArray> clark(bitv, bit_value);
    clark.build();
    for (uint i = 0; i < bitv.size(); i++) {
      EXPECT_EQ(clark.select(i), linear_select(bitv, i, bit_value));
    }
    // bitv = BitArray{1};
    // clark = ClarkSelect<BitArray>(bitv, bit_value);
    // clark.build();
    // for (uint i = 0; i < bitv.size(); i++) {
    //   EXPECT_EQ(clark.select(i), linear_select(bitv, i, bit_value));
    // }

    uint iterations = 2;
    for (uint p = 0; p <= 100; p += 10) {
      run_clark_linearly(1000, bit_value, iterations, p);
    }
    for (uint p = 0; p <= 100; p += 25) {
      run_clark_linearly(5000, bit_value, iterations / 2, p);
    }
  }
}

}  // namespace test
}  // namespace lib
}  // namespace compact