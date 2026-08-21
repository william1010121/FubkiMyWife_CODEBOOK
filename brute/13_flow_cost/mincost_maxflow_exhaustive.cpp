#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#define N 64
#define INF (1LL << 60)
#define SZ(x) ((int)(x).size())
#define pb push_back

#include "codebook/4_Flow_Matching/MincostMaxflow.cpp"

struct Arc {
  int u, v, cap, cost;
};
struct Answer {
  int flow = -1;
  ll cost = 0;
};

static void brute(const vector<Arc> &a, int at, int s, int t,
                  vector<int> &bal, ll cost, Answer &best) {
  if (at == (int)a.size()) {
    for (int v = 0; v < (int)bal.size(); ++v)
      if (v != s && v != t && bal[v] != 0) return;
    if (bal[s] < 0 || bal[t] != -bal[s]) return;
    if (bal[s] > best.flow ||
        (bal[s] == best.flow && cost < best.cost)) {
      best.flow = bal[s];
      best.cost = cost;
    }
    return;
  }
  const Arc &e = a[at];
  for (int f = 0; f <= e.cap; ++f) {
    bal[e.u] += f;
    bal[e.v] -= f;
    brute(a, at + 1, s, t, bal, cost + 1LL * f * e.cost, best);
    bal[e.u] -= f;
    bal[e.v] += f;
  }
}

static void dump_case(const vector<Arc> &a) {
  for (const Arc &e : a)
    cerr << "  " << e.u << " -> " << e.v << " cap=" << e.cap
         << " cost=" << e.cost << '\n';
}

static void check(const vector<Arc> &a, int n, bool neg, int tc) {
  Answer want;
  vector<int> bal(n);
  brute(a, 0, 0, n - 1, bal, 0, want);

  MinCostMaxFlow mf;
  mf.init(n);
  for (const Arc &e : a) mf.add_edge(e.u, e.v, e.cap, e.cost);
  ll got_flow, got_cost;
  mf.solve(0, n - 1, got_flow, got_cost, neg);
  if (got_flow != want.flow || got_cost != want.cost) {
    cerr << "mincost maxflow mismatch case=" << tc << " neg=" << neg
         << " got=(" << got_flow << ',' << got_cost << ") want=("
         << want.flow << ',' << want.cost << ")\n";
    dump_case(a);
    exit(1);
  }
}

int main() {
  int cases = 0;
  const vector<pair<int, vector<Arc>>> edge_cases = {
      {2, {}},
      {2, {{0, 1, 0, -7}}},
      {2, {{0, 1, 3, 0}, {0, 1, 2, -2}}},
      {3, {{0, 1, 2, -4}, {1, 2, 1, 6}, {0, 2, 1, 0}}},
      {4, {{0, 1, 2, 1}, {0, 2, 2, 1}, {1, 3, 2, 2},
           {2, 3, 2, 2}, {1, 2, 2, -3}, {0, 3, 1, 5}}},
      {4, {{0, 1, 1, -8}, {1, 3, 1, 8}, {0, 2, 1, -2},
           {2, 3, 1, 2}, {0, 3, 0, -100}, {1, 2, 0, -100}}},
  };
  for (const auto &item : edge_cases) {
    bool has_negative = any_of(item.second.begin(), item.second.end(),
                                [](const Arc &e) { return e.cost < 0; });
    check(item.second, item.first, has_negative, cases++);
    if (!has_negative) check(item.second, item.first, false, cases++);
  }

  // Exhaust the three basic s -> {middle} -> t routes, including parallel
  // direct edges and every small signed cost/capacity combination.
  const vector<vector<pair<int, int>>> topologies = {
      {{0, 1}, {1, 2}, {0, 2}},
      {{0, 1}, {0, 1}, {1, 2}},
      {{0, 1}, {1, 2}, {1, 2}},
  };
  for (const auto &topo : topologies) {
    for (int code = 0; code < 3375; ++code) {
      int x = code;
      vector<Arc> a;
      bool nonnegative = true;
      for (auto [u, v] : topo) {
        int cap = x % 3;
        x /= 3;
        int cost = x % 5 - 2;
        x /= 5;
        nonnegative &= cost >= 0;
        a.push_back({u, v, cap, cost});
      }
      check(a, 3, true, cases++);
      if (nonnegative) check(a, 3, false, cases++);
    }
  }

  mt19937 rng(0x51AC057);
  for (int tc = 0; tc < 1800; ++tc) {
    int n = 2 + (int)(rng() % 4);
    int m = rng() % 9;
    vector<Arc> a;
    for (int i = 0; i < m; ++i) {
      int u = rng() % (n - 1);
      int v = u + 1 + rng() % (n - u - 1);
      a.push_back({u, v, (int)(rng() % 4), (int)(rng() % 17) - 8});
    }
    bool has_negative = any_of(a.begin(), a.end(),
                               [](const Arc &e) { return e.cost < 0; });
    check(a, n, true, cases++);
    if (!has_negative) check(a, n, false, cases++);
  }

  cout << "mincost_maxflow_exhaustive: PASS (" << cases
       << " exact DAG-flow cases)\n";
}
