#pragma once

#include "glog/logging.h"
#include "temporalgraph/common/graph/AbstractGraph.h"
#include "temporalgraph/common/graph/GraphUtils.h"

namespace compact {
namespace temporalgraph {

class AdjacencyList : public AbstractGraph {
  using AbstractGraph::Edge;
  using AbstractGraph::EdgeContainer;
  using AbstractGraph::Vertex;
  using AbstractGraph::VertexContainer;
  using TemporalAdjacencyList = GraphUtils::TemporalAdjacencyList;

 public:
  AdjacencyList() {}

  AdjacencyList(uint n) { this->n = n; }

  AdjacencyList(TemporalAdjacencyList adj) { reset(adj); }

  void reset(TemporalAdjacencyList adj) {
    this->n = adj.size();
    this->adj = adj;
  }

  // returns whether there is an edge (u, v) active during that time interval
  bool has_edge(uint u, uint v, uint start, uint end) const override {
    check_vertex(u);
    check_vertex(v);
    return GraphUtils::has_edge(this->adj, u, v, start, end);
  }

  // returns neighbours of vertex u on that time interval
  VertexContainer neighbours(uint u, uint start, uint end) const override {
    check_vertex(u);
    return GraphUtils::neighbours(this->adj, u, start, end);
  }

  std::string get_name() const override { return "AdjacencyList"; }

  uint measure_memory() const override {
    return GraphUtils::measure_memory(adj);
  }

  std::string to_string() const {
    std::string line = "\nAdjacencyList\n";
    return line + GraphUtils::to_string(this->adj);
  }

 private:
  TemporalAdjacencyList adj;

  void check_vertex(uint u) const {
    if (u >= n) {
      throw std::runtime_error("Vertex index is out of bounds.");
    }
  }
};

}  // namespace temporalgraph
}  // namespace compact
