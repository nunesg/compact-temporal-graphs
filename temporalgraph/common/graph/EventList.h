#pragma once

#include <algorithm>

#include "glog/logging.h"
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
  void set_events(EdgeContainer& events) {
    sz = events.size();

    // order events by time in ascending order
    std::sort(events.begin(), events.end(), [](const Edge& e1, const Edge& e2) {
      return e1.second < e2.second;
    });
    set_labels(events);
    set_timestamps(events);
  }

 private:
  uint sz;
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
    labels.reset(values);
  }
};

}  // namespace temporalgraph
}  // namespace compact
