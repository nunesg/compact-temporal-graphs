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

  virtual bool has_edge(uint u, uint v, int start, int end) const = 0;

  virtual VertexContainer neighbours(uint u, int start, int end) const = 0;

  virtual EdgeContainer aggregate(int start, int end) const = 0;
};

}  // namespace temporalgraph
}  // namespace compact