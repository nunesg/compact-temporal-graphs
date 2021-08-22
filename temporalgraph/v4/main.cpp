/*
  Test using CAS to represent Temporal Graphs
*/

#include <bits/stdc++.h>

#include "temporalgraph/common/graph/CAS.h"
#include "temporalgraph/common/graph/GraphParser.h"
#include "temporalgraph/common/graph/GraphUtils.h"

using namespace compact::temporalgraph;

int main() {
  uint nVertices, nEdges;
  GraphParser::TemporalAdjacencyList adj;
  GraphParser::parseStdin(adj, nVertices, nEdges);

  CAS g(adj);

  std::cout << g.to_string() << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}