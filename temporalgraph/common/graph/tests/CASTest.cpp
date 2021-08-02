#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "temporalgraph/common/graph/CAS.h"

namespace compact {
namespace temporalgraph {
namespace test {

// test expected functionalities of CAS data structure
TEST(CASTest, test) {
  /*
    init
  */
  CAS graph(5);
  EXPECT_EQ(5, graph.size());
}

}  // namespace test
}  // namespace temporalgraph
}  // namespace compact