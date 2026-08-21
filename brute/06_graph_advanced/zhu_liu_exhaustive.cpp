#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#define N 16
#define SZ(v) static_cast<int>((v).size())
#define pb push_back
const ll INF = (1LL << 60);

#include "../../codebook/2_Graph/Minimum_Arborescence.cpp"

struct InputEdge {
  int u, v;
  ll w;
};

static ll oracle(int n, int root, const vector<InputEdge> &edges) {
  vector<vector<int>> incoming(n);
  for (int i = 0; i < SZ(edges); ++i)
    if (edges[i].u != edges[i].v && edges[i].v != root)
      incoming[edges[i].v].push_back(i);
  for (int v = 0; v < n; ++v)
    if (v != root && incoming[v].empty()) return -INF;

  ll best = INF;
  vector<int> parent(n, -1);
  function<void(int)> dfs = [&](int v) {
    while (v < n && v == root) ++v;
    if (v == n) {
      ll cost = 0;
      for (int x = 0; x < n; ++x)
        if (x != root) cost += edges[parent[x]].w;
      for (int x = 0; x < n; ++x) if (x != root) {
        vector<char> seen(n);
        int u = x;
        while (u != root && !seen[u]) {
          seen[u] = 1;
          u = edges[parent[u]].u;
        }
        if (u != root) return;
      }
      best = min(best, cost);
      return;
    }
    for (int id : incoming[v]) {
      parent[v] = id;
      dfs(v + 1);
    }
  };
  dfs(0);
  return best == INF ? -INF : best;
}

static void check(int tc, int n, int root, const vector<InputEdge> &edges) {
  zhu_liu solver;
  solver.init();
  for (auto e : edges) solver.add_edge(e.u, e.v, e.w);
  ll got = solver.build(root, n);
  ll want = oracle(n, root, edges);
  if (got != want) {
    cerr << "zhu_liu mismatch tc=" << tc << " n=" << n
         << " root=" << root << " got=" << got << " want=" << want << '\n';
    for (auto e : edges)
      cerr << e.u << ' ' << e.v << ' ' << e.w << '\n';
    exit(1);
  }
}

int main() {
  int tc = 0;

  // Explicit degenerate cases: loops are ignored by add_edge, while parallel
  // arcs and negative/zero costs remain meaningful.
  vector<vector<InputEdge>> special = {
    {{0, 0, -99}},
    {{0, 0, -99}, {0, 1, 7}, {0, 1, -4}, {1, 0, -8}},
    {{0, 1, 0}, {1, 2, 0}, {2, 1, -3}},
    {{0, 1, -10}, {1, 2, -10}, {2, 0, -10}, {0, 2, 8}},
    {{1, 0, 4}, {2, 0, 5}, {1, 2, -7}, {2, 1, -6}},
    {{0, 1, 1}, {2, 3, 1}, {3, 2, 1}},
  };
  vector<int> special_n = {1, 2, 3, 3, 3, 4};
  for (int i = 0; i < SZ(special); ++i)
    for (int root = 0; root < special_n[i]; ++root)
      check(++tc, special_n[i], root, special[i]);

  // Exhaust every simple directed graph through four vertices. The weight
  // pattern changes with the mask, so the same topology is not only tested
  // with nonnegative costs.
  for (int n = 2; n <= 4; ++n) {
    vector<pair<int, int>> arcs;
    for (int u = 0; u < n; ++u)
      for (int v = 0; v < n; ++v)
        if (u != v) arcs.push_back({u, v});
    const int total = 1 << SZ(arcs);
    for (int mask = 0; mask < total; ++mask) {
      vector<InputEdge> edges;
      for (int i = 0; i < SZ(arcs); ++i) if (mask >> i & 1) {
        ll w = (ll)((i * 17 + mask * 11) % 41) - 20;
        edges.push_back({arcs[i].first, arcs[i].second, w});
      }
      for (int root = 0; root < n; ++root)
        check(++tc, n, root, edges);
    }
  }

  mt19937 rng(0xA8B0C0DE);
  for (int round = 0; round < 4000; ++round) {
    int n = 2 + rng() % 6;
    int m = rng() % 19;
    vector<InputEdge> edges;
    for (int i = 0; i < m; ++i) {
      int u = rng() % n, v = rng() % n;
      ll w = (ll)(rng() % 101) - 50;
      edges.push_back({u, v, w});
    }
    check(++tc, n, rng() % n, edges);
  }

  cout << "zhu_liu PASS cases=" << tc
       << " oracle=enumerated-incoming-arcs-with-root-reachability\n";
}
