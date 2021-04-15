#include <bits/stdc++.h>

#include "temporalgraph/common/AdjacencyList.h"
#include "temporalgraph/common/Edge.h"
#include "temporalgraph/common/Graph.h"
#include "temporalgraph/common/Vertex.h"

using namespace compact::temporalgraph;

int main() {
  Vertex v1(1), v2(2);
  Edge edge(v1, v2, 0, 5);
  Graph g(3, "MyGraph");

  g.addEdge(edge);

  std::cout << g.toString() << std::endl;

  return 0;
}