#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#define X first
#define Y second
#define ALL(v) (v).begin(), (v).end()
#define SZ(v) static_cast<int>((v).size())
#define pb push_back

#include "../../codebook/3_Data_Structure/min_heap.cpp"
#include "../../codebook/3_Data_Structure/DSU.cpp"
#include "../../codebook/2_Graph/Minimum_Arborescence_fast.cpp"

struct InputEdge {
  int s, t;
  ll w;
};

static ll oracle(int n, int root, const vector<InputEdge> &edges) {
  vector<vector<int>> incoming(n);
  for (int i = 0; i < SZ(edges); ++i)
    if (edges[i].s != edges[i].t && edges[i].t != root)
      incoming[edges[i].t].push_back(i);
  for (int v = 0; v < n; ++v)
    if (v != root && incoming[v].empty()) return -(1LL << 60);

  ll best = 1LL << 60;
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
          u = edges[parent[u]].s;
        }
        if (u != root) return;
      }
      best = min(best, cost);
      return;
    }
    for (int id : incoming[v]) parent[v] = id, dfs(v + 1);
  };
  dfs(0);
  return best == (1LL << 60) ? -(1LL << 60) : best;
}

static bool validates_solution(int n, int root, const vector<E> &edges,
                               const vector<int> &chosen, ll want, string &why) {
  if (n == 1) {
    if (!chosen.empty()) why = "nonempty solution for one vertex";
    return chosen.empty();
  }
  if (SZ(chosen) != n - 1) {
    why = "wrong number of selected edges";
    return false;
  }
  vector<int> parent(n, -1);
  ll cost = 0;
  for (int id : chosen) {
    if (id < 0 || id >= SZ(edges)) {
      why = "edge id out of range";
      return false;
    }
    const E &e = edges[id];
    if (e.s == e.t || e.t == root || parent[e.t] != -1) {
      why = "invalid incoming-edge set";
      return false;
    }
    parent[e.t] = id;
    cost += e.w;
  }
  for (int v = 0; v < n; ++v) if (v != root && parent[v] == -1) {
    why = "missing non-root parent";
    return false;
  }
  for (int v = 0; v < n; ++v) if (v != root) {
    vector<char> seen(n);
    int u = v;
    while (u != root && !seen[u]) {
      seen[u] = 1;
      u = edges[parent[u]].s;
    }
    if (u != root) {
      why = "selected edges contain a cycle away from root";
      return false;
    }
  }
  if (cost != want) {
    why = "selected edge cost differs from oracle";
    return false;
  }
  return true;
}

static void check(int tc, int n, int root, const vector<InputEdge> &input) {
  vector<E> edges;
  for (auto e : input) edges.push_back({e.s, e.t, e.w});
  ll want = oracle(n, root, input);
  vector<int> got = dmst(edges, n, root);
  if (want == -(1LL << 60)) {
    if (!got.empty()) {
      cerr << "dmst_fast mismatch tc=" << tc << " expected no solution\n";
      exit(1);
    }
    return;
  }
  string why;
  if (!validates_solution(n, root, edges, got, want, why)) {
    cerr << "dmst_fast mismatch tc=" << tc << " n=" << n
         << " root=" << root << ": " << why << "\n";
    for (auto e : input) cerr << e.s << ' ' << e.t << ' ' << e.w << '\n';
    exit(1);
  }
}

int main() {
  int tc = 0;
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

  mt19937 rng(0xD05F7A57);
  for (int round = 0; round < 4000; ++round) {
    int n = 2 + rng() % 6;
    int m = rng() % 20;
    vector<InputEdge> edges;
    for (int i = 0; i < m; ++i)
      edges.push_back({(int)(rng() % n), (int)(rng() % n),
                       (ll)(rng() % 101) - 50});
    check(++tc, n, rng() % n, edges);
  }
  cout << "dmst_fast PASS cases=" << tc
       << " oracle=enumerated-incoming-arcs-with-root-reachability\n";
}
