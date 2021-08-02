#pragma once

#include <algorithm>
#include <vector>

#include "glog/logging.h"
#include "lib/BitVector.h"
#include "lib/WaveletTree.h"
#include "temporalgraph/common/graph/GraphInterface.h"

namespace compact {
namespace temporalgraph {

class CAS : public GraphInterface {
  using GraphInterface::Edge;
  using GraphInterface::EdgeContainer;
  using GraphInterface::TemporalNeighbourContainer;
  using GraphInterface::Vertex;
  using GraphInterface::VertexContainer;

 public:
  using EventContainer = TemporalNeighbourContainer;
  using TemporalAdjacencyList = std::vector<EventContainer>;

  CAS() {}

  CAS(uint n) {
    n = n;
    std::vector<EventContainer> adj(n);
    build(adj);
  }

  CAS(TemporalAdjacencyList adj) {
    n = adj.size();
    build(adj);
  }

  // returns whether there is an edge (u, v) active during that time interval
  bool has_edge(uint u, uint v, uint start, uint end) const override {
    // TODO
    return false;
  }

  // returns neighbours of vertex u on that time interval
  VertexContainer neighbours(uint u, uint start, uint end) const override {
    // TODO
    return VertexContainer();
  }

  // returns the graph's edges that were active on that time interval
  EdgeContainer aggregate(uint start, uint end) const override {
    // TODO
    return EdgeContainer();
  }

  uint size() const { return n; }

  std::string to_string() const {
    // TODO
    std::string line = "\nTODO: CAS\n";
    return line;
  }

 private:
  uint n;
  uint offset;
  lib::BitVector bitv;
  lib::WaveletTree wavelet;

  void build(TemporalAdjacencyList adj) {
    // TODO
    n = adj.size();
    LOG(INFO) << "Build CAS, adj.size() = " << adj.size();
  }

  void check_vertex(uint u) const {
    if (u >= n) {
      throw std::runtime_error("Vertex index is out of bounds.");
    }
  }
};

}  // namespace temporalgraph
}  // namespace compact
