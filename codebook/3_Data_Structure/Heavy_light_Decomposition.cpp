struct Heavy_light_Decomposition { // 1-base, root 1
  // Needs Seg, N; connected tree; 1<=n<N; stack for depth.
  // init, edges, all val, build, query; int math fits.
  // Build O(n), path sum O(log^2 n); positions 0-base.
  int n, ulink[N], deep[N], mxson[N], w[N], pa[N];
  int t, pl[N], val[N]; // val: vertex data
  Seg seg; vector<int> G[N];
  void init(int _n) {
    n = _n, w[0] = 0;
    for (int i = 1; i <= n; ++i)
      G[i].clear(), mxson[i] = 0;
  }
  void add_edge(int a, int b) { G[a].pb(b), G[b].pb(a); }
  void dfs(int u, int f, int d) {
    w[u] = 1, pa[u] = f, deep[u] = d;
    for (int i : G[u]) if (i != f) {
      dfs(i, u, d + 1), w[u] += w[i];
      if (w[i] > w[mxson[u]]) mxson[u] = i;
    }
  }
  void cut_impl(int u, int link, bool values) {
    pl[u] = t++, ulink[u] = link;
    if (values) seg.update(pl[u], pl[u] + 1, val[u]);
    if (!mxson[u]) return;
    cut_impl(mxson[u], link, values);
    for (int i : G[u]) if (i != pa[u] && i != mxson[u])
      cut_impl(i, i, values);
  }
  void cut(int u, int link) { cut_impl(u, link, true); }
  void build() {
    t = 0, seg = Seg(n), dfs(1, 1, 1);
    cut_impl(1, 1, false);
    vector<int> a(n);
    for (int u = 1; u <= n; ++u) a[pl[u]] = val[u];
    auto make = [&](auto &&self, int l, int r)
        -> Seg::Node* {
      if (r - l == 1)
        return a[l] ? new Seg::Node{a[l], a[l]} : nullptr;
      int m = l + (r - l) / 2;
      auto x = self(self, l, m), y = self(self, m, r);
      if (!x && !y) return nullptr;
      return new Seg::Node{
        (x ? x->sum : 0) + (y ? y->sum : 0), 0, x, y};
    }; seg.root = make(make, 0, n);
  }
  int query(int a, int b) {
    int ta = ulink[a], tb = ulink[b], res = 0;
    while (ta != tb) {
      if (deep[ta] > deep[tb]) swap(ta, tb), swap(a, b);
      res += seg.query(pl[tb], pl[b] + 1);
      tb = ulink[b = pa[tb]];
    }
    if (pl[a] > pl[b]) swap(a, b);
    return res + seg.query(pl[a], pl[b] + 1);
  }
};
