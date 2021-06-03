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
  WaveletTree tree;
  LOG(INFO) << tree.to_string() << " test!";
}

}  // namespace test
}  // namespace lib
}  // namespace compact