#include <bits/stdc++.h>
using namespace std;
#define N 64
#define pb push_back
#include "../../../codebook/2_Graph/Dominator_Tree.cpp"

static void fail(int tc, int n, const vector<pair<int,int>> &e, const string &why) {
  cerr << "Dominator_Tree failure tc=" << tc << ": " << why << "\n";
  cerr << n << " " << e.size() << "\n";
  for (auto [u, v] : e) cerr << u << " " << v << "\n";
  exit(1);
}

int main() {
  mt19937 rng(0xD0A1);
  for (int tc = 0; tc < 5000; ++tc) {
    int n = 1 + rng() % 9;
    vector<pair<int,int>> e;
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
      if (rng() % 100 < 28) {
        e.push_back({u, v});
        if (rng() % 100 < 12) e.push_back({u, v});
      }
    vector<vector<int>> g(n), rg(n);
    dominator_tree d;
    d.init(n);
    for (auto [u, v] : e) d.add_edge(u + 1, v + 1), g[u].pb(v), rg[v].pb(u);
    d.tarjan(1);

    vector<int> reach(n); queue<int> q; q.push(0), reach[0] = 1;
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (int v : g[u]) if (!reach[v]) reach[v] = 1, q.push(v);
    }
    vector<set<int>> dom(n);
    for (int v = 0; v < n; ++v) if (reach[v]) {
      if (v == 0) dom[v].insert(0);
      else for (int x = 0; x < n; ++x) if (reach[x]) dom[v].insert(x);
    }
    bool changed = true;
    while (changed) {
      changed = false;
      for (int v = 1; v < n; ++v) if (reach[v]) {
        set<int> nd;
        bool first = true;
        for (int p : rg[v]) if (reach[p]) {
          if (first) nd = dom[p], first = false;
          else {
            set<int> z;
            set_intersection(nd.begin(), nd.end(), dom[p].begin(), dom[p].end(),
                             inserter(z, z.begin()));
            nd.swap(z);
          }
        }
        nd.insert(v);
        if (nd != dom[v]) dom[v] = nd, changed = true;
      }
    }
    vector<int> want(n, 0);
    for (int v = 1; v < n; ++v) if (reach[v]) {
      int best = -1, best_depth = -1;
      for (int x : dom[v]) if (x != v && (int)dom[x].size() > best_depth)
        best = x, best_depth = dom[x].size();
      want[v] = best + 1;
    }
    vector<int> got(n + 1);
    for (int u = 1; u <= n; ++u) for (int v : d.tree[u]) {
      if (got[v]) fail(tc, n, e, "dominator tree has multiple parents");
      got[v] = u;
    }
    for (int v = 0; v < n; ++v) {
      int expected = reach[v] ? (v == 0 ? 0 : want[v]) : 0;
      if (got[v + 1] != expected) fail(tc, n, e, "immediate dominator differs");
    }
  }
  cout << "Dominator_Tree PASS 5000 randomized directed multigraph cases\n";
}
