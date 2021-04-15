#pragma once

#include <iostream>

namespace compact {
namespace temporalgraph {

class Vertex {
 private:
  int id;

 public:
  Vertex() : id(0) {}

  Vertex(int id) : id(id) {}

  int getId() const { return id; }

  std::string toString() const { return "<vertex " + std::to_string(id) + ">"; }
};

}  // namespace temporalgraph
}  // namespace compact