#pragma once

#include <stdlib.h>
#include <time.h>

#include <memory>

#include "glog/logging.h"
#include "temporalgraph/common/TestSummary.h"
#include "temporalgraph/common/TimeCounter.h"
#include "temporalgraph/common/graph/AbstractGraph.h"

namespace compact {
namespace temporalgraph {
class TestRunner {
 public:
  TestRunner() { srand(time(NULL)); }

  TestSummary run(const AbstractGraph& graph, uint V, uint E, uint T,
                  uint has_edge_epochs, uint neighbours_epochs,
                  uint aggregate_epochs) {
    TimeCounter edge_c, neigh_c, agg_c;
    LOG(INFO) << "has_edge:";
    for (uint i = 0; i < has_edge_epochs; i++) {
      edge_c.start();
      int u = get_random_vertex(V);
      int v = get_random_vertex(V);
      auto t = get_random_temporal_range(T);
      LOG(INFO) << "u: " << u << ", v: " << v << ", t0: " << t.first
                << ", t1: " << t.second;
      graph.has_edge(u, v, t.first,
                     t.second);  // dummy values. TODO: use random
      edge_c.stop();
    }
    LOG(INFO) << "neighbours:";
    for (uint i = 0; i < neighbours_epochs; i++) {
      neigh_c.start();
      int u = get_random_vertex(V);
      auto t = get_random_temporal_range(T);
      LOG(INFO) << "u: " << u << ", t0: " << t.first << ", t1: " << t.second;
      graph.neighbours(u, t.first, t.second);  // dummy values. TODO: use random
      neigh_c.stop();
    }
    LOG(INFO) << "aggregate:";
    for (uint i = 0; i < aggregate_epochs; i++) {
      agg_c.start();
      auto t = get_random_temporal_range(T);
      LOG(INFO) << "t0: " << t.first << ", t1: " << t.second;
      graph.aggregate(t.first, t.second);  // dummy values. TODO: use random
      agg_c.stop();
    }

    return TestSummary(V, E, T, edge_c, neigh_c, agg_c, graph.get_name());
  }

 private:
  uint get_random(uint l = 0, uint len = 10) const {
    return (random() % len) + l;
  }

  uint get_random_vertex(uint n) const { return get_random(0, n); }

  // returns a pair (t0, t1) where t1 >= t0 and 0 <= t0, t1 < tlen
  std::pair<uint, uint> get_random_temporal_range(uint tlen) const {
    uint t0 = get_random(0, tlen);
    uint t1 = get_random(0, tlen);
    if (t1 < t0) std::swap(t0, t1);
    return {t0, t1};
  }
};
}  // namespace temporalgraph
}  // namespace compact