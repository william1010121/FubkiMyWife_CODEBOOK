struct Cent_Dec { // 1-base; build O(n log n), modify/query O(log n)
  // Connected weighted tree, 1 <= n < N; N supplied; stack for depth.
  // init, edges, build, then modify/query; all ll intermediates fit.
  vector<pll> G[N];
  pll info[N], upinfo[N]; // (sum distance, count); subtract same branch
  int n, pa[N], layer[N], sz[N], done[N];
  ll dis[__lg(N) + 1][N];
  void init(int _n) {
    n = _n, layer[0] = -1;
    fill_n(pa + 1, n, 0), fill_n(done + 1, n, 0);
    fill_n(info, n + 1, pll()), fill_n(upinfo, n + 1, pll());
    for (int i = 1; i <= n; ++i) G[i].clear();
  }
  void add_edge(int a, int b, int w) {
    G[a].pb({b, w}), G[b].pb({a, w});
  }
  void get_cent(int u, int f, int &mx, int &c, int num) {
    int mxsz = 0; sz[u] = 1;
    for (auto [v, w] : G[u]) if (!done[v] && v != f) {
      get_cent(v, u, mx, c, num);
      sz[u] += sz[v], mxsz = max(mxsz, sz[v]);
    }
    if (mx > (mxsz = max(mxsz, num - sz[u]))) mx = mxsz, c = u;
  }
  void dfs(int u, int f, ll d, int org) {
    dis[layer[org]][u] = d;
    for (auto [v, w] : G[u])
      if (!done[v] && v != f) dfs(v, u, d + w, org);
  }
  int cut(int u, int f, int num) {
    int mx = num + 1, c = 0;
    get_cent(u, f, mx, c, num);
    done[c] = 1, pa[c] = f, layer[c] = layer[f] + 1;
    dfs(c, f, 0, c); // fill distances before cutting child components
    for (auto [v, w] : G[c]) if (!done[v])
      upinfo[cut(v, c, sz[v] > sz[c] ? num - sz[c] : sz[v])] = pll();
    return done[c] = 0, c;
  }
  void build() { cut(1, 0, n); }
  void modify(int u) { // mark one copy of u
    for (int a = u, ly = layer[a]; a; a = pa[a], --ly) {
      info[a].X += dis[ly][u], ++info[a].Y;
      if (pa[a]) upinfo[a].X += dis[ly - 1][u], ++upinfo[a].Y;
    }
  }
  ll query(int u) { // sum path weights to marked copies; signed edges OK
    ll rt = 0;
    for (int a = u, ly = layer[a]; a; a = pa[a], --ly) {
      rt += info[a].X + info[a].Y * dis[ly][u];
      if (pa[a]) rt -= upinfo[a].X + upinfo[a].Y * dis[ly - 1][u];
    }
    return rt;
  }
};
