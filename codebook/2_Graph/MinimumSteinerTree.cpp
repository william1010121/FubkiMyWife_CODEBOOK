struct SteinerTree { // 0-base, nonnegative edge/vertex costs
  int n, dst[N][N], dp[1 << T][N], tdst[N], vcst[N];
  void init(int _n) {
    n = _n;
    for (int i = 0; i < n; ++i)
      fill_n(dst[i], n, INF), dst[i][i] = vcst[i] = 0;
  }
  void chmin(int &x, int val) { x = min(x, val); }
  void add_edge(int u, int v, int w) { chmin(dst[u][v], w); }
  void shortest_path() { // charge each entered vertex; call once per init
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
        if (i != j && dst[i][j] < INF) dst[i][j] += vcst[j];
    for (int k = 0; k < n; ++k)
      for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
          chmin(dst[i][j], dst[i][k] + dst[k][j]);
  }
  int solve(const vector<int>& ter) { // calls shortest_path; <= T distinct
    vector<int> terminals = ter;
    sort(terminals.begin(), terminals.end());
    terminals.erase(unique(terminals.begin(), terminals.end()),
      terminals.end());
    if (terminals.empty()) return 0;
    shortest_path();
    int t = SZ(terminals), full = (1 << t) - 1;
    for (int m = 0; m <= full; ++m) fill_n(dp[m], n, INF);
    copy_n(vcst, n, dp[0]);
    for (int msk = 1; msk <= full; ++msk) {
      if (!(msk & (msk - 1))) {
        int u = terminals[__lg(msk)];
        for (int i = 0; i < n; ++i) dp[msk][i] = vcst[u] + dst[u][i];
        continue; // singleton paths are already closed
      }
      for (int sub = (msk - 1) & msk; sub; sub = (sub - 1) & msk)
        if (sub < (msk ^ sub)) // merge each unordered split once
          for (int i = 0; i < n; ++i)
            chmin(dp[msk][i], dp[sub][i] + dp[msk ^ sub][i] - vcst[i]);
      for (int i = 0; i < n; ++i) {
        tdst[i] = INF;
        for (int j = 0; j < n; ++j)
          chmin(tdst[i], dp[msk][j] + dst[j][i]);
      }
      copy_n(tdst, n, dp[msk]);
    }
    return *min_element(dp[full], dp[full] + n);
  }
}; // O(V^3 + V 3^T + V^2 2^T); sums fit int, finite cost < INF
