#pragma once

#include <bits/stdc++.h>

#include "temporalgraph/common/AdjacencyList.h"
#include "temporalgraph/common/Edge.h"
#include "temporalgraph/common/Utils.h"

namespace compact {
namespace temporalgraph {

class Graph {
 private:
  AdjacencyList adj;
  std::string id;
  int nVertices;

 public:
  Graph() : id("Empty graph"), adj() {}

  Graph(int n, const std::string& id_ = "Nameless Graph") {
    nVertices = n;
    adj = AdjacencyList(n);
    id = id_;
  }

  void addEdge(const Edge& edge) { adj.addEdge(edge); }

  std::string toString() const { return id + ":\n\n" + adj.toString(); }
};

}  // namespace temporalgraph
}  // namespace compact