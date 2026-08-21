#include <bits/stdc++.h>
using namespace std;

#define pii pair<int, int>
#define pb push_back
#include "../../codebook/2_Graph/Bridge.cpp"
#include "oracle.hpp"

using lowlink_test::Edge;

[[noreturn]] static void fail_case(const string &where, int n,
                                   const vector<Edge> &edges,
                                   const string &reason) {
  cerr << "bridge_edge_cases failure in " << where << ": " << reason << '\n'
       << lowlink_test::describe(n, edges) << '\n';
  exit(1);
}

static void check_case(const string &where, int n, const vector<Edge> &edges) {
  const auto want_bridge = lowlink_test::bridge_edges(n, edges);
  ECC graph(n);
  for (auto [u, v] : edges) graph.add_edge(u, v);
  graph.solve();
  if (graph.is_bridge.size() != edges.size())
    fail_case(where, n, edges, "bridge vector size differs");
  for (int i = 0; i < (int)edges.size(); ++i)
    if ((bool)graph.is_bridge[i] != (bool)want_bridge[i])
      fail_case(where, n, edges, "bridge flag differs");

  if (lowlink_test::component_signature(graph.bln) !=
      lowlink_test::edge_connected_signature(n, edges, want_bridge))
    fail_case(where, n, edges, "2-edge-connected partition differs");
  set<int> ids(graph.bln.begin(), graph.bln.end());
  if (graph.necc != (int)ids.size())
    fail_case(where, n, edges, "component count differs");
  for (int i = 0; i < (int)edges.size(); ++i) {
    const auto [u, v] = edges[i];
    if ((graph.bln[u] == graph.bln[v]) != !(bool)graph.is_bridge[i])
      fail_case(where, n, edges, "edge endpoint/component invariant differs");
  }
}

static void add_copies(vector<Edge> &edges, int u, int v, int copies) {
  while (copies--) edges.push_back({u, v});
}

static vector<Edge> random_graph(mt19937 &rng, int n, int mode) {
  vector<Edge> edges;
  if (mode == 0) return edges;
  if (mode == 1) {
    for (int u = 0; u < n; ++u) add_copies(edges, u, u, rng() % 5);
    return edges;
  }
  if (mode == 2) {
    for (int u = 0; u + 1 < n; ++u) {
      add_copies(edges, u, u + 1, 1 + rng() % 4);
      add_copies(edges, u, u, rng() % 3);
    }
    return edges;
  }
  if (mode == 3) {
    for (int u = 0; u < n; ++u)
      add_copies(edges, u, (u + 1) % n, 1 + rng() % 3);
    for (int u = 0; u < n; ++u) add_copies(edges, u, u, rng() % 4);
    return edges;
  }
  if (mode == 4) {
    for (int u = 1; u < n; ++u) {
      add_copies(edges, 0, u, 1 + rng() % 3);
      add_copies(edges, u, u, rng() % 2);
    }
    return edges;
  }
  for (int u = 0; u < n; ++u) {
    add_copies(edges, u, u, rng() % 5);
    for (int v = u + 1; v < n; ++v)
      if (rng() % 100 < 48) add_copies(edges, u, v, 1 + rng() % 4);
  }
  return edges;
}

int main() {
  vector<pair<int, vector<Edge>>> fixed = {
      {1, {}},
      {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}},
      {7, {{0, 1}, {0, 1}, {1, 2}, {2, 3}, {2, 3}, {3, 4},
           {4, 5}, {5, 6}, {6, 6}, {0, 0}, {3, 3}}},
      {8, {{0, 1}, {1, 2}, {2, 0}, {2, 3}, {3, 4}, {4, 2},
           {4, 5}, {5, 6}, {6, 7}, {7, 5}, {1, 1}, {7, 7}}},
      {9, {{0, 1}, {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 3},
           {5, 6}, {6, 7}, {7, 8}, {8, 6}, {2, 2}, {8, 8}}},
  };

  mt19937 rng(0xB21D2026);
  long long cases = 0;
  for (auto graph : fixed) {
    check_case("fixed", graph.first, graph.second);
    ++cases;
    for (int rep = 0; rep < 32; ++rep) {
      shuffle(graph.second.begin(), graph.second.end(), rng);
      check_case("fixed permutation", graph.first, graph.second);
      ++cases;
    }
  }
  for (int tc = 0; tc < 3500; ++tc) {
    const int n = 1 + rng() % 10;
    auto edges = random_graph(rng, n, tc % 6);
    const int repetitions = tc % 7 == 0 ? 2 : 1;
    for (int rep = 0; rep < repetitions; ++rep) {
      shuffle(edges.begin(), edges.end(), rng);
      check_case("seeded random", n, edges);
      ++cases;
    }
  }
  cout << "Bridge edge cases PASS " << cases
       << " targeted/permuted multigraphs\n";
}
