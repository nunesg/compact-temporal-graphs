/*
  Test using EveLog to represent Temporal Graphs
*/

#include <bits/stdc++.h>

#include "temporalgraph/common/TimeCounter.h"
#include "temporalgraph/common/graph/EveLog.h"
#include "temporalgraph/common/graph/GraphParser.h"
#include "temporalgraph/common/graph/GraphUtils.h"

using namespace compact::temporalgraph;

int main() {
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  uint nVertices, nEdges;
  GraphParser::TemporalAdjacencyList adj;
  GraphParser::parseStdin(adj, nVertices, nEdges);

  TimeCounter counter;
  counter.start();
  EveLog g(nVertices);
  // LOG(INFO) << "built evelog";
  GraphParser::fillEveLog(adj, g);
  // LOG(INFO) << "filled evelog";
  counter.stop();

  // std::cout << g.to_string() << std::endl;
  std::cout << "Time to build (ms): " << counter.get_mean() << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  return 0;
}