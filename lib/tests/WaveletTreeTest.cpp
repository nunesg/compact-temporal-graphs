#include "glog/logging.h"
#include "gtest/gtest.h"
#include "lib/WaveletTree.h"

namespace compact {
namespace lib {
namespace test {

// test WaveletTree
TEST(WaveletTreeTest, waveletTreeTest) {
  /*
    init
  */
  WaveletTree tree({1, 2, 3, 1, 2, 4});
  LOG(INFO) << tree.to_string() << " test!";

  EXPECT_EQ(tree.rank(4, 1), 2);
  EXPECT_EQ(tree.rank(2, 3), 0);
  EXPECT_EQ(tree.rank(8, 4), 1);
}

}  // namespace test
}  // namespace lib
}  // namespace compact