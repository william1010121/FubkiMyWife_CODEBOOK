struct Heavy_light_Decomposition { // 1-base
  int n, ulink[N], deep[N], mxson[N], w[N], pa[N];
  int t, pl[N], data[N], val[N]; // val: vertex data
  int seg[4 * N];
  vector<int> G[N];
  void init(int _n) {
    n = _n;
    w[0] = 0;
    for (int i = 1; i <= n; ++i)
      G[i].clear(), mxson[i] = 0;
  }
  void add_edge(int a, int b) { G[a].pb(b), G[b].pb(a); }
  void dfs(int u, int f, int d) {
    w[u] = 1, pa[u] = f, deep[u] = d++;
    for (int &i : G[u])
      if (i != f) {
        dfs(i, u, d), w[u] += w[i];
        if (w[mxson[u]] < w[i]) mxson[u] = i;
      }
  }
  void cut(int u, int link) {
    data[pl[u] = ++t] = val[u], ulink[u] = link;
    if (!mxson[u]) return;
    cut(mxson[u], link);
    for (int i : G[u])
      if (i != pa[u] && i != mxson[u]) cut(i, i);
  }
  void seg_build(int p, int l, int r) {
    if (l == r) return seg[p] = data[l], void();
    int m = (l + r) >> 1;
    seg_build(p << 1, l, m), seg_build(p << 1 | 1, m + 1, r);
    seg[p] = seg[p << 1] + seg[p << 1 | 1];
  }
  int seg_query(int p, int l, int r, int L, int R) {
    if (L <= l && r <= R) return seg[p];
    int m = (l + r) >> 1, res = 0;
    if (L <= m) res += seg_query(p << 1, l, m, L, R);
    if (R > m) res += seg_query(p << 1 | 1, m + 1, r, L, R);
    return res;
  }
  void build() { t = 0, dfs(1, 1, 1), cut(1, 1), seg_build(1, 1, n); }
  int query(int a, int b) {
    int ta = ulink[a], tb = ulink[b], res = 0;
    while (ta != tb) {
      if (deep[ta] > deep[tb]) swap(ta, tb), swap(a, b);
      res += seg_query(1, 1, n, pl[tb], pl[b]);
      tb = ulink[b = pa[tb]];
    } if (pl[a] > pl[b]) swap(a, b);
    return res + seg_query(1, 1, n, pl[a], pl[b]);
  }
};
