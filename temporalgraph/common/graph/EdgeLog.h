#pragma once

#include <algorithm>

#include "glog/logging.h"
#include "lib/VariableSizeDenseArray.h"
#include "temporalgraph/common/graph/EdgeList.h"
#include "temporalgraph/common/graph/GraphInterface.h"

namespace compact {
namespace temporalgraph {

class EdgeLog : public GraphInterface {
  using GraphInterface::Edge;
  using GraphInterface::EdgeContainer;
  using GraphInterface::TemporalNeighbourContainer;
  using GraphInterface::Vertex;
  using GraphInterface::VertexContainer;

 public:
  using EventContainer = EdgeList;
  EdgeLog() {}

  EdgeLog(uint n) {
    this->n = n;
    adj = new EdgeList[n];
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

  // returns the graph's edges that were active on that time interval
  EdgeContainer aggregate(uint start, uint end) const override {
    EdgeContainer edges;
    VertexContainer tmpVertices;
    for (uint i = 0; i < n; i++) {
      tmpVertices.clear();
      tmpVertices = neighbours(i, start, end);

      for (uint j = 0; j < tmpVertices.size(); j++) {
        edges.push_back(Edge(i, tmpVertices[j]));
      }
    }
    return edges;
  }

  // consider each edge being a pair {v, t}, where v is the vertex number, and t
  // is the time of the event
  void set_events(Vertex u, const TemporalNeighbourContainer& events) {
    adj[u].set_events(events, n);
  }

  uint size() const { return n; }

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
  uint n;
  EdgeList* adj;

  void check_vertex(Vertex u) const {
    if (u >= n) {
      throw std::runtime_error("Vertex index is out of bounds.");
    }
  }
};

}  // namespace temporalgraph
}  // namespace compact
