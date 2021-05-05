#pragma once

#include <algorithm>
#include <unordered_map>

#include "glog/logging.h"
#include "lib/VariableSizeDenseArray.h"
#include "lib/utils/DeltaGapUtility.h"
#include "lib/utils/Utils.h"
#include "temporalgraph/common/graph/GraphUtils.h"

namespace compact {
namespace temporalgraph {

class EdgeList {
  using Edge = GraphUtils::Edge;
  using EdgeContainer = GraphUtils::EdgeContainer;
  using Vertex = GraphUtils::Vertex;
  using VertexContainer = GraphUtils::VertexContainer;
  using TemporalNeighbour = GraphUtils::TemporalNeighbour;
  using TemporalNeighbourContainer = GraphUtils::TemporalNeighbourContainer;
  using Array = lib::VariableSizeDenseArray;

 public:
  EdgeList() {}

  // each event is a triple {vertex, {start_t, end_t}}
  void set_events(TemporalNeighbourContainer& events) {
    sz = events.size();
    if (!sz) return;

    std::sort(events.begin(), events.end());

    build_intervals(events);
    build_labels(events);
    build_offsets(events);
  }

  bool check_edge(uint v, int start, int end) const {
    if (!sz) return false;
    // TODO
    return false;
  }

  /*
    Return the vertices that were active during some time on the interval
    [start, end], including the interval limits.
  */
  VertexContainer get_neighbours(uint start, uint end) const {
    VertexContainer neighbours;
    // TODO
    return neighbours;
  }

  uint size() const { return sz; }

  std::string to_string() const {
    // TODO
    return "";
  }

 private:
  uint sz;
  Array labels, intervals, offsets;

  /*
    Receive a sorted array of pairs {vertex, {start_t, end_t}} and builds the
    array with the list of intervals of each neighbour concatenated
  */
  void build_intervals(const TemporalNeighbourContainer& sorted_events) {
    std::vector<uint> interval_array;
    get_intervals(sorted_events, interval_array);
    intervals.reset(interval_array);
  }
  /*
    Receive a sorted array of pairs {vertex, {start_t, end_t}} and builds the
    array with the offsets of the linked lists for each neighbour
  */
  void build_labels(const TemporalNeighbourContainer& sorted_events) {
    VertexContainer labels_array;
    get_labels(sorted_events, labels_array);
    labels.reset(labels_array);
  }

  /*
    Receive a sorted array of pairs {vertex, {start_t, end_t}} and builds the
    array with the offsets of the linked lists for each neighbour
  */
  void build_offsets(const TemporalNeighbourContainer& sorted_events) {
    VertexContainer offsets_array;
    get_offsets(sorted_events, offsets_array);
    offsets.reset(offsets_array);
  }

  /*
    Receive a sorted array of pairs {vertex, {start_t, end_t}} and builds the
    array with the timestamps of the intervals
    {start_t, end_t, start_t,end_t...}
  */
  static void get_intervals(const TemporalNeighbourContainer& events,
                            std::vector<uint>& results) {
    results.clear();
    for (auto& e : events) {
      results.push_back(e.second.first);
      results.push_back(e.second.second);
    }
  }

  /*
    Receive a sorted array of pairs {vertex, {start_t, end_t}} and builds the
    array of distinct neighbours
  */
  static void get_labels(const TemporalNeighbourContainer& sorted_events,
                         VertexContainer& labels) {
    labels.clear();
    labels.push_back(sorted_events[0].first);
    for (uint i = 1; i < sorted_events.size(); i++) {
      if (sorted_events[i].first != sorted_events[i - 1].first) {
        labels.push_back(sorted_events[i].first);
      }
    }
  }

  /*
    Receive a sorted array of pairs {vertex, {start_t, end_t}} and labels
    Build the array of offsets (position in which that linked list start on the
    intervals array)
  */
  static void get_offsets(const TemporalNeighbourContainer& sorted_events,
                          VertexContainer& offsets) {
    offsets.clear();
    uint curr_label = 0;
    for (uint i = 0; i < sorted_events.size(); i++) {
      if (sorted_events[i].first == labels[curr_label]) {
        offsets.push_back(i << 1);  // each entry has 2 elements
        curr_label++;
      }
      if (curr_label == labels.size()) break;
    }
  }
};

}  // namespace temporalgraph
}  // namespace compact
