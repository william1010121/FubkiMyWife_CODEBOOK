struct Heavy_light_Decomposition { // 1-base, root 1; needs Seg
  // Build O(n log n), path sum O(log^2 n); positions 0-base.
  int n, ulink[N], deep[N], mxson[N], w[N], pa[N];
  int t, pl[N], val[N]; // val: vertex data
  Seg seg;
  vector<int> G[N];
  void init(int _n) {
    n = _n, w[0] = 0;
    for (int i = 1; i <= n; ++i) G[i].clear(), mxson[i] = 0;
  }
  void add_edge(int a, int b) { G[a].pb(b), G[b].pb(a); }
  void dfs(int u, int f, int d) {
    w[u] = 1, pa[u] = f, deep[u] = d;
    for (int i : G[u]) if (i != f) {
      dfs(i, u, d + 1), w[u] += w[i];
      if (w[i] > w[mxson[u]]) mxson[u] = i;
    }
  }
  void cut(int u, int link) {
    pl[u] = t++, ulink[u] = link;
    seg.update(pl[u], pl[u] + 1, val[u]);
    if (!mxson[u]) return;
    cut(mxson[u], link);
    for (int i : G[u])
      if (i != pa[u] && i != mxson[u]) cut(i, i);
  }
  void build() { t = 0, seg = Seg(n), dfs(1, 1, 1), cut(1, 1); }
  int query(int a, int b) {
    int ta = ulink[a], tb = ulink[b], res = 0;
    while (ta != tb) {
      if (deep[ta] > deep[tb]) swap(ta, tb), swap(a, b);
      res += seg.query(pl[tb], pl[b] + 1), tb = ulink[b = pa[tb]];
    }
    if (pl[a] > pl[b]) swap(a, b);
    return res + seg.query(pl[a], pl[b] + 1);
  }
};
