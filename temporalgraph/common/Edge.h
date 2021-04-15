#pragma once

#include <bits/stdc++.h>

#include "temporalgraph/common/Vertex.h"

namespace compact {
namespace temporalgraph {

class Edge {
 private:
  Vertex from, to;
  int start, end;

 public:
  Edge() : from(0), to(0), start(0), end(-1) {}

  Edge(const Vertex& from, const Vertex& to, int st, int en)
      : from(from), to(to), start(st), end(en) {}

  Vertex getFrom() const { return from; }

  Vertex getTo() const { return to; }

  int getStart() const { return start; }

  int getEnd() const { return end; }

  Edge getReverse() const { return Edge(to, from, start, end); }

  std::string toString() const {
    return "(" + from.toString() + "," + to.toString() + ")";
  }
};

}  // namespace temporalgraph
}  // namespace compact