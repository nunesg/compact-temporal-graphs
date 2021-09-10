#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "temporalgraph/common/graph/AbstractGraph.h"
#include "temporalgraph/common/graph/AdjacencyList.h"
#include "temporalgraph/common/graph/tests/TestUtils.h"

namespace compact {
namespace temporalgraph {
namespace test {

// test expected functionalities of AdjacencyList data structure
TEST(AdjacencyListTest, adjacencyList_test) {
  /*
    init
  */
  LOG(INFO) << "AdjacencyListTest init";
  uint V = 20, E = 100, T = 100, epochs = 10, graphs = 10;
  AdjacencyList graph(V);
  for (uint x = 0; x < graphs; x++) {
    GraphUtils::TemporalAdjacencyList adj =
        TestUtils::get_random_graph(V, E, T);
    LOG(INFO) << "adj: " << GraphUtils::to_string(adj);
    graph.reset(adj);
    // LOG(INFO) << graph.to_string();

    for (uint i = 0; i < V; i++) {
      for (uint j = 0; j < V; j++) {
        for (uint k = 0; k < epochs; k++) {
          auto t = TestUtils::get_random_temporal_range(T + T / 2);
          // LOG(INFO) << "i: " << i << ", j: " << j << ", tbegin: " << t.first
          //           << ", tend: " << t.second;

          // test has_edge
          EXPECT_EQ(graph.has_edge(i, j, t.first, t.second),
                    GraphUtils::has_edge(adj, i, j, t.first, t.second));

          // test neighbours
          auto tmp = graph.neighbours(i, t.first, t.second);
          std::sort(tmp.begin(), tmp.end());
          EXPECT_EQ(tmp, GraphUtils::neighbours(adj, i, t.first, t.second));
          tmp = graph.neighbours(j, t.first, t.second);
          std::sort(tmp.begin(), tmp.end());
          EXPECT_EQ(tmp, GraphUtils::neighbours(adj, j, t.first, t.second));
        }
      }
    }
  }
  EXPECT_EQ(graph.get_name(), std::string("AdjacencyList"));

  LOG(INFO) << graph.to_string();
}

}  // namespace test
}  // namespace temporalgraph
}  // namespace compact