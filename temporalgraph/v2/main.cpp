/*
  Test using EdgeLog to represent Temporal Graphs
*/

#include <bits/stdc++.h>

#include "temporalgraph/common/graph/EdgeLog.h"
#include "temporalgraph/common/graph/GraphParser.h"
#include "temporalgraph/common/graph/GraphUtils.h"

using namespace compact::temporalgraph;

int main() {
  uint nVertices, nEdges;
  GraphParser::TemporalAdjacencyList adj;
  GraphParser::parseStdin(adj, nVertices, nEdges);

  // LOG(INFO) << "before EdgeLog";
  EdgeLog g(nVertices);
  // LOG(INFO) << "after EdgeLog";
  GraphParser::fillEdgeLog(adj, g);
  // LOG(INFO) << "after filling EdgeLog";

  std::cout << g.to_string() << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}