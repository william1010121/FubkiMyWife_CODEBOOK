#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#define ALL(x) (x).begin(), (x).end()
#include "../../../codebook/2_Graph/BCC_Vertex.cpp"

static bool conn(int n, const vector<pair<int,int>> &e, int banv, int bane,
                 int s, int t) {
  if (s == banv || t == banv) return false;
  vector<int> vis(n);
  queue<int> q;
  q.push(s), vis[s] = 1;
  while (!q.empty()) {
    int u = q.front(); q.pop();
    for (int i = 0; i < (int)e.size(); ++i) if (i != bane) {
      auto [a, b] = e[i];
      if (a == banv || b == banv) continue;
      if (a == u && !vis[b]) vis[b] = 1, q.push(b);
      if (b == u && !vis[a]) vis[a] = 1, q.push(a);
    }
  }
  return vis[t];
}

static int comps(int n, const vector<pair<int,int>> &e, int banv, int bane) {
  vector<int> vis(n); int ret = 0;
  for (int s = 0; s < n; ++s) if (s != banv && !vis[s]) {
    ++ret; queue<int> q; q.push(s), vis[s] = 1;
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (int i = 0; i < (int)e.size(); ++i) if (i != bane) {
        auto [a, b] = e[i];
        if (a == banv || b == banv) continue;
        int v = (a == u ? b : (b == u ? a : -1));
        if (v != -1 && !vis[v]) vis[v] = 1, q.push(v);
      }
    }
  }
  return ret;
}

static vector<vector<int>> oracle_blocks(int n, const vector<pair<int,int>> &e) {
  vector<vector<int>> rel(n, vector<int>(n));
  for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v) {
    rel[u][v] = rel[v][u] = conn(n, e, -1, -1, u, v);
    for (int w = 0; w < n && rel[u][v]; ++w)
      if (w != u && w != v && !conn(n, e, w, -1, u, v)) rel[u][v] = rel[v][u] = 0;
  }
  vector<vector<int>> ans;
  for (int mask = 1; mask < (1 << n); ++mask) {
    vector<int> s;
    for (int i = 0; i < n; ++i) if (mask >> i & 1) s.pb(i);
    bool clique = true;
    for (int i = 0; i < (int)s.size(); ++i)
      for (int j = i + 1; j < (int)s.size(); ++j)
        if (!rel[s[i]][s[j]]) clique = false;
    if (!clique) continue;
    bool maximal = true;
    for (int x = 0; x < n; ++x) if (!(mask >> x & 1)) {
      bool can = true;
      for (int y : s) if (!rel[x][y]) can = false;
      if (can) maximal = false;
    }
    if (maximal && (s.size() > 1 || !any_of(rel[s[0]].begin(), rel[s[0]].end(),
                                               [](int x) { return x; }))) ans.pb(s);
  }
  for (auto &s : ans) sort(ALL(s));
  sort(ALL(ans));
  return ans;
}

static vector<vector<int>> canon(vector<vector<int>> a) {
  for (auto &v : a) sort(ALL(v));
  sort(ALL(a));
  return a;
}

static void fail(int tc, int n, const vector<pair<int,int>> &e, const string &why) {
  cerr << "BCC_Vertex failure tc=" << tc << ": " << why << "\n";
  cerr << n << " " << e.size() << "\n";
  for (auto [u, v] : e) cerr << u << " " << v << "\n";
  exit(1);
}

int main() {
  mt19937 rng(0xBCCA11);
  for (int tc = 0; tc < 5000; ++tc) {
    int n = 1 + rng() % 8;
    vector<pair<int,int>> e;
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      if (rng() % 100 < 38) {
        e.pb({u, v});
        if (rng() % 100 < 20) e.pb({u, v});
      }
    BCC b(n);
    for (auto [u, v] : e) b.add_edge(u, v);
    b.solve();
    auto want = oracle_blocks(n, e);
    if (canon(b.bcc) != want) fail(tc, n, e, "blocks differ");
    int base = comps(n, e, -1, -1);
    for (int u = 0; u < n; ++u) {
      bool ap = comps(n, e, u, -1) > base;
      if ((bool)b.is_ap[u] != ap) fail(tc, n, e, "articulation point differs");
    }
    b.block_cut_tree();
    int apcnt = count(ALL(b.is_ap), 1);
    if ((int)b.nG.size() != (int)b.bcc.size() + apcnt)
      fail(tc, n, e, "block-cut size differs");
    set<pair<int,int>> got, expected;
    for (int i = 0; i < (int)b.bcc.size(); ++i)
      for (int u : b.bcc[i]) if (b.is_ap[u]) expected.insert({i, b.bln[u]});
    for (int i = 0; i < (int)b.bcc.size(); ++i)
      for (int u : b.nG[i]) got.insert({i, u});
    if (got != expected) fail(tc, n, e, "block-cut edges differ");
  }
  cout << "BCC_Vertex PASS 5000 randomized multigraph cases\n";
}
