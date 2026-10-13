#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
#define X first
#define Y second
#define pb push_back
#include "../../codebook/9_Else/min_plus_convolution.cpp"
#include "../../codebook/2_Graph/Bridge.cpp"
#include "../../codebook/2_Graph/SCC.cpp"
#include "../../codebook/3_Data_Structure/IntervalContainer.cpp"
namespace mo {
#include "../../codebook/9_Else/Mos_Algorithm_On_Tree.cpp"
}
namespace qr {
#include "../../codebook/6_Math/QuadraticResidue.cpp"
}
#include "../04_graph_lowlink/oracle.hpp"

void require(bool ok, const char *what) {
  if (!ok) throw runtime_error(what);
}
void convolution() {
  auto check = [](const vector<ll> &a, const vector<ll> &b) {
    vector<ll> want;
    if (!a.empty() && !b.empty()) {
      want.assign(a.size() + b.size() - 1, LLONG_MAX);
      for (int i = 0; i < int(a.size()); ++i)
        for (int j = 0; j < int(b.size()); ++j) {
          __int128 sum = (__int128)a[i] + b[j];
          require(LLONG_MIN <= sum && sum <= LLONG_MAX, "test overflow");
          want[i + j] = min(want[i + j], ll(sum));
        }
    }
    require(min_plus_convolution(a, b) == want, "exact min-plus");
  };
  check({}, {}); check({}, {LLONG_MIN}); check({LLONG_MAX}, {});
  check({LLONG_MIN}, {0}); check({LLONG_MAX}, {0});
  // Rounded double sums lose Monge here; integer sums remain exact.
  check({0, 0, 0, 2, 7, 13, 20},
        {9007199254740998LL, 9007199254740992LL,
         9007199254740996LL});
  for (int n : {1, 2, 3, 31, 1024})
    for (int m : {1, 2, 3, 31, 1024}) {
      vector<ll> a(n), b(m);
      for (int i = 0; i < n; ++i) a[i] = LLONG_MIN + 4096 + i*i;
      for (int j = 0; j < m; ++j)
        b[j] = j % 3 == 0 ? 0 : j % 3 == 1 ? -4096 : 4096;
      check(a, b);
      for (int i = 0; i < n; ++i) a[i] = LLONG_MAX - i*4096LL;
      for (int j = 0; j < m; ++j) b[j] = -j % 13;
      check(a, b);
    }
}
void lowlink() {
  mt19937 rng(0x11ecc);
  // Dense SCC blocks in a DAG and undirected rings linked by bridges.
  for (int tc = 0; tc < 400; ++tc) {
    int n = 1 + rng() % 16;
    vector<pii> edges;
    for (int u = 0; u < n; ++u) {
      for (int copies = 0; copies < 3; ++copies) edges.pb({u, u});
      if (u + 1 < n) {
        edges.pb({u, u + 1});
        if (u % 3 != 2) edges.pb({u + 1, u});
      }
      if (u % 3 == 0 && u + 2 < n) edges.pb({u + 2, u});
    }
    vector<int> labels(n); iota(labels.begin(), labels.end(), 0);
    shuffle(labels.begin(), labels.end(), rng);
    for (auto &[u, v] : edges) u = labels[u], v = labels[v];
    shuffle(edges.begin(), edges.end(), rng);
    ECC e(n); SCC s(n);
    vector<vector<bool>> reach(n, vector<bool>(n));
    for (int u = 0; u < n; ++u) reach[u][u] = true;
    for (auto [u, v] : edges) {
      e.add_edge(u, v); s.add_edge(u, v); reach[u][v] = true;
    }
    for (int k = 0; k < n; ++k) for (int u = 0; u < n; ++u)
      for (int v = 0; v < n; ++v)
        reach[u][v] = reach[u][v] || (reach[u][k] && reach[k][v]);
    e.solve(); s.solve();
    auto flags = lowlink_test::bridge_edges(n, edges);
    require(e.is_bridge.size() == flags.size(), "bridge ids");
    for (int i = 0; i < int(flags.size()); ++i)
      require(e.is_bridge[i] == flags[i], "edge deletion oracle");
    require(lowlink_test::component_signature(e.bln) ==
            lowlink_test::edge_connected_signature(n, edges, flags),
            "ECC oracle");
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
      require((s.bln[u] == s.bln[v]) == (reach[u][v] && reach[v][u]),
              "SCC closure oracle");
    for (auto [u, v] : edges)
      require(s.bln[u] == s.bln[v] || s.bln[u] > s.bln[v],
              "reverse SCC order");
    auto eb = e.bln, sb = s.bln, bridges = e.is_bridge;
    int ne = e.necc, ns = s.nscc;
    e.solve(); s.solve(); // No added edges: repeated solve is a no-op.
    require(e.bln == eb && s.bln == sb && e.is_bridge == bridges &&
            e.necc == ne && s.nscc == ns && e.stk.empty() &&
            s.stk.empty() && count(s.instack.begin(), s.instack.end(), 1)
            == 0, "repeated lowlink solve");
  }
}
void intervals() {
  const vector<int> xs{INT_MIN, INT_MIN + 1, -100, -1, 0,
                       1, 100, INT_MAX - 1, INT_MAX};
  for (int mask = 0; mask < 256; ++mask)
    for (int l = 0; l < 9; ++l) for (int r = l; r < 9; ++r)
      for (bool add : {false, true}) {
        set<pii> got, want;
        for (int i = 0; i < 8; ++i) if (mask >> i & 1)
          addInterval(got, xs[i], xs[i + 1]);
        auto saved = got.end();
        if (l < r) for (auto it = got.begin(); it != got.end(); ++it)
          if (it->X <= xs[l] && xs[r] <= it->Y) saved = it;
        if (add) {
          auto it = addInterval(got, xs[l], xs[r]);
          if (l == r) require(it == got.end(), "empty interval result");
          else {
            require(it != got.end() && it->X <= xs[l] &&
                    xs[r] <= it->Y, "merged interval result");
            if (saved != got.end()) require(it == saved, "retained node");
          }
        } else removeInterval(got, xs[l], xs[r]);
        int result = mask;
        for (int i = l; i < r; ++i)
          if (add) result |= 1 << i; else result &= ~(1 << i);
        for (int i = 0; i < 8; ) {
          if (!(result >> i & 1)) { ++i; continue; }
          int j = i + 1;
          while (j < 8 && (result >> j & 1)) ++j;
          want.emplace(xs[i], xs[j]); i = j;
        }
        require(got == want, "all extreme interval coverage states");
      }
}
void mo_tree(const vector<vector<int>> &g, int root, mt19937 &rng) {
  int n = g.size();
  vector<int> par(n, -1), depth(n), in(n), out(n), ord;
  auto dfs = [&](auto &&self, int u) -> void {
    in[u] = ord.size(); ord.pb(u);
    for (int v : g[u]) if (v != par[u]) {
      par[v] = u; depth[v] = depth[u] + 1; self(self, v);
    }
    out[u] = ord.size(); ord.pb(u);
  };
  dfs(dfs, root);
  auto lca = [&](int u, int v) {
    while (depth[u] > depth[v]) u = par[u];
    while (depth[v] > depth[u]) v = par[v];
    while (u != v) u = par[u], v = par[v];
    return u;
  };
  // Independent BFS path masks; LCA is used only to build the queries.
  vector<int> want(n*n);
  for (int u = 0; u < n; ++u) {
    queue<int> q; q.push(u); vector<int> prev(n, -1); prev[u] = u;
    while (!q.empty()) {
      int v = q.front(); q.pop();
      for (int w : g[v]) if (prev[w] == -1)
        prev[w] = v, q.push(w);
    }
    for (int v = 0; v < n; ++v) {
      int mask = 1 << u;
      for (int w = v; w != u; w = prev[w]) mask |= 1 << w;
      want[u*n + v] = mask;
    }
  }
  for (int block : {1, 2, 3, INT_MAX}) {
    vector<mo::Query> qs;
    for (int copy = 0; copy < 2; ++copy)
      for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
        qs.emplace_back(u, v, block, in, out, lca,
                        copy*n*n + u*n + v);
    shuffle(qs.begin(), qs.end(), rng);
    int active = 0, last_l = 0, last_r = -1;
    vector<int> seen(qs.size());
    int queries = qs.size();
    mo::solve(move(qs), ord, [&](int u) { active ^= 1 << u; },
      [&](const mo::Query &q) {
        require(++seen.at(q.id) == 1 && active == want[q.id % (n*n)],
                "Mo duplicate oriented path and BFS membership");
        last_l = q.L; last_r = q.R;
      });
    int mask = 0;
    for (int i = last_l; i <= last_r; ++i) mask ^= 1 << ord[i];
    require(active == mask && count(seen.begin(), seen.end(), 1) ==
            queries, "Mo final state / query coverage");
  }
}
void mo_trees() {
  mt19937 rng(0x1100);
  int trees = 0;
  for (int n = 1; n <= 5; ++n) {
    int total = 1;
    for (int i = 0; i < n - 2; ++i) total *= n;
    for (int code = 0; code < total; ++code) {
      vector<int> sequence(max(0, n-2)), degree(n, 1);
      int z = code;
      for (int &u : sequence) u = z % n, z /= n, ++degree[u];
      vector<vector<int>> g(n);
      auto edge = [&](int u, int v) { g[u].pb(v); g[v].pb(u); };
      for (int u : sequence) {
        int v = find(degree.begin(), degree.end(), 1) - degree.begin();
        edge(u, v); --degree[u]; --degree[v];
      }
      if (n > 1) {
        int u = find(degree.begin(), degree.end(), 1) - degree.begin();
        int v = find(degree.begin() + u + 1, degree.end(), 1)
                - degree.begin();
        edge(u, v);
      }
      for (auto &row : g) shuffle(row.begin(), row.end(), rng);
      for (int root = 0; root < n; ++root) mo_tree(g, root, rng);
      ++trees;
    }
  }
  require(trees == 146, "Prufer coverage");
}
ll power(ll x, ll e, ll p) {
  ll y = 1;
  for (; e; e >>= 1, x = x*x % p) if (e & 1) y = y*x % p;
  return y;
}
int jacobi_oracle(int a, int m) {
  int result = 1;
  for (int p = 3; 1LL*p*p <= m; p += 2)
    while (m % p == 0) {
      int y = power((1LL*a % p + p) % p, (p-1)/2, p);
      result *= y == p-1 ? -1 : y; m /= p;
    }
  if (m > 1) {
    int y = power((1LL*a % m + m) % m, (m-1)/2, m);
    result *= y == m-1 ? -1 : y;
  }
  return result;
}
void residues() {
  for (int m : {1, 9, 81, 65535, 2147395609, INT_MAX})
    for (int a : {INT_MIN, -m, -1, 0, 1, m, INT_MAX})
      require(qr::Jacobi(a, m) == jacobi_oracle(a, m),
              "Jacobi composite powers and signed boundaries");
  for (int seed : {0, 1, 11, 999}) {
    srand(seed);
    for (int p : {2, 3, 17, 65537, 998244353, INT_MAX})
      for (ll x : {0LL, 1LL, 2LL, ll(p/2), ll(p-2), ll(p-1)}) {
        int a = x*x % p;
        for (int signed_a : {a, a-p}) {
          int root = qr::QuadraticResidue(signed_a, p);
          require(0 <= root && root < p && 1LL*root*root % p == a,
                  "Cipolla seeded square near INT_MAX");
        }
      }
  }
}
int main() {
  convolution(); lowlink(); intervals(); mo_trees(); residues();
  cout << "round11 PASS: exact min-plus, lowlink, 23040 interval states, "
          "146 labelled trees/all roots/blocks/duplicate paths, "
          "residues\n";
}
