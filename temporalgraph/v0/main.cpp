#include <bits/stdc++.h>

#include "temporalgraph/common/AdjacencyList.h"
#include "temporalgraph/common/Edge.h"
#include "temporalgraph/common/Graph.h"
#include "temporalgraph/common/Vertex.h"

using namespace compact::temporalgraph;

int main() {
  int nVertices, nEdges;
  std::cin >> nVertices >> nEdges;
  Graph g(nVertices, "MyGraph");

  int from, to, st, en;
  while (scanf("%d %d %d %d", &from, &to, &st, &en) != EOF) {
    g.addEdge(from, to, st, en);
  }

  std::cout << g.toString() << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}