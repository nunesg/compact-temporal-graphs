#pragma once

#include <utility>
#include <vector>

#include "temporalgraph/common/graph/GraphUtils.h"

namespace compact {
namespace temporalgraph {

class AbstractGraph {
 public:
  using Vertex = GraphUtils::Vertex;
  using Edge = GraphUtils::Edge;
  using VertexContainer = GraphUtils::VertexContainer;
  using EdgeContainer = GraphUtils::EdgeContainer;
  using TimeInterval = GraphUtils::TimeInterval;
  using TimeIntervalContainer = GraphUtils::TimeIntervalContainer;
  using TemporalNeighbour = GraphUtils::TemporalNeighbour;
  using TemporalNeighbourContainer = GraphUtils::TemporalNeighbourContainer;
  using TemporalEdge = GraphUtils::TemporalEdge;
  using TemporalEdgeContainer = GraphUtils::TemporalEdgeContainer;

  virtual bool has_edge(uint u, uint v, uint start, uint end) const = 0;

  virtual VertexContainer neighbours(uint u, uint start, uint end) const = 0;

  virtual std::string get_name() const = 0;

  // virtual uint size_edges() const = 0;

  uint size() const { return this->n; }

  // returns the graph's edges that were active on that time interval
  EdgeContainer aggregate(uint start, uint end) const {
    EdgeContainer edges;
    VertexContainer tmpVertices;
    for (uint i = 0; i < this->n; i++) {
      // LOG(INFO) << "aggregate: i = " << i << ", n = " << this->n;
      tmpVertices.clear();
      tmpVertices = this->neighbours(i, start, end);
      // LOG(INFO) << "result: " << tmpVertices.size() << " neighbours";

      for (uint j = 0; j < tmpVertices.size(); j++) {
        edges.push_back(Edge(i, tmpVertices[j]));
      }
    }
    return edges;
  }

 protected:
  uint n;
};

}  // namespace temporalgraph
}  // namespace compact