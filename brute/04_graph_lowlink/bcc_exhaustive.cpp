#include <bits/stdc++.h>
using namespace std;

#define pb push_back
#include "../../codebook/2_Graph/BCC_Vertex.cpp"
#include "oracle.hpp"

using lowlink_test::Edge;

[[noreturn]] static void fail_case(const string &where, int n,
                                   const vector<Edge> &edges,
                                   const string &reason) {
  cerr << "bcc_exhaustive failure in " << where << ": " << reason << '\n'
       << lowlink_test::describe(n, edges) << '\n';
  exit(1);
}

static void check_case(const string &where, int n, const vector<Edge> &edges) {
  BCC graph(n);
  for (auto [u, v] : edges) graph.add_edge(u, v);
  graph.solve();

  const auto want_blocks = lowlink_test::vertex_blocks(n, edges);
  const auto got_blocks = lowlink_test::canonical_blocks(graph.bcc);
  if (got_blocks != want_blocks)
    fail_case(where, n, edges, "vertex blocks differ");

  const auto want_ap = lowlink_test::articulation_points(n, edges);
  for (int u = 0; u < n; ++u)
    if ((bool)graph.is_ap[u] != (bool)want_ap[u])
      fail_case(where, n, edges, "articulation point differs");

  const int base = graph.bcc.size();
  vector<int> containing(n, -1);
  for (int i = 0; i < base; ++i) {
    for (int u : graph.bcc[i]) {
      if (!want_ap[u]) {
        if (containing[u] != -1 && containing[u] != i)
          fail_case(where, n, edges, "non-articulation vertex in two blocks");
        containing[u] = i;
      }
    }
  }
  for (int u = 0; u < n; ++u) {
    if (!want_ap[u] && containing[u] == -1)
      fail_case(where, n, edges, "vertex missing from its block");
  }

  graph.block_cut_tree();
  const int ap_count = count(want_ap.begin(), want_ap.end(), true);
  if ((int)graph.nG.size() != base + ap_count)
    fail_case(where, n, edges, "block-cut tree node count differs");

  vector<int> ap_node(n, -1);
  int next = base;
  for (int u = 0; u < n; ++u) if (want_ap[u]) ap_node[u] = next++;
  vector<pair<int, int>> want_tree;
  for (int i = 0; i < base; ++i) {
    for (int u : graph.bcc[i]) if (want_ap[u]) {
      want_tree.push_back({i, ap_node[u]});
      want_tree.push_back({ap_node[u], i});
    }
  }
  vector<pair<int, int>> got_tree;
  for (int u = 0; u < (int)graph.nG.size(); ++u)
    for (int v : graph.nG[u]) got_tree.push_back({u, v});
  sort(want_tree.begin(), want_tree.end());
  sort(got_tree.begin(), got_tree.end());
  if (got_tree != want_tree)
    fail_case(where, n, edges, "block-cut tree edges differ");

  for (int u = 0; u < n; ++u) {
    if (want_ap[u]) {
      if (graph.bln[u] != ap_node[u])
        fail_case(where, n, edges, "articulation id differs");
    } else if (graph.bln[u] != containing[u]) {
      fail_case(where, n, edges, "vertex block id differs");
    }
  }
}

int main() {
  long long cases = 0;
  for (int n = 1; n <= 4; ++n) {
    lowlink_test::enumerate_multigraphs(n, 2, [&](const vector<Edge> &edges) {
      check_case("exhaustive", n, edges);
      ++cases;
    });
  }
  cout << "BCC exhaustive PASS " << cases
       << " multigraphs (loops, parallel edges, isolated vertices)\n";
}
