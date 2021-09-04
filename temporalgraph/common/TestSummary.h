#pragma once

#include <string>

#include "temporalgraph/common/TimeCounter.h"

namespace compact {
namespace temporalgraph {
class TestSummary {
 public:
  TestSummary(uint V, uint E, uint T, const TimeCounter& edge_counter,
              const TimeCounter& neighbour_counter,
              const TimeCounter& aggregate_counter, const std::string& id) {
    this->V = V;
    this->E = E;
    this->T = T;
    this->build_time = 0;
    this->graph_rss = 0;
    this->edge_counter = edge_counter;
    this->neighbour_counter = neighbour_counter;
    this->aggregate_counter = aggregate_counter;
    this->id = id;
  }

  void set_build_time(uint t) { this->build_time = t; }

  void set_graph_rss(uint val) { this->graph_rss = val; }

  std::string to_string() {
    std::string str;
    str += "Graph type: " + id + "\n";
    str += "V: " + std::to_string(V) + "\n";
    str += "E: " + std::to_string(E) + "\n";
    str += "T: " + std::to_string(T) + "\n";
    str += "has_edge    (avg time ms): " +
           std::to_string(edge_counter.get_mean()) + "\n";
    str += "neighbours  (avg time ms): " +
           std::to_string(neighbour_counter.get_mean()) + "\n";
    str += "aggregate   (avg time ms): " +
           std::to_string(aggregate_counter.get_mean()) + "\n";
    str += "build time (ms): " + std::to_string(this->build_time) + "\n";
    str += "graph rss (kb): " + std::to_string(this->graph_rss) + "\n";
    return str;
  }

 private:
  uint V;
  uint E;
  uint T;
  uint build_time;
  uint graph_rss;
  TimeCounter edge_counter;
  TimeCounter neighbour_counter;
  TimeCounter aggregate_counter;
  std::string id;
};
}  // namespace temporalgraph
}  // namespace compact