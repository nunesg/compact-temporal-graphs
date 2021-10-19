#pragma once

#include <algorithm>

#include "glog/logging.h"
#include "lib/VariableSizeDenseArray.h"
#include "temporalgraph/common/graph/AbstractGraph.h"
#include "temporalgraph/common/graph/EdgeList.h"

namespace compact {
namespace temporalgraph {

class EdgeLog : public AbstractGraph {
  using AbstractGraph::Edge;
  using AbstractGraph::EdgeContainer;
  using AbstractGraph::TemporalNeighbourContainer;
  using AbstractGraph::Vertex;
  using AbstractGraph::VertexContainer;

 public:
  using EventContainer = EdgeList;
  EdgeLog() {}

  EdgeLog(uint n) {
    this->n = n;
    adj = new EdgeList[n]();
  }

  // returns whether there is an edge (u, v) active during that time interval
  bool has_edge(Vertex u, Vertex v, uint start, uint end) const override {
    check_vertex(u);
    check_vertex(v);
    return adj[u].check_edge(v, start, end);
  }

  // returns neighbours of vertex u on that time interval
  VertexContainer neighbours(Vertex u, uint start, uint end) const override {
    check_vertex(u);
    return adj[u].get_neighbours(start, end);
  }

  std::string get_name() const override { return "EdgeLog"; }

  uint measure_memory() const override {
    uint sum = 0;
    for (uint i = 0; i < n; i++) {
      sum += adj[i].measure_memory();
    }
    return sum;
  }

  // each event is a triple {vertex, {start_t, end_t}}
  void set_events(Vertex u, TemporalNeighbourContainer& events) {
    adj[u].set_events(events);
  }

  std::string to_string() const {
    std::string line = "\nEdgeLog\n";
    line += "n = ";
    line += std::to_string(n) + "\n";
    for (uint i = 0; i < n; i++) {
      line += std::to_string(i) + ": " + adj[i].to_string() + "\n";
    }
    return line;
  }

 private:
  EdgeList* adj;

  void check_vertex(Vertex u) const {
    if (u >= n) {
      throw std::runtime_error("Vertex index is out of bounds.");
    }
  }
};

}  // namespace temporalgraph
}  // namespace compact
