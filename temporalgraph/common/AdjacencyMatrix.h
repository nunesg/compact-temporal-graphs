#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "temporalgraph/common/Edge.h"
#include "temporalgraph/common/Utils.h"

namespace compact {
namespace temporalgraph {

template <typename ArrayType>
class AdjacencyMatrix {
 public:
  AdjacencyMatrix() {}

  AdjacencyMatrix(int size) {
    sz = size;
    initMatrix(adj);
    initMatrix(st);
    initMatrix(en);
  }

  void addEdge(const Edge& edge) {
    if (!isValid(edge)) {
      throw std::runtime_error("Invalid Edge: " + edge.toString());
    }
    adj[edge.getFrom().getId()].write(edge.getTo().getId(), 1);
  }

  std::string toString() const {
    std::string res;
    for (int i = 0; i < sz; i++) {
      std::vector<std::string> tokens;
      for (int j = 0; j < sz; j++) {
        tokens.push_back(std::to_string(adj[i][j]));
      }
      res += "[" + Utils::join(tokens.begin(), tokens.end(), ",") + "]\n";
    }
    return res;
  }

 private:
  ArrayType *adj, *st, *en;
  int sz;

  bool isValid(const Edge& edge) {
    return edge.getFrom().getId() >= 0 && edge.getFrom().getId() < sz &&
           edge.getTo().getId() >= 0 && edge.getTo().getId() < sz;
  }

  void initMatrix(ArrayType*& mat) {
    mat = new ArrayType[sz];
    for (int i = 0; i < sz; i++) {
      mat[i].assign(sz, 0);
    }
  }
};

}  // namespace temporalgraph
}  // namespace compact