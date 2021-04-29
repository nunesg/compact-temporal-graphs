#pragma once

#include <algorithm>

#include "glog/logging.h"
#include "lib/VariableSizeDenseArray.h"
#include "temporalgraph/common/graph/EventList.h"
#include "temporalgraph/common/graph/GraphInterface.h"

namespace compact {
namespace temporalgraph {

class EveLog : public GraphInterface {
  using GraphInterface::Edge;
  using GraphInterface::EdgeContainer;
  using GraphInterface::Vertex;
  using GraphInterface::VertexContainer;

 public:
  using EventContainer = EventList;
  EveLog() {}

  EveLog(uint n) {
    this->n = n;
    adj = new EventList[n];
  }

  // returns whether there is an edge (u, v) active during that time interval
  bool has_edge(uint u, uint v, int start, int end) const override {
    if (u >= n) {
      throw std::runtime_error("Vertex index is out of bounds.");
    }
    return adj[u].check_edge(v, start, end);
  }

  // returns neighbours of vertex u on that time interval
  VertexContainer neighbours(uint u, int start, int end) const override {
    // TODO
    return VertexContainer();
  }

  // returns the graph's edges that were active on that time interval
  EdgeContainer aggregate(int start, int end) const override {
    // TODO
    return EdgeContainer();
  }

  // consider each edge being a pair {v, t}, where v is the vertex number, and t
  // is the time of the event
  void set_events(uint u, EdgeContainer& events) { adj[u].set_events(events); }

  uint size() const { return n; }

 private:
  uint n;
  EventList* adj;
};

}  // namespace temporalgraph
}  // namespace compact
