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
  cerr << "bridge_exhaustive failure in " << where << ": " << reason << '\n'
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

  const auto want_component =
      lowlink_test::edge_connected_signature(n, edges, want_bridge);
  const auto got_component = lowlink_test::component_signature(graph.bln);
  if (got_component != want_component)
    fail_case(where, n, edges, "2-edge-connected partition differs");

  set<int> component_ids(want_component.begin(), want_component.end());
  if (graph.necc != (int)component_ids.size())
    fail_case(where, n, edges, "component count differs");
  for (int u = 0; u < n; ++u)
    if (graph.bln[u] < 0 || graph.bln[u] >= graph.necc)
      fail_case(where, n, edges, "component id out of range");
  for (int i = 0; i < (int)edges.size(); ++i) {
    const auto [u, v] = edges[i];
    const bool same = graph.bln[u] == graph.bln[v];
    if (same != !(bool)graph.is_bridge[i])
      fail_case(where, n, edges, "edge endpoint/component invariant differs");
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
  cout << "Bridge exhaustive PASS " << cases
       << " multigraphs (loops, parallel edges, isolated vertices)\n";
}
