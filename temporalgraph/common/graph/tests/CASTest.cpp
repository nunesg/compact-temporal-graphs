#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "temporalgraph/common/graph/CAS.h"

namespace compact {
namespace temporalgraph {
namespace test {

// test expected functionalities of CAS data structure
TEST(CASTest, cas_test) {
  /*
    init
  */

  /*
    0: {1, {20, 22}}, {2, {25, 28}}
    1: {2, {30, 32}}
    2:
  */
  CAS::TemporalAdjacencyList adj{
      {{1, {25, 28}}, {2, {20, 25}}}, {{2, {30, 32}}}, {}};
  CAS graph(adj);
  EXPECT_EQ(3, graph.size());
}

}  // namespace test
}  // namespace temporalgraph
}  // namespace compact