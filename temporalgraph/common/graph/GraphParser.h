#pragma once

#include "glog/logging.h"
#include "temporalgraph/common/graph/CAS.h"
#include "temporalgraph/common/graph/EdgeLog.h"
#include "temporalgraph/common/graph/EveLog.h"
#include "temporalgraph/common/graph/GraphUtils.h"

namespace compact {
namespace temporalgraph {

class GraphParser {
 public:
  using TemporalNeighbourContainer = GraphUtils::TemporalNeighbourContainer;
  using TemporalAdjacencyList = GraphUtils::TemporalAdjacencyList;
  /*
    parse the input from stdin considering the following structure:

    first line: read two integers N and M, the number of vertices and edges,
    respectively next N lines:

    on the i-th line (0<=i<N) you'll read:
      1. the number Ni of events for the i-th vertex
      2. Ni triples v_j, tbegin_j, tend_j of integers separated by spaces
      indicating a temporal edge between vertices (i, v_j)

    Example:

    graph:
    0: {1, {4, 5}}, {2, {6, 7}}, {3, {8, 9}}
    1: {0, {4, 5}}
    2: {0, {6, 7}}
    3: {0, {8, 9}}

    input:
    4 5
    3 1 4 5 2 6 7 3 8 9
    1 0 4 5
    1 0 6 7
    1 0 8 9
  */
  static void parseStdin(TemporalAdjacencyList& adj, uint& n, uint& m,
                         uint& t) {
    scanf("%d %d %d", &n, &m, &t);
    uint from, to, st, en, neighbours;
    adj.resize(n);
    for (from = 0; from < n; from++) {
      scanf("%d", &neighbours);
      adj[from].clear();
      for (uint j = 0; j < neighbours; j++) {
        scanf("%d %d %d", &to, &st, &en);
        adj[from].push_back(GraphUtils::createTemporalNeighbour(to, st, en));
      }
    }
  }

  template <typename GraphType>
  static void fillGraph(const TemporalAdjacencyList& adj, GraphType& g) {
    uint from, to, st, en;
    for (from = 0; from < adj.size(); from++) {
      for (auto& neigh : adj[from]) {
        GraphUtils::extractTemporalNeighbour(neigh, to, st, en);
        g.addEdge(from, to, st, en);
      }
    }
  }

  static void fillEdgeLog(TemporalAdjacencyList& adj, EdgeLog& g) {
    for (uint u = 0; u < adj.size(); u++) {
      // LOG(INFO) << "set events for vertex " << u;
      g.set_events(u, adj[u]);
    }
  }

  static void fillEveLog(TemporalAdjacencyList& adj, EveLog& g) {
    GraphUtils::EdgeContainer events;
    for (uint u = 0; u < adj.size(); u++) {
      createEvelogEvents(adj[u], events);
      g.set_events(u, events);
    }
  }

  static void fillCAS(const TemporalAdjacencyList& adj, CAS& g) {
    g.reset(adj);
  }

 private:
  static void createEvelogEvents(const TemporalNeighbourContainer& neighbours,
                                 GraphUtils::EdgeContainer& ans) {
    ans.clear();
    for (auto& neigh : neighbours) {
      uint to, st, en;
      GraphUtils::extractTemporalNeighbour(neigh, to, st, en);
      ans.push_back({to, st});
      ans.push_back({to, en});
    }
  }
};

}  // namespace temporalgraph
}  // namespace compact
