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
  if (got.dft != n || !got.stk.empty() ||
      any_of(got.instack.begin(), got.instack.end(), [](int x) { return x; })) {
    cerr << "SCC multigraph failure tc=" << tc << ": active stack remains\n";
    exit(1);
  }
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
  check(tc++, 0, {});
  check(tc++, 1, {});
  check(tc++, 1, {{0, 0}, {0, 0}});
  // A completed SCC is reachable later but must not lower active low links.
  check(tc++, 5, {{0, 1}, {1, 0}, {2, 1}, {2, 3},
                  {3, 2}, {4, 3}, {4, 0}});
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

  // A later sibling reaches both a popped SCC and an active ancestor's
  // descendant. Relabeling changes DFS roots, and shuffling changes siblings.
  vector<Edge> adversarial = {{0, 1}, {1, 2}, {2, 0}, {0, 3},
      {3, 4}, {4, 1}, {4, 5}, {5, 6}, {6, 5}, {0, 7},
      {7, 6}, {7, 3}, {8, 7}, {8, 8}, {9, 8}};
  mt19937 order_rng(0x8085CC);
  vector<int> labels(11);
  iota(labels.begin(), labels.end(), 0);
  for (int rep = 0; rep < 600; ++rep) {
    shuffle(labels.begin(), labels.end(), order_rng);
    vector<Edge> edges;
    for (auto [u, v] : adversarial) {
      edges.push_back({labels[u], labels[v]});
      edges.push_back({labels[u], labels[v]});
    }
    shuffle(edges.begin(), edges.end(), order_rng);
    check(tc++, labels.size(), edges);
  }

  cout << "SCC multigraph edge-case PASS " << tc << " cases\n";
}
