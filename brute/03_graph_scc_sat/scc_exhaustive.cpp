#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#include "../../codebook/2_Graph/SCC.cpp"

using Edge = pair<int, int>;

static vector<vector<char>> transitive_closure(int n, const vector<Edge> &edges) {
  vector<vector<char>> reach(n, vector<char>(n));
  for (int i = 0; i < n; ++i) reach[i][i] = 1;
  for (auto [u, v] : edges) reach[u][v] = 1;
  for (int k = 0; k < n; ++k)
    for (int i = 0; i < n; ++i)
      if (reach[i][k])
        for (int j = 0; j < n; ++j)
          reach[i][j] |= reach[k][j];
  return reach;
}

static void fail(int tc, int n, const vector<Edge> &edges, const string &why) {
  cerr << "SCC exhaustive failure tc=" << tc << ": " << why << '\n';
  cerr << "n=" << n << " m=" << edges.size() << '\n';
  for (auto [u, v] : edges) cerr << u << ' ' << v << '\n';
  exit(1);
}

static void check(int tc, int n, const vector<Edge> &edges) {
  SCC got(n);
  for (auto [u, v] : edges) got.add_edge(u, v);
  got.solve();

  auto reach = transitive_closure(n, edges);
  for (int u = 0; u < n; ++u) {
    if (got.bln[u] < 0 || got.bln[u] >= got.nscc)
      fail(tc, n, edges, "component id out of range");
    for (int v = 0; v < n; ++v) {
      bool want = reach[u][v] && reach[v][u];
      bool have = got.bln[u] == got.bln[v];
      if (have != want) fail(tc, n, edges, "component partition differs");
    }
  }

  vector<char> used(got.nscc);
  for (int x : got.bln) used[x] = 1;
  if (count(used.begin(), used.end(), 1) != got.nscc)
    fail(tc, n, edges, "component ids are not contiguous");
  for (auto [u, v] : edges)
    if (got.bln[u] != got.bln[v] && got.bln[u] <= got.bln[v])
      fail(tc, n, edges, "component ids are not reverse topological");
}

int main() {
  int tc = 0;
  check(tc++, 0, {});

  // Every simple directed graph through four vertices, including all loops.
  for (int n = 1; n <= 4; ++n) {
    int edge_count = n * n;
    uint32_t limit = uint32_t(1) << edge_count;
    for (uint32_t mask = 0; mask < limit; ++mask) {
      vector<Edge> edges;
      for (int u = 0; u < n; ++u)
        for (int v = 0; v < n; ++v)
          if ((mask >> (u * n + v)) & 1u) edges.push_back({u, v});
      check(tc++, n, edges);
    }
  }

  cout << "SCC exhaustive PASS " << tc << " directed graphs\n";
}
