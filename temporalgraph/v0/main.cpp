/*
  Test using standard Adjacency List to represent Temporal Graphs
*/

#include <bits/stdc++.h>

#include "temporalgraph/common/AdjacencyList.h"
#include "temporalgraph/common/Graph.h"
#include "temporalgraph/common/TimeCounter.h"
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
  Graph<AdjacencyList> g(nVertices, "MyAdjacencyListGraph");
  GraphParser::fillGraph(adj, g);
  counter.stop();

  // std::cout << g.toString() << std::endl;
  std::cout << "Time to build (ms): " << counter.get_mean() << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  return 0;
}