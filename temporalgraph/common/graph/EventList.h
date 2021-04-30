#pragma once

#include <algorithm>

#include "glog/logging.h"
#include "lib/FixedSizeArray.h"
#include "lib/Utils.h"
#include "lib/VariableSizeDenseArray.h"
#include "temporalgraph/common/graph/GraphUtils.h"

namespace compact {
namespace temporalgraph {

class EventList {
  using Edge = GraphUtils::Edge;
  using EdgeContainer = GraphUtils::EdgeContainer;
  using Vertex = GraphUtils::Vertex;
  using VertexContainer = GraphUtils::VertexContainer;
  using Array = lib::VariableSizeDenseArray;

 public:
  EventList() {}

  // consider each edge being a pair {v, t}, where v is the vertex number, and t
  // is the time of the event
  void set_events(EdgeContainer& events, uint n) {
    sz = events.size();
    this->n = n;

    // order events by time in ascending order
    std::sort(events.begin(), events.end(), [](const Edge& e1, const Edge& e2) {
      return e1.second < e2.second;
    });
    set_labels(events);
    set_timestamps(events);
  }

  bool check_edge(uint v, int start, int end) const {
    if (timestamps.size() == 0) return false;
    uint tbegin = lib::Utils::lower_bound(timestamps, 0,
                                          (int)timestamps.size() - 1, start);
    uint tend =
        lib::Utils::upper_bound(timestamps, 0, (int)timestamps.size() - 1, end);
    uint fbegin = count_label(v, tbegin);
    uint fend = count_label(v, tend);
    return (fbegin % 2) || fend > fbegin;
  }

  /*
    Return the vertices that were active during some time on the interval
    [start, end], including the interval limits.
  */
  VertexContainer get_neighbours(uint start, uint end) const {
    if (sz == 0) return VertexContainer();

    lib::FixedSizeArray<1> activeElements;
    activeElements.assign(n, 0);
    // check which elements were active before the interval start
    uint i;
    for (i = 0; i < sz; i++) {
      uint t = timestamps[i];
      uint vtx = labels[i];
      if (t >= start) {
        break;
      }
      activeElements.write(vtx, 1 - activeElements[vtx]);
    }

    // add the active elements to the answer
    VertexContainer neighbours;
    for (uint j = 0; j < sz; j++) {
      uint t = timestamps[j];
      uint vtx = labels[j];
      if (t >= start) break;
      if (activeElements[vtx]) {
        neighbours.push_back(vtx);
      }
    }

    // add the vertices that became active inside the interval
    for (i = 0; i < sz; i++) {
      uint t = timestamps[i];
      uint vtx = labels[i];
      if (t > end) break;

      if (!activeElements[vtx]) {
        neighbours.push_back(vtx);
      }
    }

    neighbours.resize(std::distance(
        neighbours.begin(), std::unique(neighbours.begin(), neighbours.end())));
    return neighbours;
  }

  uint size() const { return sz; }

 private:
  uint sz;
  uint n;
  Array labels, timestamps;

  void set_labels(EdgeContainer& events) {
    std::vector<uint> values;
    for (uint i = 0; i < sz; i++) {
      values.push_back(events[i].first);
    }
    labels.reset(values);
  }

  void set_timestamps(EdgeContainer& events) {
    std::vector<uint> values;
    for (uint i = 0; i < sz; i++) {
      values.push_back(events[i].second);
    }
    timestamps.reset(values);
  }

  uint count_label(uint label, uint index) const {
    uint count = 0;
    for (uint i = 0; i < index; i++) {
      count += labels[i] == label;
    }
    return count;
  }
};

}  // namespace temporalgraph
}  // namespace compact
