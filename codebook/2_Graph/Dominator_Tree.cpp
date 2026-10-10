struct dominator_tree { // 1-base, O((V+E) log V); reachable only
  vector<int> G[N], rG[N], tree[N];
  int n, pa[N], dfn[N], id[N], Time, semi[N], idom[N], best[N];
  void init(int _n) {
    n = _n;
    for (int i = 1; i <= n; ++i) G[i].clear(), rG[i].clear();
  }
  void add_edge(int u, int v) { G[u].pb(v), rG[v].pb(u); }
  void dfs(int u) {
    id[dfn[u] = ++Time] = u;
    for (int v : G[u]) if (!dfn[v]) dfs(v), pa[dfn[v]] = dfn[u];
  }
  int find(int y, int x) {
    if (y <= x) return y;
    int tmp = find(pa[y], x);
    if (semi[best[y]] > semi[best[pa[y]]]) best[y] = best[pa[y]];
    return pa[y] = tmp;
  }
  void tarjan(int root) {
    Time = 0;
    for (int i = 1; i <= n; ++i)
      dfn[i] = idom[i] = 0, tree[i].clear(), best[i] = semi[i] = i;
    dfs(root);
    for (int i = Time; i > 1; --i) {
      for (int v : rG[id[i]]) if (int w = dfn[v]) {
        find(w, i), semi[i] = min(semi[i], semi[best[w]]);
      }
      tree[semi[i]].pb(i);
      for (int v : tree[pa[i]]) {
        find(v, pa[i]);
        idom[v] = semi[best[v]] == pa[i] ? pa[i] : best[v];
      }
      tree[pa[i]].clear();
    }
    for (int i = 2; i <= Time; ++i) {
      if (idom[i] != semi[i]) idom[i] = idom[idom[i]];
      tree[id[idom[i]]].pb(id[i]);
    }
  }
};
