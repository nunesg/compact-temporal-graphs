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

  /*
    returns whether the intervals i1=[x1, y1] i2=[x2, y2] overlap
    (endpoints included)
  */
  static bool intersects(const TimeInterval& i1, const TimeInterval& i2) {
    if (i1.first > i1.second || i2.first > i2.second) {
      throw std::runtime_error("Invalid intervals! start > end");
    }
    if (i1.first <= i2.first) {
      return i2.first <= i1.second;
    }
    return i1.first <= i2.second;
  }

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