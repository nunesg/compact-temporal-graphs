#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <map>
#include <string>

#include "glog/logging.h"
#include "temporalgraph/common/graph/GraphParser.h"
#include "temporalgraph/common/graph/GraphUtils.h"

namespace compact {
namespace temporalgraph {
namespace test {

class TestUtils {
 public:
  static GraphParser::TemporalAdjacencyList get_random_graph(uint V, uint E,
                                                             uint T) {
    GraphParser::TemporalAdjacencyList adj(V);
    srand(time(NULL));
    GraphUtils::TemporalNeighbourContainer events;
    for (uint i = 0; i < E; i++) {
      uint u = get_random_vertex(V);
      uint v = get_random_vertex(V);
      auto t = get_random_temporal_range(T);

      events.push_back({t.first, {u, v}});
      events.push_back({t.second, {u, v}});
    }
    std::sort(events.begin(), events.end());

    std::map<std::pair<uint, uint>, int> m;

    for (auto e : events) {
      if (m.count(e.second) && m[e.second] > 0) {
        adj[e.second.first].push_back(
            {e.second.second, {m[e.second], e.first}});
        m[e.second] = -1;
      } else {
        m[e.second] = e.first;
      }
    }

    return adj;
  }

  static bool has_edge(GraphParser::TemporalAdjacencyList adj, uint u, uint v,
                       uint tbegin, uint tend) {
    for (auto& e : adj[u]) {
      if (e.first != v) continue;
      auto t = e.second;
      if (tend < t.first || tbegin > t.second) continue;
      return true;
    }
    return false;
  }

  static GraphUtils::VertexContainer neighbours(
      GraphParser::TemporalAdjacencyList adj, uint u, uint tbegin, uint tend) {
    GraphUtils::VertexContainer ans;
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

  static std::string to_string(GraphParser::TemporalAdjacencyList adj) {
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

  static uint get_random(uint l = 0, uint len = 10) {
    return (random() % len) + l;
  }

  static uint get_random_vertex(uint n) { return get_random(0, n); }

  // returns a pair (t0, t1) where t1 >= t0 and 0 <= t0, t1 < tlen
  static std::pair<uint, uint> get_random_temporal_range(uint tlen) {
    uint t0 = get_random(0, tlen);
    uint t1 = get_random(0, tlen);
    if (t1 < t0) std::swap(t0, t1);
    return {t0, t1};
  }
};

}  // namespace test
}  // namespace temporalgraph
}  // namespace compact