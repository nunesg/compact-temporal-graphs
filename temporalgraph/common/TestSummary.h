#pragma once

#include <string>

#include "temporalgraph/common/TimeCounter.h"

namespace compact {
namespace temporalgraph {
class TestSummary {
 public:
  TestSummary(uint V, uint E, const TimeCounter& edge_counter,
              const TimeCounter& neighbour_counter,
              const TimeCounter& aggregate_counter, const std::string& id) {
    this->V = V;
    this->E = E;
    this->edge_counter = edge_counter;
    this->neighbour_counter = neighbour_counter;
    this->aggregate_counter = aggregate_counter;
    this->id = id;
  }

  std::string to_string() {
    std::string str;
    str += "Graph type: " + id + "\n";
    str += "V: " + std::to_string(V) + "\n";
    str += "E: " + std::to_string(E) + "\n";
    str +=
        "has_edge    (avg time): " + std::to_string(edge_counter.get_mean()) +
        "\n";
    str += "neighbours  (avg time): " +
           std::to_string(neighbour_counter.get_mean()) + "\n";
    str += "aggregate   (avg time): " +
           std::to_string(aggregate_counter.get_mean()) + "\n";
    return str;
  }

 private:
  uint V;
  uint E;
  TimeCounter edge_counter;
  TimeCounter neighbour_counter;
  TimeCounter aggregate_counter;
  std::string id;
};
}  // namespace temporalgraph
}  // namespace compact