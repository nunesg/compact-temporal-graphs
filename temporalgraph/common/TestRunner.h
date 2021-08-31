#pragma once

#include "glog/logging.h"
#include "temporalgraph/common/TestSummary.h"
#include "temporalgraph/common/TimeCounter.h"
#include "temporalgraph/common/graph/AbstractGraph.h"

namespace compact {
namespace temporalgraph {
class TestRunner {
 public:
  static TestSummary run(const AbstractGraph& graph, uint has_edge_epochs = 10,
                         uint neighbours_epochs = 10,
                         uint aggregate_epochs = 10) {
    TimeCounter edge_c, neigh_c, agg_c;
    LOG(INFO) << "has_edge:";
    for (uint i = 0; i < has_edge_epochs; i++) {
      edge_c.start();
      graph.has_edge(0, 1, 0, 10);  // dummy values. TODO: use random
      edge_c.stop();
    }
    LOG(INFO) << "neighbours:";
    for (uint i = 0; i < neighbours_epochs; i++) {
      neigh_c.start();
      graph.neighbours(0, 0, 10);  // dummy values. TODO: use random
      neigh_c.stop();
    }
    LOG(INFO) << "aggregate:";
    for (uint i = 0; i < aggregate_epochs; i++) {
      agg_c.start();
      graph.aggregate(0, 10);  // dummy values. TODO: use random
      agg_c.stop();
    }

    return TestSummary(graph.size(), 0, edge_c, neigh_c, agg_c,
                       graph.get_name());
  }
};
}  // namespace temporalgraph
}  // namespace compact