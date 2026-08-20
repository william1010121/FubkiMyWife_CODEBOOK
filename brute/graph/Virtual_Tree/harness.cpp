#include <bits/stdc++.h>
using namespace std;
#define N 64
#define pb push_back
#define ALL(x) (x).begin(), (x).end()
vector<int> base[N];
int par[N], dep[N], dfn[N], timer_dfs;

int LCA(int u, int v) {
  while (dep[u] > dep[v]) u = par[u];
  while (dep[v] > dep[u]) v = par[v];
  while (u != v) u = par[u], v = par[v];
  return u;
}

void dfs(int u, int p) {
  par[u] = p, dfn[u] = timer_dfs++;
  for (int v : base[u]) if (v != p) dep[v] = dep[u] + 1, dfs(v, u);
}

#include "../../../codebook/2_Graph/Virtual_Tree.cpp"

static void fail(int tc, int n, const vector<int> &keys, const string &why) {
  cerr << "Virtual_Tree failure tc=" << tc << ": " << why << "\n";
  cerr << n << " keys:";
  for (int x : keys) cerr << " " << x;
  cerr << "\n";
  exit(1);
}

int main() {
  mt19937 rng(0x77E); // deterministic tree/key generation
  for (int tc = 0; tc < 5000; ++tc) {
    int n = 1 + rng() % 30;
    for (int i = 0; i < n; ++i) base[i].clear(), vG[i].clear();
    for (int i = 1; i < n; ++i) {
      int p = rng() % i;
      base[i].pb(p), base[p].pb(i);
    }
    dep[0] = 0, timer_dfs = 0, dfs(0, 0);
    int k = 1 + rng() % n;
    vector<int> keys;
    vector<int> order(n); iota(ALL(order), 0); shuffle(ALL(order), rng);
    keys.assign(order.begin(), order.begin() + k);
    auto original = keys;

    set<int> nodes(keys.begin(), keys.end());
    for (int i = 0; i < k; ++i) for (int j = i + 1; j < k; ++j)
      nodes.insert(LCA(keys[i], keys[j]));
    int root = -1;
    set<pair<int,int>> want;
    for (int u : nodes) {
      if (u == 0) { root = 0; continue; }
      int x = par[u];
      while (!nodes.count(x) && x != par[x]) x = par[x];
      if (nodes.count(x)) want.insert({x, u});
      else root = u;
    }
    int got_root = build(keys);
    if (got_root != root) fail(tc, n, original, "root differs");
    set<pair<int,int>> got;
    for (int u : nodes) for (int v : vG[u]) got.insert({u, v});
    if (got != want) fail(tc, n, original, "virtual edges differ");
    reset(got_root);
    for (int i = 0; i < n; ++i) if (!vG[i].empty())
      fail(tc, n, original, "reset left stale virtual edges");
  }
  vector<int> empty;
  if (build(empty) != -1) return 1;
  cout << "Virtual_Tree PASS 5000 randomized rooted-tree key sets\n";
}
