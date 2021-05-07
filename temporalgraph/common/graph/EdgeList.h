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
  using TimeInterval = GraphUtils::TimeInterval;
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
    build_offsets(events, labels);
  }

  bool check_edge(uint v, int start, int end) const {
    if (!sz) return false;
    auto labels = labels_dgap.decode_array(this->labels);
    auto intervals = intervals_dgap.decode_array(this->intervals);
    auto offsets = offsets_dgap.decode_array(this->offsets);

    uint v_index = get_label_index(v, labels);
    if (v_index == labels.size()) return false;

    uint l_offset = offsets[v_index];
    uint r_offset = (v_index + 1) < offsets.size() ? offsets[v_index + 1] - 1
                                                   : (int)intervals.size() - 1;

    TimeInterval t{start, end};
    for (uint i = l_offset; i < r_offset; i += 2) {
      if (GraphUtils::intersects(t, {intervals[i], intervals[i + 1]})) {
        return true;
      }
    }
    return false;
  }

  /*
    Return the vertices that were active during some time on the interval
    [start, end], including the interval limits.
  */
  VertexContainer get_neighbours(uint start, uint end) const {
    VertexContainer neighbours;
    auto labels = labels_dgap.decode_array(this->labels);
    for (uint i = 0; i < labels.size(); i++) {
      if (check_edge(labels[i], start, end)) {
        neighbours.push_back(labels[i]);
      }
    }
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
  lib::DeltaGapUtility labels_dgap, intervals_dgap, offsets_dgap;

  static uint get_label_index(uint label, const std::vector<uint>& labels) {
    auto idx = lib::Utils::lower_bound(labels, 0, labels.size(), label);
    if (idx == labels.size() || labels[idx] != label) {
      return labels.size();
    }
    return idx;
  }

  /*
    Receive a sorted array of pairs {vertex, {start_t, end_t}} and builds the
    array with the list of intervals of each neighbour concatenated
  */
  void build_intervals(const TemporalNeighbourContainer& sorted_events) {
    std::vector<uint> interval_array;
    get_intervals_from_events(sorted_events, interval_array);
    intervals.reset(intervals_dgap.get_array_code(interval_array));
  }
  /*
    Receive a sorted array of pairs {vertex, {start_t, end_t}} and builds the
    array with the offsets of the linked lists for each neighbour
  */
  void build_labels(const TemporalNeighbourContainer& sorted_events) {
    VertexContainer labels_array;
    get_labels_from_events(sorted_events, labels_array);
    labels.reset(labels_dgap.get_array_code(labels_array));
  }

  /*
    Receive a sorted array of pairs {vertex, {start_t, end_t}} and builds the
    array with the offsets of the linked lists for each neighbour
  */
  template <typename ArrayType>
  void build_offsets(const TemporalNeighbourContainer& sorted_events,
                     const ArrayType& labels) {
    VertexContainer offsets_array;
    get_offsets_from_events(sorted_events, labels, offsets_array);
    offsets.reset(offsets_dgap.get_array_code(offsets_array));
  }

  /*
    Receive a sorted array of pairs {vertex, {start_t, end_t}} and builds the
    array with the timestamps of the intervals
    {start_t, end_t, start_t,end_t...}
  */
  static void get_intervals_from_events(
      const TemporalNeighbourContainer& events, std::vector<uint>& results) {
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
  static void get_labels_from_events(
      const TemporalNeighbourContainer& sorted_events,
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
  template <typename ArrayType>
  static void get_offsets_from_events(
      const TemporalNeighbourContainer& sorted_events, const ArrayType& labels,
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
