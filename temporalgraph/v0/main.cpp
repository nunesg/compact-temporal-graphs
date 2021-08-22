/*
  Test using standard Adjacency List to represent Temporal Graphs
*/

#include <bits/stdc++.h>

#include "temporalgraph/common/AdjacencyList.h"
#include "temporalgraph/common/Graph.h"
#include "temporalgraph/common/graph/GraphParser.h"
#include "temporalgraph/common/graph/GraphUtils.h"

using namespace compact::temporalgraph;

int main() {
  uint nVertices, nEdges;
  GraphParser::TemporalAdjacencyList adj;
  GraphParser::parseStdin(adj, nVertices, nEdges);

  Graph<AdjacencyList> g(nVertices, "MyAdjacencyListGraph");
  GraphParser::fillGraph(adj, g);

  std::cout << g.toString() << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}