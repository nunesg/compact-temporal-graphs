#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "temporalgraph/common/graph/EveLog.h"
#include "temporalgraph/common/graph/GraphInterface.h"

namespace compact {
namespace temporalgraph {
namespace test {

// test basic construction of EveLog data structure
TEST(EveLogTest, constructionTest) {
  /*
    init
  */
  EveLog graph(5);
  EXPECT_EQ(5, graph.size());

  GraphInterface::EdgeContainer events = {{2, 0}, {3, 3}, {3, 5}, {2, 4}};
  graph.set_events(0, events);
}

}  // namespace test
}  // namespace temporalgraph
}  // namespace compact