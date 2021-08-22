/*
  Test using standard Adjacency Matrix to represent Temporal Graphs
*/

#include <bits/stdc++.h>

#include "lib/FixedSizeArray.h"
#include "temporalgraph/common/AdjacencyMatrix.h"
#include "temporalgraph/common/Graph.h"
#include "temporalgraph/common/graph/GraphParser.h"
#include "temporalgraph/common/graph/GraphUtils.h"

using namespace compact;
using namespace compact::temporalgraph;

int main() {
  uint nVertices, nEdges;
  GraphParser::TemporalAdjacencyList adj;
  GraphParser::parseStdin(adj, nVertices, nEdges);

  Graph<AdjacencyMatrix<lib::FixedSizeArray>> g(nVertices,
                                                "MyAdjacencyMatrixGraph");
  GraphParser::fillGraph(adj, g);

  std::cout << g.toString() << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}