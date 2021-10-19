#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "temporalgraph/common/graph/CAS.h"
#include "temporalgraph/common/graph/tests/TestUtils.h"

namespace compact {
namespace temporalgraph {
namespace test {

bool has_edge(uint u, uint v, uint start, uint end,
              const CAS::TemporalAdjacencyList& adj) {
  for (auto& e : adj[u]) {
    if (e.first != v) continue;
    uint l = e.second.first;
    uint r = e.second.second;
    if (end < l || r < start) continue;
    return true;
  }
  return false;
}

AbstractGraph::VertexContainer neighbours(const CAS::TemporalAdjacencyList& adj,
                                          uint u, uint start, uint end) {
  AbstractGraph::VertexContainer ans;
  for (auto& e : adj[u]) {
    uint v = e.first;
    uint tbegin = e.second.first;
    uint tend = e.second.second;
    if (tend < start || tbegin > end) continue;
    ans.push_back(v);
  }
  std::sort(ans.begin(), ans.end());
  return ans;
}

AbstractGraph::EdgeContainer aggregate(const CAS::TemporalAdjacencyList& adj,
                                       uint start, uint end) {
  AbstractGraph::EdgeContainer ans;
  for (uint u = 0; u < adj.size(); u++) {
    auto neigh = neighbours(adj, u, start, end);
    for (uint v : neigh) ans.push_back(AbstractGraph::Edge(u, v));
  }
  return ans;
}

bool equals(AbstractGraph::VertexContainer v1,
            AbstractGraph::VertexContainer v2) {
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  return v1 == v2;
}

bool equals(AbstractGraph::EdgeContainer v1, AbstractGraph::EdgeContainer v2) {
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  return v1 == v2;
}

// test expected functionalities of CAS data structure
TEST(CASTest, CAS_test) {
  /*
    init
  */
  LOG(INFO) << "CASTest init";
  uint V = 20, E = 100, T = 100, epochs = 10, graphs = 10;
  CAS graph;
  for (uint x = 0; x < graphs; x++) {
    GraphUtils::TemporalAdjacencyList adj =
        TestUtils::get_random_graph(V, E, T);
    CAS graph(adj);
    LOG(INFO) << "adj: " << GraphUtils::to_string(adj);
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
    LOG(INFO) << graph.to_string();
    EXPECT_EQ(graph.get_name(), std::string("CAS"));
  }
}

}  // namespace test
}  // namespace temporalgraph
}  // namespace compact