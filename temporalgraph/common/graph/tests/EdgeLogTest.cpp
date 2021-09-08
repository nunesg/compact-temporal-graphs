#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "temporalgraph/common/graph/AbstractGraph.h"
#include "temporalgraph/common/graph/EdgeLog.h"
#include "temporalgraph/common/graph/GraphParser.h"
#include "temporalgraph/common/graph/tests/TestUtils.h"

namespace compact {
namespace temporalgraph {
namespace test {

// test expected functionalities of EdgeLog data structure
TEST(EdgeLogTest, edgeLog_test) {
  /*
    init
  */
  LOG(INFO) << "EdgeLogTest init";
  uint V = 20, E = 100, T = 100, epochs = 10, graphs = 10;
  EdgeLog graph(V);
  for (uint x = 0; x < graphs; x++) {
    GraphParser::TemporalAdjacencyList adj =
        TestUtils::get_random_graph(V, E, T);
    LOG(INFO) << "adj: " << TestUtils::to_string(adj);
    GraphParser::fillEdgeLog(adj, graph);
    // LOG(INFO) << graph.to_string();

    for (uint i = 0; i < V; i++) {
      for (uint j = 0; j < V; j++) {
        for (uint k = 0; k < epochs; k++) {
          auto t = TestUtils::get_random_temporal_range(T + T / 2);
          // LOG(INFO) << "i: " << i << ", j: " << j << ", tbegin: " << t.first
          //           << ", tend: " << t.second;

          // test has_edge
          EXPECT_EQ(graph.has_edge(i, j, t.first, t.second),
                    TestUtils::has_edge(adj, i, j, t.first, t.second));

          // test neighbours
          auto tmp = graph.neighbours(i, t.first, t.second);
          std::sort(tmp.begin(), tmp.end());
          EXPECT_EQ(tmp, TestUtils::neighbours(adj, i, t.first, t.second));
          tmp = graph.neighbours(j, t.first, t.second);
          std::sort(tmp.begin(), tmp.end());
          EXPECT_EQ(tmp, TestUtils::neighbours(adj, j, t.first, t.second));
        }
      }
    }
  }
  EXPECT_EQ(graph.get_name(), std::string("EdgeLog"));

  LOG(INFO) << graph.to_string();
}

// // test expected functionalities of EdgeLog data structure
// TEST(EdgeLogTest, edgelog_test) {
//   /*
//     init
//   */
//   // LOG(INFO) << "EdgeLogTest init";
//   EdgeLog graph(5);
//   EXPECT_EQ(5, graph.size());
//   AbstractGraph::TemporalNeighbourContainer events = {{2, {0, 4}}, {3, {3,
//   5}}}; graph.set_events(0, events); graph.set_events(2, events);
//   // LOG(INFO) << "EdgeLogTest events set";

//   AbstractGraph::EdgeContainer edges = graph.aggregate(2, 4);
//   // LOG(INFO) << "EdgeLogTest after aggregate";
//   AbstractGraph::EdgeContainer expectedEdges = {{0, 2}, {0, 3}, {2, 2}, {2,
//   3}};

//   LOG(INFO) << "Aggregate result:";
//   for (uint i = 0; i < edges.size(); i++) {
//     LOG(INFO) << "(" << edges[i].first << "," << edges[i].second << ")";
//   }

//   EXPECT_EQ(edges, expectedEdges);

//   // LOG(INFO) << "EdgeLogTest before has_edge tests";
//   // check has_edge function
//   EXPECT_EQ(false, graph.has_edge(0, 3, 0, 2));
//   EXPECT_EQ(false, graph.has_edge(0, 3, 6, 6));
//   EXPECT_EQ(true, graph.has_edge(0, 3, 3, 3));
//   EXPECT_EQ(true, graph.has_edge(0, 3, 5, 5));
//   EXPECT_EQ(true, graph.has_edge(0, 3, 2, 4));
//   EXPECT_EQ(false, graph.has_edge(2, 3, 0, 2));
//   EXPECT_EQ(false, graph.has_edge(2, 3, 6, 6));
//   EXPECT_EQ(true, graph.has_edge(2, 3, 3, 3));
//   EXPECT_EQ(true, graph.has_edge(2, 3, 5, 5));
//   EXPECT_EQ(true, graph.has_edge(2, 3, 2, 4));

//   EXPECT_EQ(graph.get_name(), std::string("EdgeLog"));

//   // LOG(INFO) << "EdgeLogTest after has_edge tests";
//   LOG(INFO) << graph.to_string();
// }

}  // namespace test
}  // namespace temporalgraph
}  // namespace compact