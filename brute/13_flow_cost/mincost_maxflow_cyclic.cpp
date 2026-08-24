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
      while (v == u) v = rng() % n;
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
    mf.init(n);
    for (const Arc &e : a) mf.add_edge(e.u, e.v, e.cap, e.cost);
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
  }
  cout << "mincost_maxflow_cyclic: PASS (4000 exact cyclic-flow cases)\n";
}
