#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#define N 32
#define INF (1LL << 60)
#define SZ(x) ((int)(x).size())
#define pb push_back
#include "codebook/4_Flow_Matching/MincostMaxflow.cpp"

struct Arc { int u, v, cap, cost; };
struct Answer { int flow = -1; ll cost = 0; };

// Independent successive shortest paths: scan every residual edge with
// Bellman-Ford, without a heap or Johnson potentials.
struct BFReference {
  struct Edge { int u, v, cap, cost; };
  int n;
  vector<Edge> edges;
  explicit BFReference(int n) : n(n) {}
  void add(const Arc &e) {
    edges.push_back({e.u, e.v, e.cap, e.cost});
    edges.push_back({e.v, e.u, 0, -e.cost});
  }
  pair<ll, ll> solve(int s, int t) {
    ll flow = 0, cost = 0;
    if (s == t) return {flow, cost};
    for (;;) {
      vector<ll> dist(n, INF);
      vector<int> prev(n, -1);
      dist[s] = 0;
      for (int k = 1; k < n; ++k) {
        bool changed = false;
        for (int i = 0; i < (int)edges.size(); ++i) {
          const auto &e = edges[i];
          if (e.cap && dist[e.u] != INF && dist[e.u] + e.cost < dist[e.v]) {
            dist[e.v] = dist[e.u] + e.cost, prev[e.v] = i;
            changed = true;
          }
        }
        if (!changed) break;
      }
      if (dist[t] == INF) return {flow, cost};
      int push = INT_MAX;
      for (int v = t; v != s; v = edges[prev[v]].u)
        push = min(push, edges[prev[v]].cap);
      for (int v = t; v != s; v = edges[prev[v]].u) {
        edges[prev[v]].cap -= push;
        edges[prev[v] ^ 1].cap += push;
      }
      flow += push, cost += push * dist[t];
    }
  }
};

static void brute(const vector<Arc> &a, int at, vector<int> &bal,
                  ll cost, Answer &best) {
  if (at == (int)a.size()) {
    for (int v = 1; v + 1 < (int)bal.size(); ++v)
      if (bal[v]) return;
    if (bal[0] < 0 || bal.back() != -bal[0]) return;
    if (bal[0] > best.flow ||
        (bal[0] == best.flow && cost < best.cost)) {
      best.flow = bal[0];
      best.cost = cost;
    }
    return;
  }
  const Arc &e = a[at];
  for (int f = 0; f <= e.cap; ++f) {
    bal[e.u] += f;
    bal[e.v] -= f;
    brute(a, at + 1, bal, cost + 1LL * f * e.cost, best);
    bal[e.u] -= f;
    bal[e.v] += f;
  }
}

int main() {
  mt19937 rng(0xC1C1C);
  for (int tc = 0; tc < 4000; ++tc) {
    int n = 2 + rng() % 4, m = 1 + rng() % 8;
    vector<int> potential(n);
    for (int &x : potential) x = (int)(rng() % 11) - 5;
    vector<Arc> a;
    for (int i = 0; i < m; ++i) {
      int u = rng() % n, v = rng() % n;
      // cost = nonnegative base + potential[v] - potential[u]. Every cycle
      // therefore has nonnegative total cost, while individual edges may be
      // negative. This exercises Johnson initialisation on cyclic graphs.
      int base = rng() % 6;
      a.push_back({u, v, (int)(rng() % 3), base + potential[v] - potential[u]});
    }

    Answer want;
    vector<int> balance(n);
    brute(a, 0, balance, 0, want);

    MinCostMaxFlow mf;
    BFReference reference(n);
    mf.init(n);
    for (const Arc &e : a) {
      mf.add_edge(e.u, e.v, e.cap, e.cost);
      reference.add(e);
    }
    ll got_flow, got_cost;
    mf.solve(0, n - 1, got_flow, got_cost, true);
    if (got_flow != want.flow || got_cost != want.cost) {
      cerr << "cyclic mincost maxflow mismatch case=" << tc
           << " got=(" << got_flow << ',' << got_cost << ") want=("
           << want.flow << ',' << want.cost << ")\n";
      for (const Arc &e : a)
        cerr << e.u << ' ' << e.v << ' ' << e.cap << ' ' << e.cost << '\n';
      return 1;
    }
    auto first = reference.solve(0, n - 1);
    if (first != pair<ll, ll>{got_flow, got_cost}) return 2;
    // Same source again, reverse terminals, then a new arbitrary source.
    vector<pair<int, int>> queries = {{0, n - 1}, {n - 1, 0},
                                     {(int)(rng() % n), (int)(rng() % n)}};
    for (auto [s, t] : queries) {
      mf.solve(s, t, got_flow, got_cost, tc & 1);
      auto expected = reference.solve(s, t);
      if (expected != pair<ll, ll>{got_flow, got_cost}) {
        cerr << "repeated solve mismatch case=" << tc << " s=" << s << " t=" << t
             << " got=" << got_flow << ',' << got_cost << " expected="
             << expected.first << ',' << expected.second << '\n';
        return 3;
      }
    }
  }
  cout << "mincost_maxflow_cyclic: PASS (4000 exact cyclic-flow cases, "
          "12000 repeated queries against Bellman-Ford)\n";
}
