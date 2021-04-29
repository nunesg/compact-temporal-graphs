#pragma once

#include <utility>
#include <vector>

namespace compact {
namespace temporalgraph {

class GraphUtils {
 public:
  using Vertex = int;
  using Edge = std::pair<Vertex, Vertex>;
  using VertexContainer = std::vector<Vertex>;
  using EdgeContainer = std::vector<Edge>;
};

}  // namespace temporalgraph
}  // namespace compact