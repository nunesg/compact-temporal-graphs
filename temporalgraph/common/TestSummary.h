#pragma once

#include <stdio.h>

#include <string>

#include "temporalgraph/common/TimeCounter.h"

namespace compact {
namespace temporalgraph {
class TestSummary {
 public:
  /*
   graph_type  VARCHAR(50),
   V           INT,
   E           INT,
   T           INT,
   build_time_ms      FLOAT,
   has_edge_time_ms   FLOAT,
   neighbours_time_ms FLOAT,
   aggregate_time_ms  FLOAT,
   graph_size_kb       INT,
   max_rss_kb         INT,
   has_edge_epochs     INT,
   neighbours_epochs   INT,
   aggregate_epochs    INT
  */
  TestSummary(const std::string& graph_type, uint V, uint E, uint T,
              const TimeCounter& edge_counter,
              const TimeCounter& neighbour_counter,
              const TimeCounter& aggregate_counter) {
    this->graph_type = graph_type;
    this->V = V;
    this->E = E;
    this->T = T;
    this->edge_counter = edge_counter;
    this->neighbour_counter = neighbour_counter;
    this->aggregate_counter = aggregate_counter;
  }

  void set_remaining_fields(double build_time_ms, uint graph_size_kb,
                            uint max_rss_kb, uint has_edge_epochs,
                            uint neighbours_epochs, uint aggregate_epochs) {
    this->build_time_ms = build_time_ms;
    this->graph_size_kb = graph_size_kb;
    this->max_rss_kb = max_rss_kb;
    this->has_edge_epochs = has_edge_epochs;
    this->neighbours_epochs = neighbours_epochs;
    this->aggregate_epochs = aggregate_epochs;
  }

  //   std::string to_csv() {
  //     char str[1000];
  //     sprintf(str, "%s,%d,%d,%d,%lf,%lf,%lf,%lf,")
  //   }

  std::string to_json() {
    char str[1000];

    sprintf(str,
            "{\n"
            "      \"graph_type\": \"%s\",\n"
            "      \"V\": %d,\n"
            "      \"E\": %d,\n"
            "      \"T\": %d,\n"
            "      \"build_time_ms\": %.4lf,\n"
            "      \"has_edge_time_ms\": %.4lf,\n"
            "      \"neighbours_time_ms\": %.4lf,\n"
            "      \"aggregate_time_ms\": %.4lf,\n"
            "      \"graph_size_kb\": %d,\n"
            "      \"max_rss_kb\": %d,\n"
            "      \"has_edge_epochs\": %d,\n"
            "      \"neighbours_epochs\": %d,\n"
            "      \"aggregate_epochs\": %d\n"
            "}",
            graph_type.c_str(), V, E, T, build_time_ms, edge_counter.get_mean(),
            neighbour_counter.get_mean(), aggregate_counter.get_mean(),
            graph_size_kb, max_rss_kb, has_edge_epochs, neighbours_epochs,
            aggregate_epochs);
    return std::string(str);
  }

 private:
  std::string graph_type;
  uint V;
  uint E;
  uint T;
  uint graph_size_kb;
  uint max_rss_kb;
  uint has_edge_epochs;
  uint neighbours_epochs;
  uint aggregate_epochs;
  double build_time_ms;
  TimeCounter edge_counter;
  TimeCounter neighbour_counter;
  TimeCounter aggregate_counter;
};
}  // namespace temporalgraph
}  // namespace compact