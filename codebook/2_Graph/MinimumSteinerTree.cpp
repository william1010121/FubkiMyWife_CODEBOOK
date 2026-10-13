struct SteinerTree { // 0-base, 0 <= n <= N, t <= T < 31
  // 0 <= edge/vertex costs < INF <= INT_MAX; INF > 0
  // Directed common sink; solve once; no shortest_path
  int n, dst[N][N], dp[1 << T][N], tdst[N], vcst[N];
  void init(int _n) {
    n = _n; for (int i = 0; i < n; ++i)
      fill_n(dst[i], n, INF), dst[i][i] = vcst[i] = 0;
  }
  void chmin(int &x, ll val) { x = min<ll>(x, val); }
  void add_edge(int u, int v, int w) {
    chmin(dst[u][v], w); }
  void shortest_path() { // entered vertex cost; once/init
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
        if (i != j) dst[i][j] =
          min<ll>(INF, (ll)dst[i][j] + vcst[j]);
    for (int k = 0; k < n; ++k)
      for (int i = 0; i < n; ++i) if (dst[i][k] < INF)
        for (int j = 0; j < n; ++j)
          chmin(dst[i][j], (ll)dst[i][k] + dst[k][j]);
  }
  int solve(const vector<int>& ter) { // <= T distinct
    vector<int> ts = ter; sort(ALL(ts));
    ts.erase(unique(ALL(ts)), ts.end());
    if (ts.empty()) return 0;
    shortest_path();
    int t = SZ(ts), full = (1 << t) - 1;
    copy_n(vcst, n, dp[0]);
    for (int msk = 1; msk <= full; ++msk) {
      fill_n(dp[msk], n, INF); int rest = msk & (msk - 1);
      if (!rest) { int u = ts[__lg(msk)];
        dp[msk][u] = vcst[u]; } // singleton, then close
      for (int sub = rest; sub; sub = (sub - 1) & rest)
        for (int i = 0; i < n; ++i) // low bit in msk^sub
          chmin(dp[msk][i], (ll)dp[sub][i] +
                dp[msk ^ sub][i] - vcst[i]);
      fill_n(tdst, n, INF);
      for (int j = 0; j < n; ++j) if (dp[msk][j] < INF)
        for (int i = 0; i < n; ++i)
          chmin(tdst[i], (ll)dp[msk][j] + dst[j][i]);
      copy_n(tdst, n, dp[msk]);
    }
    return *min_element(dp[full], dp[full] + n);
  }
}; // O(V^3+V 3^T+V^2 2^T); min(cost, INF), empty: 0
