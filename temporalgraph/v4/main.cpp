/*
  Test using CAS to represent Temporal Graphs
*/

#include <bits/stdc++.h>
#include <sys/resource.h>

#include "gflags/gflags.h"
#include "temporalgraph/common/TestRunner.h"
#include "temporalgraph/common/TimeCounter.h"
#include "temporalgraph/common/graph/CAS.h"
#include "temporalgraph/common/graph/GraphParser.h"
#include "temporalgraph/common/graph/GraphUtils.h"

using namespace compact::temporalgraph;

DEFINE_int32(has_edge_epochs, 10,
             "Number of times to run the has_edge operation");
DEFINE_int32(neighbours_epochs, 10,
             "Number of times to run the neighbours operation");
DEFINE_int32(aggregate_epochs, 10,
             "Number of times to run the aggregate operation");

int main(int argc, char *argv[]) {
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  uint V, E, T;
  GraphParser::TemporalAdjacencyList adj;
  GraphParser::parseStdin(adj, V, E, T);

  struct rusage rusage_before;
  getrusage(RUSAGE_SELF, &rusage_before);

  TimeCounter counter;
  counter.start();
  CAS g(adj);
  counter.stop();

  struct rusage rusage_after;
  getrusage(RUSAGE_SELF, &rusage_after);

  TestSummary summary =
      (new TestRunner())
          ->run(g, V, E, T, FLAGS_has_edge_epochs, FLAGS_neighbours_epochs,
                FLAGS_aggregate_epochs);
  summary.set_build_time(counter.get_mean());
  summary.set_graph_rss(rusage_after.ru_maxrss - rusage_before.ru_maxrss);
  LOG(INFO) << summary.to_string();

  // std::cout << g.to_string() << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  gflags::ShutDownCommandLineFlags();
  return 0;
}