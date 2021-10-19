/*
  Test using EveLog to represent Temporal Graphs
*/

#include <bits/stdc++.h>

#include "gflags/gflags.h"
#include "temporalgraph/common/CommandLineFlags.h"
#include "temporalgraph/common/RssMonitor.h"
#include "temporalgraph/common/TestRunner.h"
#include "temporalgraph/common/TimeCounter.h"
#include "temporalgraph/common/graph/EveLog.h"
#include "temporalgraph/common/graph/GraphParser.h"
#include "temporalgraph/common/graph/GraphUtils.h"

using namespace compact::temporalgraph;

int main(int argc, char *argv[]) {
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  uint V, E, T;
  GraphUtils::TemporalAdjacencyList adj;
  GraphParser::parseStdin(adj, V, E, T);

  RssMonitor rss;

  TimeCounter build_time_counter;
  build_time_counter.start();
  EveLog g(V);
  // LOG(INFO) << "built evelog";
  GraphParser::fillEveLog(adj, g);
  // LOG(INFO) << "filled evelog";
  build_time_counter.stop();

  TestSummary summary =
      (new TestRunner())
          ->run(g, V, E, T, FLAGS_has_edge_epochs, FLAGS_neighbours_epochs,
                FLAGS_aggregate_epochs);

  rss.measure("after_tests");

  uint graph_size_kb = g.measure_memory() >> 10;

  summary.set_remaining_fields(build_time_counter.get_mean(), graph_size_kb,
                               rss.get_discounted("after_tests"),
                               FLAGS_has_edge_epochs, FLAGS_neighbours_epochs,
                               FLAGS_aggregate_epochs);
  // write results to file
  std::ofstream f;
  f.open(FLAGS_output_file);
  LOG(INFO) << summary.to_json();
  f << summary.to_json();
  f.close();

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  gflags::ShutDownCommandLineFlags();
  return 0;
}