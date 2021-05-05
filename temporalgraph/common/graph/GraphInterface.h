#pragma once

#include <utility>
#include <vector>

#include "temporalgraph/common/graph/GraphUtils.h"

namespace compact {
namespace temporalgraph {

class GraphInterface {
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

  virtual EdgeContainer aggregate(uint start, uint end) const = 0;
};

}  // namespace temporalgraph
}  // namespace compact