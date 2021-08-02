#include <bits/stdc++.h>

#include "temporalgraph/common/AdjacencyList.h"
#include "temporalgraph/common/Edge.h"
#include "temporalgraph/common/Graph.h"
#include "temporalgraph/common/Vertex.h"

using namespace compact::temporalgraph;

int main() {
  int nVertices, nEdges;
  std::cin >> nVertices >> nEdges;
  Graph<AdjacencyList> g(nVertices, "MyGraph");

  int from, to, st, en, neighbours;
  for (from = 0; from < nVertices; from++) {
    scanf("%d", &neighbours);
    for (int j = 0; j < neighbours; j++) {
      scanf("%d %d %d", &to, &st, &en);
      g.addEdge(from, to, st, en);
    }
  }

  // std::cout << g.toString() << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}