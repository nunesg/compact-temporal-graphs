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
  using TemporalAdjacencyList = std::vector<TemporalNeighbourContainer>;

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

  static void extractTemporalNeighbour(const TemporalNeighbour& neigh, uint& to,
                                       uint& t_begin, uint& t_end) {
    to = neigh.first;
    t_begin = neigh.second.first;
    t_end = neigh.second.second;
  }

  static TemporalNeighbour createTemporalNeighbour(uint to, uint t_begin,
                                                   uint t_end) {
    return {to, {t_begin, t_end}};
  }

  static bool has_edge(const TemporalAdjacencyList& adj, uint u, uint v,
                       uint tbegin, uint tend) {
    for (auto& e : adj[u]) {
      if (e.first != v) continue;
      auto t = e.second;
      if (tend < t.first || tbegin > t.second) continue;
      return true;
    }
    return false;
  }

  static VertexContainer neighbours(const TemporalAdjacencyList& adj, uint u,
                                    uint tbegin, uint tend) {
    VertexContainer ans;
    for (auto& e : adj[u]) {
      uint v = e.first;
      uint t0 = e.second.first;
      uint t1 = e.second.second;
      if (t1 < tbegin || t0 > tend) continue;
      ans.push_back(v);
    }
    std::sort(ans.begin(), ans.end());
    ans.resize(std::distance(ans.begin(), std::unique(ans.begin(), ans.end())));
    return ans;
  }

  static std::string to_string(TemporalAdjacencyList adj) {
    std::string str = "{";
    for (uint i = 0; i < adj.size(); i++) {
      if (i) str += ",";
      str += "{";
      for (uint j = 0; j < adj[i].size(); j++) {
        auto e = adj[i][j];
        if (j) str += ",";
        str += "{";
        str += std::to_string(e.first);
        str += ",";
        str += "{";
        str += std::to_string(e.second.first);
        str += ",";
        str += std::to_string(e.second.second);
        str += "}";
        str += "}";
      }
      str += "}";
    }
    str += "}";
    return str;
  }

  template <typename T = TemporalAdjacencyList>
  static uint measure_memory(T adj) {
    uint n = adj.size();
    uint sum = sizeof(T);
    for (uint i = 0; i < n; i++) {
      sum += sizeof(TemporalNeighbourContainer) +
             adj[i].size() * sizeof(TemporalNeighbour);
    }
    return sum;
  }
};

}  // namespace temporalgraph
}  // namespace compact