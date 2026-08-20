#include <bits/stdc++.h>
using namespace std;
#define N 64
#define INF 0x3f3f3f3f
#define SZ(x) ((int)(x).size())
#define pb push_back

#include "codebook/4_Flow_Matching/BoundedFlow.cpp"

struct Arc { int u, v, lo, hi; };

static bool feasible_dfs(const vector<Arc> &a, int at, vector<int> &f,
                         vector<int> &balance) {
  if (at == (int)a.size()) {
    for (int x : balance) if (x) return false;
    return true;
  }
  const Arc &e = a[at];
  for (f[at] = e.lo; f[at] <= e.hi; ++f[at]) {
    balance[e.u] += f[at];
    balance[e.v] -= f[at];
    if (feasible_dfs(a, at + 1, f, balance)) return true;
    balance[e.u] -= f[at];
    balance[e.v] += f[at];
  }
  return false;
}

static int max_value_dfs(const vector<Arc> &a, int at, int s, int t,
                         vector<int> &balance, int &answer) {
  if (at == (int)a.size()) {
    for (int x = 0; x < (int)balance.size(); ++x)
      if (x != s && x != t && balance[x]) return 0;
    answer = max(answer, balance[s]);
    return 1;
  }
  const Arc &e = a[at];
  int count = 0;
  for (int f = e.lo; f <= e.hi; ++f) {
    balance[e.u] += f;
    balance[e.v] -= f;
    count += max_value_dfs(a, at + 1, s, t, balance, answer);
    balance[e.u] -= f;
    balance[e.v] += f;
  }
  return count;
}

int main() {
  mt19937 rng(0x8badf00d);
  for (int tc = 0; tc < 1200; ++tc) {
    int n = 2 + rng() % 3, m = 1 + rng() % 7;
    vector<Arc> a;
    for (int i = 0; i < m; ++i) {
      int u = rng() % n, v = rng() % n;
      while (v == u) v = rng() % n;
      int lo = rng() % 3, hi = lo + rng() % 3;
      a.push_back({u, v, lo, hi});
    }
    vector<int> flow(m), balance(n);
    bool want = feasible_dfs(a, 0, flow, balance);
    BoundedFlow bf;
    bf.init(n);
    for (auto e : a) bf.add_edge(e.u, e.v, e.lo, e.hi);
    bool got = bf.solve();
    if (got != want) {
      cerr << "bounded circulation mismatch tc=" << tc
           << " got=" << got << " want=" << want << '\n';
      for (auto e : a) cerr << e.u << ' ' << e.v << ' ' << e.lo << ' ' << e.hi << '\n';
      return 1;
    }
  }

  for (int tc = 0; tc < 1200; ++tc) {
    int n = 2 + rng() % 4, s = 0, t = n - 1, m = 1 + rng() % 7;
    vector<Arc> a;
    for (int i = 0; i < m; ++i) {
      int u = rng() % n, v = rng() % n;
      while (v == u) v = rng() % n;
      a.push_back({u, v, 0, 1 + (int)(rng() % 3)});
    }
    vector<int> balance(n);
    int want = -1;
    max_value_dfs(a, 0, s, t, balance, want);
    BoundedFlow bf;
    bf.init(n);
    for (auto e : a) bf.add_edge(e.u, e.v, e.lo, e.hi);
    int got = bf.solve(s, t);
    if (got != want) {
      cerr << "bounded max-flow mismatch tc=" << tc
           << " got=" << got << " want=" << want << '\n';
      return 1;
    }
  }
  cout << "bounded_flow: PASS (2400 exhaustive-oracle random cases)\n";
}
