#pragma once

#include <bits/stdc++.h>

#include "temporalgraph/common/Edge.h"
#include "temporalgraph/common/Utils.h"

namespace compact {
namespace temporalgraph {

class AdjacencyList {
 private:
  std::vector<std::vector<Edge>> list;

  bool isValid(const Edge& edge) const {
    return (edge.getFrom().getId() >= 0 &&
            edge.getFrom().getId() < list.size()) &&
           (edge.getFrom().getId() >= 0 &&
            edge.getFrom().getId() < list.size());
  }

 public:
  AdjacencyList() {}

  AdjacencyList(int size) { list.resize(size); }

  void addEdge(const Edge& edge) {
    int u = edge.getFrom().getId();
    int v = edge.getTo().getId();
    if (!isValid(edge)) {
      printf("Invalid edge for adjacency matrix");
      return;
    }

    list[u].push_back(edge);
    list[v].push_back(edge.getReverse());
  }

  std::string toString() const {
    std::string result(""), line("");
    for (int i = 0; i < list.size(); i++) {
      line = "[" + std::to_string(i) + "]: ";
      std::vector<std::string> tokens;
      for (auto& e : list[i]) {
        tokens.push_back(e.toString());
      }
      line += Utils::join(tokens.begin(), tokens.end(), ", ");
      result += line + "\n";
    }
    return result;
  }
};

}  // namespace temporalgraph
}  // namespace compact