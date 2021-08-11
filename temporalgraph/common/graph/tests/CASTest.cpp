#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "temporalgraph/common/graph/CAS.h"

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

  for (uint u = 0; u < 3; u++) {
    for (uint v = 0; v < 3; v++) {
      for (uint i = 0; i <= 35; i++) {
        for (uint j = i; j <= 35; j++) {
          // LOG(INFO) << "u: " << u << " v: " << v << " i: " << i << " j: " <<
          // j;
          EXPECT_EQ(graph.has_edge(u, v, i, j), has_edge(u, v, i, j, adj));
        }
      }
    }
  }
  LOG(INFO) << graph.to_string();
}

}  // namespace test
}  // namespace temporalgraph
}  // namespace compact