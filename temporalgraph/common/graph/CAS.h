#pragma once

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include "glog/logging.h"
#include "lib/BitVector.h"
#include "lib/WaveletTree.h"
#include "temporalgraph/common/Utils.h"
#include "temporalgraph/common/graph/AbstractGraph.h"
#include "temporalgraph/common/graph/GraphUtils.h"

namespace compact {
namespace temporalgraph {

class CAS : public AbstractGraph {
  using AbstractGraph::Edge;
  using AbstractGraph::EdgeContainer;
  using AbstractGraph::TemporalNeighbourContainer;
  using AbstractGraph::Vertex;
  using AbstractGraph::VertexContainer;

 public:
  using IntType = GraphUtils::IntType;
  // event = {neighbour, event_time}
  using Event = std::pair<Vertex, IntType>;
  using EventContainer = std::vector<Event>;
  using TemporalAdjacencyList = std::vector<TemporalNeighbourContainer>;

  CAS() {}

  CAS(uint n) {
    this->n = n;
    TemporalAdjacencyList adj(n);
    build(adj);
  }

  CAS(TemporalAdjacencyList adj) {
    this->n = adj.size();
    build(adj);
  }

  void reset(TemporalAdjacencyList adj) {
    this->n = adj.size();
    build(adj);
  }

  // returns whether there is an edge (u, v) active at some moment during that
  // time interval
  bool has_edge(uint u, uint v, uint start, uint end) const override {
    start += offset;
    end += offset;
    // LOG(INFO) << "start (+offset): " << start << ", end (+offset)" << end;
    // u's range on the wavelet tree is [i,j]
    uint i = bitv.rank(bitv.select(u, 1), 0);
    // LOG(INFO) << "select = " << bitv.select(u, 1) << ", i: " << i;
    uint j = bitv.rank(bitv.select(u + 1, 1), 0);

    if (j <= i) {
      return false;
    }
    j--;
    // LOG(INFO) << "select = " << bitv.select(u + 1, 1) << ", j: " << j;

    uint kbegin = wavelet.range_next_value_pos(i, j, start);
    // LOG(INFO) << "kbegin: " << kbegin;
    uint kend = wavelet.range_next_value_pos(i, j, end + 1);

    // LOG(INFO) << "i = " << i << " j = " << j << " kbegin = " << kbegin
    //           << " kend = " << kend << ", u = " << u << ", v = " << v;

    uint freq_at_beginning =
        (i != kbegin ? (wavelet.range_count(i, kbegin - 1, v)) : 0);
    uint freq_at_end = (i != kend ? (wavelet.range_count(i, kend - 1, v)) : 0);
    uint freq_on_interval = freq_at_end - freq_at_beginning;
    bool active_at_beginning = freq_at_beginning & 1;

    // LOG(INFO) << "fbegin: " << freq_at_beginning << ", fend: " <<
    // freq_at_end; LOG(INFO) << wavelet.to_string();

    return active_at_beginning || freq_on_interval > 0;
  }

  // returns neighbours of vertex u on the [start, end] time interval
  VertexContainer neighbours(uint u, uint start, uint end) const override {
    start += offset;
    end += offset;
    // LOG(INFO) << "start (+offset): " << start << ", end (+offset)" << end;
    // u's range on the wavelet tree is [i,j]
    uint i = bitv.rank(bitv.select(u, 1), 0);
    // LOG(INFO) << "select = " << bitv.select(u, 1) << ", i: " << i;
    uint j = bitv.rank(bitv.select(u + 1, 1), 0) - 1;
    // LOG(INFO) << "select = " << bitv.select(u + 1, 1) << ", j: " << j;

    uint kbegin = wavelet.range_next_value_pos(i, j, start);
    // LOG(INFO) << "kbegin: " << kbegin;
    uint kend = wavelet.range_next_value_pos(i, j, end + 1);

    // LOG(INFO) << "i = " << i << " j = " << j << " kbegin = " << kbegin
    //           << " kend = " << kend << ", u = " << u << ", v = " << v;

    std::unordered_map<uint, uint> report_before;
    std::unordered_map<uint, uint> report_interval;
    if (i != kbegin) wavelet.range_report(i, kbegin - 1, report_before);
    if (i != kend) wavelet.range_report(kbegin, kend - 1, report_interval);

    VertexContainer answer;
    std::unordered_map<uint, bool> chosen;
    for (auto& p : report_before) {
      uint v = p.first, f = p.second;
      if (v >= n) continue;
      if ((f & 1) ||
          report_interval.count(v)) {  // odd frequency before the time interval
        answer.push_back(v);
        chosen[v] = true;
      }
    }
    for (auto& p : report_interval) {
      uint v = p.first, f = p.second;
      if (v >= n) continue;
      if (!chosen.count(v) && f > 0) {  // vertex appears inside the interval
        answer.push_back(v);
      }
    }
    return answer;
  }

  std::string get_name() const override { return "CAS"; }

  std::string to_string() const {
    std::string line("CAS_");
    line += wavelet.to_string();
    return line;
  }

 private:
  uint offset;
  lib::BitVector bitv;
  lib::WaveletTree wavelet;

  void build(TemporalAdjacencyList adj) {
    n = adj.size();
    offset = n;  // value to be added to every timestamp
    // LOG(INFO) << "Build CAS, adj.size() = " << adj.size();
    EventContainer events;
    build_events(adj, events);
    // LOG(INFO) << "Events (not ordered):" << events;
    sort_events(adj, events);
    // LOG(INFO) << "Events (ordered):" << events;
    std::vector<uint> sizes, sequence;
    build_sequence(adj, events, sequence, sizes);
    // LOG(INFO) << "Sequence:" << sequence;
    build_bitvector(sizes);
    // LOG(INFO) << "BitVector:" << bitv.to_string();
    wavelet.reset(sequence);
  }

  // transforms the adjacency list in an event sequence
  // with timestamps increased by n to avoid conflict
  // between timestamps and vertices numbers
  void build_events(const TemporalAdjacencyList adj,
                    EventContainer& events) const {
    events.clear();
    for (uint u = 0; u < adj.size(); u++) {
      for (auto& neighbour : adj[u]) {
        uint v = neighbour.first;
        uint tbegin = neighbour.second.first;
        uint tend = neighbour.second.second;
        events.push_back({v, tbegin + offset});
        events.push_back({v, tend + offset});
      }
    }
  }

  // sort events of same origin vertex by timestamp
  static void sort_events(const TemporalAdjacencyList adj,
                          EventContainer& events) {
    auto start = events.begin();
    auto end = events.begin();
    for (uint i = 0; i < adj.size(); i++) {
      end = start + 2 * adj[i].size();
      std::sort(start, end, [](Event e1, Event e2) {
        // order increasingly by time
        return e1.second < e2.second;
      });
      start = end;
    }
  }

  // build wavelet_tree sequence as an array
  static void build_sequence(const TemporalAdjacencyList adj,
                             EventContainer& events, std::vector<uint>& seq,
                             std::vector<uint>& sizes) {
    seq.clear();
    sizes.clear();
    uint sz, e_idx = 0;
    for (uint u = 0; u < adj.size(); u++) {
      sz = 0;
      for (uint j = 0; j < 2 * adj[u].size(); j++) {
        if (j == 0 || events[e_idx].second != events[e_idx - 1].second) {
          seq.push_back(events[e_idx].second);
          sz++;
        }
        seq.push_back(events[e_idx].first);
        sz++;
        e_idx++;
      }
      sizes.push_back(sz);
    }
  }

  // build bitvector indicating where each vertex's sequence start on the
  // wavelet_tree
  void build_bitvector(const std::vector<uint>& sizes) {
    uint sum = sizes.size() + std::accumulate(sizes.begin(), sizes.end(), 0);
    // LOG(INFO) << "sizes = " << sizes << ", sum = " << sum;
    lib::BitArray bitarr(sum);
    for (uint bidx = 0, i = 0; i < sizes.size(); i++) {
      bitarr.write(bidx, 1);
      bidx += sizes[i] + 1;
    }
    bitv.reset(bitarr);
  }

  void check_vertex(uint u) const {
    if (u >= n) {
      throw std::runtime_error("Vertex index is out of bounds.");
    }
  }
};

}  // namespace temporalgraph
}  // namespace compact
