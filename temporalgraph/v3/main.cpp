/*
  Test using EveLog to represent Temporal Graphs
*/

#include <bits/stdc++.h>

#include "temporalgraph/common/graph/EveLog.h"
#include "temporalgraph/common/graph/GraphParser.h"
#include "temporalgraph/common/graph/GraphUtils.h"

using namespace compact::temporalgraph;

int main() {
  uint nVertices, nEdges;
  GraphParser::TemporalAdjacencyList adj;
  GraphParser::parseStdin(adj, nVertices, nEdges);

  EveLog g(nVertices);
  // LOG(INFO) << "built evelog";
  GraphParser::fillEveLog(adj, g);
  // LOG(INFO) << "filled evelog";

  std::cout << g.to_string() << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}