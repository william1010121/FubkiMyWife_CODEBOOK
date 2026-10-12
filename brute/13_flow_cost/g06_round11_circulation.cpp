#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 16;
const ll INF = 4000000000000000000LL;
#define SZ(a) ((int)(a).size())
#define pb push_back
#include "../../codebook/4_Flow_Matching/MinCostCirculation.cpp"
struct Arc { int u, v; ll cap, cost; };
void require(bool ok, const char *msg) {
  if (!ok) { cerr << "FAIL round11 circulation " << msg << '\n'; exit(1); }
}
void check(int n, vector<Arc> arcs, mt19937_64 &rng) {
  for (int order = 0; order < 3; ++order) {
    shuffle(arcs.begin(), arcs.end(), rng); mcmf.init(n);
    vector<pair<int, int>> refs;
    ll capacity = 0;
    for (auto a : arcs) {
      refs.pb({a.u, SZ(mcmf.G[a.u])});
      mcmf.add_edge(a.u, a.v, a.cap, a.cost);
      capacity = max(capacity, a.cap);
    }
    mcmf.solve(capacity ? 63 - countl_zero((uint64_t)capacity) : 0);
    vector<__int128> balance(n); __int128 objective = 0;
    for (int i = 0; i < SZ(arcs); ++i) {
      auto a = arcs[i]; auto [u, j] = refs[i]; auto &e = mcmf.G[u][j];
      require(e.cap == a.cap && 0 <= e.flow && e.flow <= e.cap,
              "capacity reconstruction and feasible forward flow");
      balance[e.from] -= e.flow; balance[e.to] += e.flow;
      objective += (__int128)e.flow * e.cost;
    }
    require(objective <= 0, "zero circulation remains feasible");
    for (auto x : balance) require(x == 0, "wide flow conservation");
    for (int u = 0; u < n; ++u) for (int j = 0; j < SZ(mcmf.G[u]); ++j) {
      auto &e = mcmf.G[u][j], &r = mcmf.G[e.to][e.rev];
      require(r.from == e.to && r.to == u && r.rev == j &&
              r.flow == -e.flow && r.cost == -e.cost,
              "residual reciprocity including loops and parallel arcs");
    }
    // Independent optimality certificate: a feasible circulation is
    // optimal exactly when its residual graph has no negative cycle.
    vector<__int128> d(n);
    for (int pass = 0; pass < n; ++pass) {
      bool changed = false;
      for (int u = 0; u < n; ++u) for (auto &e : mcmf.G[u])
        if (e.cap > e.flow && d[e.to] > d[u] + e.cost)
          d[e.to] = d[u] + e.cost, changed = true;
      require(pass < n - 1 || !changed, "no negative residual cycle");
      if (!changed) break;
    }
  }
}
int main() {
  mt19937_64 rng(110604);
  check(0, {}, rng); check(1, {}, rng); check(N, {}, rng);
  for (ll c : {0LL, 1LL, (1LL << 31) - 1, 1LL << 62, LLONG_MAX})
    check(1, {{0, 0, c, -1}, {0, 0, c, 1}, {0, 0, c, 0}}, rng);
  for (int tc = 0; tc < 200; ++tc) {
    int n = 1 + rng() % N; vector<Arc> a;
    for (int i = 0; i < 60; ++i) {
      int bit = rng() % 63; ll cap = (1LL << bit) - 1;
      if (i % 7 == 0) cap = LLONG_MAX;
      if (i % 11 == 0) cap = 0;
      a.pb({int(rng() % n), int(rng() % n), cap, ll(rng() % 9) - 4});
    }
    check(n, a, rng);
  }
  cout << "round11 circulation: PASS 624 solves, full-width capacities, "
          "feasibility and residual optimality certificates\n";
}
