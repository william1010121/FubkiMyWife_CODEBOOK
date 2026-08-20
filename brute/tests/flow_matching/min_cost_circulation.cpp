#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define N 32
#define INF ((ll)4e18)
#define SZ(x) ((int)(x).size())
#define pb push_back

#include "codebook/4_Flow_Matching/MinCostCirculation.cpp"

struct Arc { int u, v, cap, cost; };

static void brute(const vector<Arc> &a, int at, vector<int> &bal,
                  long long cost, long long &best) {
  if (at == (int)a.size()) {
    if (all_of(bal.begin(), bal.end(), [](int x) { return x == 0; }))
      best = min(best, cost);
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
  mt19937 rng(0xc1ac01a);
  for (int tc = 0; tc < 1800; ++tc) {
    int n = 2 + rng() % 4, m = 1 + rng() % 6;
    vector<Arc> a;
    int mx = 0;
    for (int i = 0; i < m; ++i) {
      int u = rng() % n, v = rng() % n;
      while (v == u) v = rng() % n;
      int cap = rng() % 4, cost = (int)(rng() % 9) - 4;
      a.push_back({u, v, cap, cost});
      mx = max(mx, cap);
    }
    long long want = INF;
    vector<int> bal(n);
    brute(a, 0, bal, 0, want);
    mcmf.init(n);
    vector<pair<int, int>> ref;
    for (auto e : a) {
      int idx = SZ(mcmf.G[e.u]);
      mcmf.add_edge(e.u, e.v, e.cap, e.cost);
      ref.push_back({e.u, idx});
    }
    mcmf.solve(mx ? 31 - __builtin_clz(mx) : 0);
    long long got = 0;
    for (int i = 0; i < m; ++i)
      got += 1LL * mcmf.G[ref[i].first][ref[i].second].flow * a[i].cost;
    if (got != want) {
      cerr << "minimum-cost circulation mismatch tc=" << tc
           << " got=" << got << " want=" << want << '\n';
      return 1;
    }
  }
  cout << "min_cost_circulation: PASS (1800 exhaustive-oracle random cases)\n";
}
