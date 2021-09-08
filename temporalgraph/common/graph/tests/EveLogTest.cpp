#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "temporalgraph/common/graph/AbstractGraph.h"
#include "temporalgraph/common/graph/EveLog.h"
#include "temporalgraph/common/graph/GraphParser.h"
#include "temporalgraph/common/graph/tests/TestUtils.h"

namespace compact {
namespace temporalgraph {
namespace test {

// test expected functionalities of EveLog data structure
TEST(EveLogTest, evelog_test) {
  /*
    init
  */
  LOG(INFO) << "EvelogTest init";
  uint V = 20, E = 100, T = 100, epochs = 10, graphs = 10;
  EveLog graph(V);
  for (uint x = 0; x < graphs; x++) {
    GraphParser::TemporalAdjacencyList adj =
        TestUtils::get_random_graph(V, E, T);
    LOG(INFO) << "adj: " << TestUtils::to_string(adj);
    GraphParser::fillEveLog(adj, graph);
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
  EXPECT_EQ(graph.get_name(), std::string("EveLog"));

  LOG(INFO) << graph.to_string();
}

}  // namespace test
}  // namespace temporalgraph
}  // namespace compact