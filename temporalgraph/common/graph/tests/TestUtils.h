#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <map>
#include <string>

#include "glog/logging.h"
#include "temporalgraph/common/graph/GraphUtils.h"

namespace compact {
namespace temporalgraph {
namespace test {

class TestUtils {
 public:
  static GraphUtils::TemporalAdjacencyList get_random_graph(uint V, uint E,
                                                            uint T) {
    GraphUtils::TemporalAdjacencyList adj(V);
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