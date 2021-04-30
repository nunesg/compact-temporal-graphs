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