#pragma once

#include <algorithm>

#include "glog/logging.h"
#include "lib/VariableSizeDenseArray.h"
#include "temporalgraph/common/graph/AbstractGraph.h"
#include "temporalgraph/common/graph/EventList.h"

namespace compact {
namespace temporalgraph {

class EveLog : public AbstractGraph {
  using AbstractGraph::Edge;
  using AbstractGraph::EdgeContainer;
  using AbstractGraph::Vertex;
  using AbstractGraph::VertexContainer;

 public:
  using EventContainer = EventList;
  EveLog() {}

  EveLog(uint n) {
    this->n = n;
    adj = new EventList[n]();
  }

  // returns whether there is an edge (u, v) active during that time interval
  bool has_edge(uint u, uint v, uint start, uint end) const override {
    check_vertex(u);
    check_vertex(v);
    return adj[u].check_edge(v, start, end);
  }

  // returns neighbours of vertex u on that time interval
  VertexContainer neighbours(uint u, uint start, uint end) const override {
    check_vertex(u);
    return adj[u].get_neighbours(start, end);
  }

  std::string get_name() const override { return "EveLog"; }

  uint measure_memory() const override {
    uint sum = 0;
    for (uint i = 0; i < n; i++) {
      sum += adj[i].measure_memory();
    }
    return sum;
  }

  // consider each edge being a pair {v, t}, where v is the vertex number, and t
  // is the time of the event
  void set_events(uint u, EdgeContainer& events) {
    adj[u].set_events(events, n);
  }

  std::string to_string() const {
    std::string line = "\nEveLog\n";
    line += "n = ";
    line += std::to_string(n) + "\n";
    for (uint i = 0; i < n; i++) {
      line += std::to_string(i) + ": " + adj[i].to_string() + "\n";
    }
    return line;
  }

 private:
  EventList* adj;

  void check_vertex(uint u) const {
    if (u >= n) {
      throw std::runtime_error("Vertex index is out of bounds.");
    }
  }
};

}  // namespace temporalgraph
}  // namespace compact
