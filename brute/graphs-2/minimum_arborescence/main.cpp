#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define N 8
#define pb push_back
#define SZ(v) ((int)(v).size())
const ll INF = (1LL << 60);
#include "../../../codebook/2_Graph/Minimum_Arborescence.cpp"

struct Edge { int u, v; ll w; };
static ll oracle(int n, int root, const vector<Edge> &es) {
  vector<vector<int>> incoming(n);
  for (int i = 0; i < (int)es.size(); ++i) if (es[i].v != root)
    incoming[es[i].v].push_back(i);
  for (int v = 0; v < n; ++v) if (v != root && incoming[v].empty()) return -INF;
  ll ans = INF;
  vector<int> pick(n, -1);
  function<void(int)> dfs = [&](int v) {
    while (v < n && v == root) ++v;
    if (v == n) {
      vector<int> state(n);
      ll cost = 0;
      for (int x = 0; x < n; ++x) if (x != root) cost += es[pick[x]].w;
      if (cost >= ans) return;
      for (int x = 0; x < n; ++x) if (x != root) {
        int u = x;
        fill(state.begin(), state.end(), 0);
        while (u != root && !state[u]) state[u] = 1, u = es[pick[u]].u;
        if (u != root) return;
      }
      ans = cost;
      return;
    }
    for (int id : incoming[v]) pick[v] = id, dfs(v + 1);
  };
  dfs(0);
  return ans == INF ? -INF : ans;
}

int main() {
  mt19937 rng(0x4152424F);
  int cases = 0;
  for (int n = 2; n <= 7; ++n) for (int tc = 0; tc < 500; ++tc) {
    int root = rng() % n;
    vector<Edge> es;
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
      if (u != v && (int)(rng() % 100) < 33 && (int)es.size() < 15)
        es.push_back({u, v, (int)(rng() % 15)});
    zhu_liu zl;
    zl.init();
    for (auto e : es) zl.add_edge(e.u, e.v, e.w);
    ll got = zl.build(root, n), want = oracle(n, root, es);
    ++cases;
    if (got != want) {
      cerr << "minimum_arborescence mismatch case=" << cases << " n=" << n
           << " root=" << root << " got=" << got << " want=" << want << '\n';
      return 1;
    }
  }
  cout << "PASS cases=" << cases << " oracle=enumerated-incoming-parent-choices\n";
}
