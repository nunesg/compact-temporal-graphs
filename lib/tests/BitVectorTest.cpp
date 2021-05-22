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
  BitVector bitv2{1, 1, 0};

  EXPECT_EQ(bitv.size(), 0);
  EXPECT_EQ(bitv2[0], 1);
  EXPECT_EQ(bitv2[1], 1);
  EXPECT_EQ(bitv2[2], 0);
}
}  // namespace test
}  // namespace lib
}  // namespace compact