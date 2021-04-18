#pragma once

#include <bits/stdc++.h>

#include "temporalgraph/common/AdjacencyList.h"
#include "temporalgraph/common/Edge.h"
#include "temporalgraph/common/Utils.h"
#include "temporalgraph/common/Vertex.h"

namespace compact {
namespace temporalgraph {

class Graph {
 private:
  AdjacencyList adj;
  std::vector<Vertex> vertices;
  std::string id;
  int nVertices;

 public:
  Graph() : id("Empty graph"), adj() {}

  Graph(int n, const std::string& id_ = "Nameless Graph") {
    nVertices = n;
    for (auto i = 0; i < n; i++) {
      vertices.push_back(Vertex(i));
    }
    adj = AdjacencyList(n);
    id = id_;
  }

  void addEdge(const int from, const int to, const int start, const int end) {
    if (0 <= from && from < nVertices && 0 <= to && to < nVertices) {
      Edge edge(vertices[from], vertices[to], start, end);
      adj.addEdge(edge);
      adj.addEdge(edge.getReverse());
      return;
    }
    throw std::runtime_error("Vertices out of bound.");
  }

  std::string toString() const { return id + ":\n\n" + adj.toString(); }
};

}  // namespace temporalgraph
}  // namespace compact