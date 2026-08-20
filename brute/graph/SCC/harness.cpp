#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#include "../../../codebook/2_Graph/SCC.cpp"

static vector<vector<int>> reachability(int n, const vector<pair<int,int>> &e) {
  vector<vector<int>> r(n, vector<int>(n));
  for (int i = 0; i < n; ++i) r[i][i] = 1;
  for (auto [u, v] : e) r[u][v] = 1;
  for (int k = 0; k < n; ++k) for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) r[i][j] |= r[i][k] & r[k][j];
  return r;
}

static vector<int> canon(const vector<int> &id) {
  map<int,int> ren; vector<int> r;
  for (int x : id) {
    if (!ren.count(x)) ren[x] = ren.size();
    r.push_back(ren[x]);
  }
  return r;
}

static void fail(int tc, int n, const vector<pair<int,int>> &e, const string &why) {
  cerr << "SCC failure tc=" << tc << ": " << why << "\n";
  cerr << n << " " << e.size() << "\n";
  for (auto [u, v] : e) cerr << u << " " << v << "\n";
  exit(1);
}

int main() {
  mt19937 rng(0x5CC123);
  for (int tc = 0; tc < 5000; ++tc) {
    int n = 1 + rng() % 8;
    vector<pair<int,int>> e;
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
      if (rng() % 100 < 32) {
        e.push_back({u, v});
        if (rng() % 100 < 15) e.push_back({u, v});
      }
    SCC s(n);
    for (auto [u, v] : e) s.add_edge(u, v);
    s.solve();
    auto r = reachability(n, e);
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
      if ((s.bln[u] == s.bln[v]) != (r[u][v] && r[v][u]))
        fail(tc, n, e, "component partition differs");
    for (auto [u, v] : e)
      if (s.bln[u] != s.bln[v] && s.bln[u] <= s.bln[v])
        fail(tc, n, e, "component ids are not reverse topological");
    vector<int> got = canon(s.bln), want = got;
    (void)want;
  }
  cout << "SCC PASS 5000 randomized directed multigraph cases\n";
}
