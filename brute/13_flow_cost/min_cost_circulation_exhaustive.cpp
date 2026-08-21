#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#define N 64
#define INF ((ll)4e18)
#define SZ(x) ((int)(x).size())
#define pb push_back

#include "codebook/4_Flow_Matching/MinCostCirculation.cpp"

struct Arc {
  int u, v, cap, cost;
};

static void brute(const vector<Arc> &a, int at, vector<int> &bal, ll cost,
                  ll &best) {
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

static void dump_case(const vector<Arc> &a) {
  for (const Arc &e : a)
    cerr << "  " << e.u << " -> " << e.v << " cap=" << e.cap
         << " cost=" << e.cost << '\n';
}

static void check(const vector<Arc> &a, int n, int tc) {
  ll want = INF;
  vector<int> bal(n);
  brute(a, 0, bal, 0, want);

  int mx = 0;
  for (const Arc &e : a) mx = max(mx, e.cap);
  int mxlg = mx ? 31 - __builtin_clz(mx) : 0;

  mcmf.init(n);
  vector<pair<int, int>> ref;
  for (const Arc &e : a) {
    int idx = SZ(mcmf.G[e.u]);
    mcmf.add_edge(e.u, e.v, e.cap, e.cost);
    ref.push_back({e.u, idx});
  }
  mcmf.solve(mxlg);
  ll got = 0;
  for (int i = 0; i < (int)a.size(); ++i)
    got += mcmf.G[ref[i].first][ref[i].second].flow * 1LL * a[i].cost;
  if (got != want) {
    cerr << "minimum-cost circulation mismatch case=" << tc << " got="
         << got << " want=" << want << " mxlg=" << mxlg << '\n';
    dump_case(a);
    exit(1);
  }
}

int main() {
  int cases = 0;
  const vector<vector<Arc>> edge_cases = {
      {},
      {{0, 0, 0, -9}},
      {{0, 0, 4, -3}},
      {{0, 1, 2, -5}, {1, 0, 3, 1}},
      {{0, 1, 2, -4}, {1, 0, 2, 3}, {0, 1, 1, -2}},
      {{0, 1, 2, -4}, {1, 2, 2, 2}, {2, 0, 2, 0},
       {0, 2, 1, -1}},
      {{0, 1, 0, -100}, {1, 0, 0, -100}, {1, 1, 3, 0}},
  };
  for (int i = 0; i < (int)edge_cases.size(); ++i) {
    int n = i == 0 ? 1 : (i < 4 ? 2 : 3);
    check(edge_cases[i], n, cases++);
  }

  // Exhaust capacities and signed costs on cycles, parallel arcs, and a
  // negative self-loop.  This includes the zero-capacity/mxlg=0 path.
  const vector<vector<pair<int, int>>> topologies = {
      {{0, 1}, {1, 0}, {0, 0}},
      {{0, 1}, {1, 0}, {1, 0}},
      {{0, 1}, {1, 2}, {2, 0}},
  };
  for (const auto &topo : topologies) {
    for (int code = 0; code < 3375; ++code) {
      int x = code;
      vector<Arc> a;
      for (auto [u, v] : topo) {
        int cap = x % 3;
        x /= 3;
        int cost = x % 5 - 2;
        x /= 5;
        a.push_back({u, v, cap, cost});
      }
      check(a, 3, cases++);
    }
  }

  mt19937 rng(0xC1AC0DE);
  for (int tc = 0; tc < 1400; ++tc) {
    int n = 1 + (int)(rng() % 4);
    int m = rng() % 7;
    vector<Arc> a;
    for (int i = 0; i < m; ++i) {
      int u = rng() % n, v = rng() % n;
      a.push_back({u, v, (int)(rng() % 5), (int)(rng() % 13) - 6});
    }
    check(a, n, cases++);
  }

  cout << "min_cost_circulation_exhaustive: PASS (" << cases
       << " exact circulation cases)\n";
}
