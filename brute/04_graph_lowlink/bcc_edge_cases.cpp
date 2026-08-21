#include <bits/stdc++.h>
using namespace std;

#define pb push_back
#include "../../codebook/2_Graph/BCC_Vertex.cpp"
#include "oracle.hpp"

using lowlink_test::Edge;

[[noreturn]] static void fail_case(const string &where, int n,
                                   const vector<Edge> &edges,
                                   const string &reason) {
  cerr << "bcc_edge_cases failure in " << where << ": " << reason << '\n'
       << lowlink_test::describe(n, edges) << '\n';
  exit(1);
}

static void check_case(const string &where, int n, const vector<Edge> &edges) {
  BCC graph(n);
  for (auto [u, v] : edges) graph.add_edge(u, v);
  graph.solve();

  if (lowlink_test::canonical_blocks(graph.bcc) !=
      lowlink_test::vertex_blocks(n, edges))
    fail_case(where, n, edges, "vertex blocks differ");
  const auto want_ap = lowlink_test::articulation_points(n, edges);
  for (int u = 0; u < n; ++u)
    if ((bool)graph.is_ap[u] != (bool)want_ap[u])
      fail_case(where, n, edges, "articulation point differs");

  const int base = graph.bcc.size();
  graph.block_cut_tree();
  const int ap_count = count(want_ap.begin(), want_ap.end(), true);
  if ((int)graph.nG.size() != base + ap_count)
    fail_case(where, n, edges, "block-cut tree node count differs");

  vector<int> ap_node(n, -1);
  int next = base;
  for (int u = 0; u < n; ++u) if (want_ap[u]) ap_node[u] = next++;
  vector<pair<int, int>> want_tree, got_tree;
  for (int i = 0; i < base; ++i)
    for (int u : graph.bcc[i]) if (want_ap[u]) {
      want_tree.push_back({i, ap_node[u]});
      want_tree.push_back({ap_node[u], i});
    }
  for (int u = 0; u < (int)graph.nG.size(); ++u)
    for (int v : graph.nG[u]) got_tree.push_back({u, v});
  sort(want_tree.begin(), want_tree.end());
  sort(got_tree.begin(), got_tree.end());
  if (got_tree != want_tree)
    fail_case(where, n, edges, "block-cut tree edges differ");
}

static void add_copies(vector<Edge> &edges, int u, int v, int copies) {
  while (copies--) edges.push_back({u, v});
}

static vector<Edge> random_graph(mt19937 &rng, int n, int mode) {
  vector<Edge> edges;
  if (mode == 0) return edges;
  if (mode == 1) {
    for (int u = 0; u < n; ++u) add_copies(edges, u, u, rng() % 4);
    return edges;
  }
  if (mode == 2) {
    for (int u = 0; u + 1 < n; ++u) {
      add_copies(edges, u, u + 1, 1 + rng() % 3);
      add_copies(edges, u, u, rng() % 2);
    }
    return edges;
  }
  if (mode == 3) {
    for (int u = 0; u < n; ++u)
      add_copies(edges, u, (u + 1) % n, 1 + rng() % 2);
    for (int u = 0; u < n; ++u) add_copies(edges, u, u, rng() % 3);
    return edges;
  }
  if (mode == 4) {
    for (int u = 1; u < n; ++u) {
      add_copies(edges, 0, u, 1 + rng() % 2);
      if (u + 1 < n) add_copies(edges, u, u, rng() % 2);
    }
    return edges;
  }
  for (int u = 0; u < n; ++u) {
    add_copies(edges, u, u, rng() % 4);
    for (int v = u + 1; v < n; ++v)
      if (rng() % 100 < 42) add_copies(edges, u, v, 1 + rng() % 3);
  }
  return edges;
}

int main() {
  vector<pair<int, vector<Edge>>> fixed;
  fixed.push_back({1, {}});
  fixed.push_back({1, {{0, 0}, {0, 0}, {0, 0}}});
  fixed.push_back({6, {{0, 1}, {1, 2}, {2, 0}, {2, 3}, {3, 4}, {4, 5},
                       {4, 5}, {0, 0}, {3, 3}, {5, 5}}});
  fixed.push_back({8, {{0, 1}, {1, 2}, {2, 0}, {2, 3}, {3, 4}, {4, 2},
                       {4, 5}, {5, 6}, {6, 7}, {7, 5}, {1, 1}, {6, 6}}});
  fixed.push_back({7, {{0, 1}, {0, 1}, {1, 2}, {2, 3}, {3, 1}, {4, 5},
                       {5, 6}, {6, 4}, {3, 3}, {4, 4}}});

  long long cases = 0;
  mt19937 rng(0xBCCA2026);
  for (int i = 0; i < (int)fixed.size(); ++i) {
    auto graph = fixed[i];
    check_case("fixed", graph.first, graph.second);
    ++cases;
    for (int rep = 0; rep < 24; ++rep) {
      shuffle(graph.second.begin(), graph.second.end(), rng);
      check_case("fixed permutation", graph.first, graph.second);
      ++cases;
    }
  }

  for (int tc = 0; tc < 2200; ++tc) {
    const int n = 1 + rng() % 8;
    auto edges = random_graph(rng, n, tc % 6);
    const int repetitions = (tc % 5 == 0 ? 3 : 1);
    for (int rep = 0; rep < repetitions; ++rep) {
      shuffle(edges.begin(), edges.end(), rng);
      check_case("seeded random", n, edges);
      ++cases;
    }
  }
  cout << "BCC edge cases PASS " << cases
       << " targeted/permuted multigraphs\n";
}
