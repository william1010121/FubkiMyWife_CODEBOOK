#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#include "../../codebook/2_Graph/SCC.cpp"

using Edge = pair<int, int>;

static vector<vector<char>> closure(int n, const vector<Edge> &edges) {
  vector<vector<char>> r(n, vector<char>(n));
  for (int i = 0; i < n; ++i) r[i][i] = 1;
  for (auto [u, v] : edges) r[u][v] = 1;
  for (int k = 0; k < n; ++k)
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
        r[i][j] |= r[i][k] & r[k][j];
  return r;
}

static void check(int tc, int n, const vector<Edge> &edges) {
  SCC got(n);
  for (auto [u, v] : edges) got.add_edge(u, v);
  got.solve();
  auto r = closure(n, edges);
  for (int u = 0; u < n; ++u)
    for (int v = 0; v < n; ++v)
      if ((got.bln[u] == got.bln[v]) != (r[u][v] && r[v][u])) {
        cerr << "SCC multigraph failure tc=" << tc << " n=" << n
             << " m=" << edges.size() << " pair=" << u << ',' << v << '\n';
        for (auto [a, b] : edges) cerr << a << ' ' << b << '\n';
        exit(1);
      }
  for (auto [u, v] : edges)
    if (got.bln[u] != got.bln[v] && got.bln[u] <= got.bln[v]) {
      cerr << "SCC multigraph failure tc=" << tc
           << ": component order differs\n";
      exit(1);
    }
}

static void add_repeated(vector<Edge> &e, int u, int v, int count) {
  while (count--) e.push_back({u, v});
}

int main() {
  int tc = 0;
  check(tc++, 7, {});  // all vertices disconnected

  {
    vector<Edge> e;
    for (int i = 0; i < 8; ++i) add_repeated(e, i, i, 9);
    check(tc++, 8, e);  // only self-loops, all duplicated
  }
  {
    vector<Edge> e;
    for (int i = 0; i + 1 < 12; ++i) {
      add_repeated(e, i, i + 1, 11);
      if (i & 1) add_repeated(e, i + 1, i, 3);
    }
    check(tc++, 12, e);  // alternating two-way links and duplicate arcs
  }
  {
    vector<Edge> e;
    for (int i = 0; i < 5; ++i) e.push_back({i, (i + 1) % 5});
    for (int i = 5; i < 9; ++i) e.push_back({i, 5 + (i - 4) % 4});
    add_repeated(e, 1, 6, 20);
    add_repeated(e, 8, 2, 17);
    check(tc++, 11, e);  // two SCCs, one-way cross edges, and isolated nodes
  }
  {
    vector<Edge> e;
    for (int u = 0; u < 6; ++u)
      for (int v = 0; v < 6; ++v) add_repeated(e, u, v, (u + v) % 5 + 1);
    check(tc++, 6, e);  // complete directed multigraph
  }

  mt19937 rng(0x53CCED);
  for (int it = 0; it < 12000; ++it) {
    int n = rng() % 13;
    vector<Edge> e;
    int m = rng() % 180;
    for (int j = 0; j < m; ++j) {
      int u = n ? int(rng() % n) : 0;
      int v = n ? int(rng() % n) : 0;
      if (n) e.push_back({u, v});
    }
    check(tc++, n, e);
  }

  cout << "SCC multigraph edge-case PASS " << tc << " cases\n";
}
