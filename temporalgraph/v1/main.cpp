#include <bits/stdc++.h>

#include "temporalgraph/common/AdjacencyList.h"
#include "temporalgraph/common/Edge.h"
#include "temporalgraph/common/Graph.h"
#include "temporalgraph/common/Vertex.h"

using namespace compact::temporalgraph;

int main() {
  // long long int arr[1000000];
  // for (int i = 0; i < 1000; i++) {
  //   arr[i] = 10;
  // }
  // printf("arr[999] = %lld\n", arr[999]);

  Vertex v1(1), v2(2);
  Edge edge(v1, v2, 0, 5);
  Graph g(3, "MyGraph");

  g.addEdge(edge);

  std::cout << g.toString() << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}