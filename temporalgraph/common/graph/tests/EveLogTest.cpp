#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "temporalgraph/common/graph/AbstractGraph.h"
#include "temporalgraph/common/graph/EveLog.h"

namespace compact {
namespace temporalgraph {
namespace test {

// test expected functionalities of EveLog data structure
TEST(EveLogTest, evelog_test) {
  /*
    init
  */
  EveLog graph(5);
  EXPECT_EQ(5, graph.size());

  AbstractGraph::EdgeContainer events = {{2, 0}, {3, 3}, {3, 5}, {2, 4}};
  graph.set_events(0, events);
  graph.set_events(2, events);

  AbstractGraph::EdgeContainer edges = graph.aggregate(2, 4);
  AbstractGraph::EdgeContainer expectedEdges = {{0, 2}, {0, 3}, {2, 2}, {2, 3}};

  LOG(INFO) << "Aggregate result:";
  for (uint i = 0; i < edges.size(); i++) {
    LOG(INFO) << "(" << edges[i].first << "," << edges[i].second << ")";
  }

  EXPECT_EQ(edges, expectedEdges);

  // check has_edge function
  EXPECT_EQ(false, graph.has_edge(0, 3, 0, 2));
  EXPECT_EQ(false, graph.has_edge(0, 3, 6, 6));
  EXPECT_EQ(true, graph.has_edge(0, 3, 3, 3));
  EXPECT_EQ(true, graph.has_edge(0, 3, 5, 5));
  EXPECT_EQ(true, graph.has_edge(0, 3, 2, 4));
  EXPECT_EQ(false, graph.has_edge(2, 3, 0, 2));
  EXPECT_EQ(false, graph.has_edge(2, 3, 6, 6));
  EXPECT_EQ(true, graph.has_edge(2, 3, 3, 3));
  EXPECT_EQ(true, graph.has_edge(2, 3, 5, 5));
  EXPECT_EQ(true, graph.has_edge(2, 3, 2, 4));

  LOG(INFO) << graph.to_string();
}

}  // namespace test
}  // namespace temporalgraph
}  // namespace compact