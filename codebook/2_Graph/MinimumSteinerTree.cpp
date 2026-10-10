struct SteinerTree { // 0-base, n <= N, t <= T; nonnegative costs
  int n, dst[N][N], dp[1 << T][N], tdst[N], vcst[N];
  void init(int _n) {
    n = _n; for (int i = 0; i < n; ++i)
      fill_n(dst[i], n, INF), dst[i][i] = vcst[i] = 0;
  }
  void chmin(int &x, int val) { x = min(x, val); }
  void add_edge(int u, int v, int w) { chmin(dst[u][v], w); }
  void shortest_path() { // charge each entered vertex; call once per init
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
        if (i != j && dst[i][j] < INF) dst[i][j] += vcst[j];
    for (int k = 0; k < n; ++k)
      for (int i = 0; i < n; ++i) if (dst[i][k] < INF)
        for (int j = 0; j < n; ++j)
          chmin(dst[i][j], dst[i][k] + dst[k][j]);
  }
  int solve(const vector<int>& ter) { // calls shortest_path; <= T distinct
    vector<int> terminals = ter; sort(ALL(terminals));
    terminals.erase(unique(ALL(terminals)), terminals.end());
    if (terminals.empty()) return 0;
    shortest_path();
    int t = SZ(terminals), full = (1 << t) - 1;
    copy_n(vcst, n, dp[0]);
    for (int msk = 1; msk <= full; ++msk) {
      if (!(msk & (msk - 1))) {
        int u = terminals[__lg(msk)];
        for (int i = 0; i < n; ++i) dp[msk][i] = vcst[u] + dst[u][i];
        continue; // singleton paths are already closed
      }
      int rest = msk & (msk - 1); fill_n(dp[msk], n, INF);
      for (int sub = rest; sub; sub = (sub - 1) & rest)
        for (int i = 0; i < n; ++i) // lowest bit stays in complement
          chmin(dp[msk][i], dp[sub][i] + dp[msk ^ sub][i] - vcst[i]);
      fill_n(tdst, n, INF);
      for (int j = 0; j < n; ++j) if (dp[msk][j] < INF)
        for (int i = 0; i < n; ++i)
          chmin(tdst[i], dp[msk][j] + dst[j][i]);
      copy_n(tdst, n, dp[msk]);
    }
    return *min_element(dp[full], dp[full] + n);
  }
}; // O(V^3 + V 3^T + V^2 2^T); sums fit int, finite cost < INF
