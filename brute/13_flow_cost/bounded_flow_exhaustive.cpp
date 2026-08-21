#include <bits/stdc++.h>
using namespace std;

#define N 64
#define INF 0x3f3f3f3f
#define SZ(x) ((int)(x).size())
#define pb push_back

#include "codebook/4_Flow_Matching/BoundedFlow.cpp"

struct Arc {
  int u, v, lo, hi;
  bool plain;
};

static bool circulation_brute(const vector<Arc> &a, int at,
                              vector<int> &bal) {
  if (at == (int)a.size())
    return all_of(bal.begin(), bal.end(), [](int x) { return x == 0; });
  const Arc &e = a[at];
  for (int f = e.lo; f <= e.hi; ++f) {
    bal[e.u] += f;
    bal[e.v] -= f;
    if (circulation_brute(a, at + 1, bal)) return true;
    bal[e.u] -= f;
    bal[e.v] += f;
  }
  return false;
}

static void maxflow_brute(const vector<Arc> &a, int at, int s, int t,
                          vector<int> &bal, int &best) {
  if (at == (int)a.size()) {
    for (int v = 0; v < (int)bal.size(); ++v)
      if (v != s && v != t && bal[v] != 0) return;
    // solve(s,t) adds a nonnegative t -> s balancing edge, so negative
    // source value is not a feasible bounded s-t flow for this interface.
    if (bal[s] >= 0) best = max(best, bal[s]);
    return;
  }
  const Arc &e = a[at];
  for (int f = e.lo; f <= e.hi; ++f) {
    bal[e.u] += f;
    bal[e.v] -= f;
    maxflow_brute(a, at + 1, s, t, bal, best);
    bal[e.u] -= f;
    bal[e.v] += f;
  }
}

static bool run_circulation(const vector<Arc> &a, int n) {
  BoundedFlow bf;
  bf.init(n);
  for (const Arc &e : a) {
    if (e.plain) bf.add_edge(e.u, e.v, e.hi);
    else bf.add_edge(e.u, e.v, e.lo, e.hi);
  }
  return bf.solve();
}

static int run_maxflow(const vector<Arc> &a, int n, int s, int t) {
  BoundedFlow bf;
  bf.init(n);
  for (const Arc &e : a) {
    if (e.plain) bf.add_edge(e.u, e.v, e.hi);
    else bf.add_edge(e.u, e.v, e.lo, e.hi);
  }
  return bf.solve(s, t);
}

static void dump_case(const vector<Arc> &a) {
  for (const Arc &e : a)
    cerr << "  " << e.u << " -> " << e.v << " [" << e.lo << ',' << e.hi
         << "]" << (e.plain ? " plain" : " bounded") << '\n';
}

static void check(const vector<Arc> &a, int n, int tc) {
  vector<int> bal(n);
  bool want_circ = circulation_brute(a, 0, bal);
  bool got_circ = run_circulation(a, n);
  if (got_circ != want_circ) {
    cerr << "bounded circulation mismatch case=" << tc << " got="
         << got_circ << " want=" << want_circ << '\n';
    dump_case(a);
    exit(1);
  }

  if (n >= 2) {
    int want_max = -1;
    fill(bal.begin(), bal.end(), 0);
    maxflow_brute(a, 0, 0, n - 1, bal, want_max);
    int got_max = run_maxflow(a, n, 0, n - 1);
    if (got_max != want_max) {
      cerr << "bounded max-flow mismatch case=" << tc << " got="
           << got_max << " want=" << want_max << '\n';
      dump_case(a);
      exit(1);
    }
  }
}

int main() {
  int cases = 0;

  // Explicit contract edges: empty graph, self-loops, parallel arcs,
  // disconnected components, forced cycles, and infeasible lower bounds.
  const vector<pair<int, vector<Arc>>> edge_cases = {
      {1, {}},
      {1, {{0, 0, 0, 0, true}, {0, 0, 1, 3, false}}},
      {2, {{0, 1, 1, 1, false}, {1, 0, 1, 1, false}}},
      {2, {{0, 1, 1, 2, false}, {0, 1, 0, 0, true}}},
      {2, {{0, 1, 1, 2, false}, {1, 0, 0, 0, true}}},
      {3, {{0, 1, 1, 2, false}, {1, 2, 1, 1, false},
           {2, 0, 0, 0, true}}},
      {3, {{0, 1, 0, 2, true}, {1, 2, 0, 2, true},
           {2, 0, 0, 2, true}, {1, 1, 2, 2, false}}},
      {4, {{0, 1, 0, 2, true}, {1, 3, 0, 2, true},
           {0, 2, 1, 1, false}, {2, 3, 0, 1, true},
           {3, 0, 0, 0, true}}},
  };
  for (const auto &item : edge_cases) {
    check(item.second, item.first, cases++);
  }

  const vector<pair<int, int>> bounds = {
      {0, 0}, {0, 1}, {0, 2}, {1, 1}, {1, 2}, {2, 2},
  };
  const vector<pair<int, int>> types2 = {
      {0, 0}, {0, 1}, {1, 0}, {1, 1},
  };
  // Exhaust every endpoint/bound combination for three tiny two-vertex arcs.
  for (int code = 0; code < 64 * 216; ++code) {
    int x = code;
    vector<Arc> a;
    for (int i = 0; i < 3; ++i) {
      auto [u, v] = types2[x % 4];
      x /= 4;
      auto [lo, hi] = bounds[x % 6];
      x /= 6;
      a.push_back({u, v, lo, hi, lo == 0 && ((code + i) % 5 == 0)});
    }
    check(a, 2, cases++);
  }

  const vector<vector<pair<int, int>>> topologies3 = {
      {{0, 1}, {1, 2}, {0, 2}, {2, 0}},
      {{0, 0}, {0, 1}, {1, 0}, {2, 2}},
      {{0, 1}, {0, 1}, {1, 2}, {1, 2}},
      {{2, 0}, {0, 2}, {1, 2}, {2, 1}},
  };
  // Exhaust bounds on several cyclic, parallel, and self-loop topologies.
  for (const auto &topo : topologies3) {
    for (int code = 0; code < 1296; ++code) {
      int x = code;
      vector<Arc> a;
      for (int i = 0; i < (int)topo.size(); ++i) {
        auto [lo, hi] = bounds[x % 6];
        x /= 6;
        a.push_back({topo[i].first, topo[i].second, lo, hi,
                     lo == 0 && ((code + i) % 7 == 0)});
      }
      check(a, 3, cases++);
    }
  }

  mt19937 rng(0xB0DED123);
  for (int tc = 0; tc < 1800; ++tc) {
    int n = 1 + (int)(rng() % 4);
    int m = rng() % 7;
    vector<Arc> a;
    for (int i = 0; i < m; ++i) {
      int u = rng() % n, v = rng() % n;
      int lo = rng() % 3, hi = lo + rng() % 3;
      a.push_back({u, v, lo, hi, lo == 0 && rng() % 3 == 0});
    }
    check(a, n, cases++);
  }

  cout << "bounded_flow_exhaustive: PASS (" << cases
       << " circulation/max-flow oracle cases)\n";
}
