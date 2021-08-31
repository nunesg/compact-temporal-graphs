/*
  Test using CAS to represent Temporal Graphs
*/

#include <bits/stdc++.h>

#include "gflags/gflags.h"
#include "temporalgraph/common/TestRunner.h"
#include "temporalgraph/common/TimeCounter.h"
#include "temporalgraph/common/graph/CAS.h"
#include "temporalgraph/common/graph/GraphParser.h"
#include "temporalgraph/common/graph/GraphUtils.h"

using namespace compact::temporalgraph;

DEFINE_string(test_flag, "Test Flag!", "Test flag message to print!");

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  LOG(INFO) << FLAGS_test_flag;

  uint nVertices, nEdges;
  GraphParser::TemporalAdjacencyList adj;
  GraphParser::parseStdin(adj, nVertices, nEdges);

  TimeCounter counter;
  counter.start();
  CAS g(adj);
  counter.stop();

  LOG(INFO) << TestRunner::run(g).to_string();

  // std::cout << g.to_string() << std::endl;

  std::cout << "Time to build (ms): " << counter.get_mean() << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  gflags::ShutDownCommandLineFlags();
  return 0;
}