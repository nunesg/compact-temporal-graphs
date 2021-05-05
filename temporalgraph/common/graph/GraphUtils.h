#pragma once

#include <utility>
#include <vector>

namespace compact {
namespace temporalgraph {

class GraphUtils {
 public:
  using IntType = uint;
  using Vertex = IntType;
  using Edge = std::pair<Vertex, Vertex>;
  using VertexContainer = std::vector<Vertex>;
  using EdgeContainer = std::vector<Edge>;
  using TimeInterval = std::pair<IntType, IntType>;
  using TimeIntervalContainer = std::vector<TimeInterval>;
  using TemporalNeighbour = std::pair<Vertex, TimeInterval>;
  using TemporalNeighbourContainer = std::vector<TemporalNeighbour>;
  using TemporalEdge = std::pair<Edge, TimeInterval>;
  using TemporalEdgeContainer = std::vector<TemporalEdge>;

  static std::string to_string(const Vertex& vtx) {
    return std::to_string(vtx);
  }

  static std::string to_string(const Edge& ed) {
    return std::string() + "(" + to_string(ed.first) + "," +
           to_string(ed.second) + ")";
  }
};

}  // namespace temporalgraph
}  // namespace compact