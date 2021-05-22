#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "lib/BitVector.h"

namespace compact {
namespace lib {
namespace test {

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
}  // namespace test
}  // namespace lib
}  // namespace compact