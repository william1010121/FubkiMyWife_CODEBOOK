#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#define pii pair<int,int>
#define ALL(x) (x).begin(), (x).end()
#include "../../../codebook/2_Graph/Bridge.cpp"

static int components(int n, const vector<pair<int,int>> &e, int ban) {
  vector<int> vis(n); int c = 0;
  for (int s = 0; s < n; ++s) if (!vis[s]) {
    ++c; queue<int> q; q.push(s), vis[s] = 1;
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (int i = 0; i < (int)e.size(); ++i) if (i != ban) {
        auto [a, b] = e[i];
        int v = a == u ? b : (b == u ? a : -1);
        if (v != -1 && !vis[v]) vis[v] = 1, q.push(v);
      }
    }
  }
  return c;
}

static vector<int> canon(const vector<int> &id) {
  map<int,int> ren; vector<int> r;
  for (int x : id) {
    if (!ren.count(x)) ren[x] = ren.size();
    r.pb(ren[x]);
  }
  return r;
}

static void fail(int tc, int n, const vector<pair<int,int>> &e, const string &why) {
  cerr << "Bridge failure tc=" << tc << ": " << why << "\n";
  cerr << n << " " << e.size() << "\n";
  for (auto [u, v] : e) cerr << u << " " << v << "\n";
  exit(1);
}

int main() {
  mt19937 rng(0xB21D6E);
  for (int tc = 0; tc < 5000; ++tc) {
    int n = 1 + rng() % 8;
    vector<pair<int,int>> e;
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      if (rng() % 100 < 40) {
        e.pb({u, v});
        if (rng() % 100 < 25) e.pb({u, v});
      }
    int cc = components(n, e, -1);
    ECC x(n);
    for (auto [u, v] : e) x.add_edge(u, v);
    x.solve();
    if ((int)x.is_bridge.size() != (int)e.size()) fail(tc, n, e, "bridge size differs");
    for (int i = 0; i < (int)e.size(); ++i)
      if ((bool)x.is_bridge[i] != (components(n, e, i) > cc))
        fail(tc, n, e, "bridge flag differs");
    vector<int> want(n), parent(n);
    iota(ALL(parent), 0);
    function<int(int)> find = [&](int u) { return parent[u] == u ? u : parent[u] = find(parent[u]); };
    for (int i = 0; i < (int)e.size(); ++i) if (!x.is_bridge[i])
      parent[find(e[i].first)] = find(e[i].second);
    for (int i = 0; i < n; ++i) want[i] = find(i);
    if (canon(x.bln) != canon(want)) fail(tc, n, e, "component partition differs");
  }
  cout << "Bridge PASS 5000 randomized multigraph cases\n";
}
