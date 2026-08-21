ll road[N][N]; // input here
struct MinimumMeanCycle {
  ll dp[N + 5][N], n;
  static bool less_frac(pll a, pll b) {
    return (__int128)a.first * b.second < (__int128)b.first * a.second;
  }
  static bool greater_frac(pll a, pll b) { return less_frac(b, a); }
  pll solve() {
    vector<vector<int>> g(n), rg(n);
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
      if (road[i][j] < INF) g[i].push_back(j), rg[j].push_back(i);
    vector<int> vis(n), order, comp(n, -1);
    function<void(int)> dfs1 = [&](int u) {
      vis[u] = 1;
      for (int v : g[u]) if (!vis[v]) dfs1(v);
      order.push_back(u);
    };
    for (int i = 0; i < n; ++i) if (!vis[i]) dfs1(i);
    function<void(int, int)> dfs2 = [&](int u, int c) {
      comp[u] = c;
      for (int v : rg[u]) if (comp[v] == -1) dfs2(v, c);
    };
    reverse(order.begin(), order.end());
    int cc = 0;
    for (int u : order) if (comp[u] == -1) dfs2(u, cc++);

    bool found = false; pll ans = {0, 1};
    for (int c = 0; c < cc; ++c) {
      vector<int> vs;
      for (int i = 0; i < n; ++i) if (comp[i] == c) vs.push_back(i);
      if (vs.size() == 1 && road[vs[0]][vs[0]] >= INF) continue;
      int m = vs.size(), source = vs[0];
      for (int k = 0; k <= m; ++k) fill_n(dp[k], n, INF);
      dp[0][source] = 0;
      for (int k = 1; k <= m; ++k)
        for (int v : vs) for (int u : vs)
          if (dp[k - 1][u] < INF && road[u][v] < INF)
            dp[k][v] = min(dp[k][v], dp[k - 1][u] + road[u][v]);
      bool comp_found = false; pll best = {0, 1};
      for (int v : vs) if (dp[m][v] < INF) {
        bool have = false; pll high = {0, 1};
        for (int k = 0; k < m; ++k) if (dp[k][v] < INF) {
          pll cur = {dp[m][v] - dp[k][v], m - k};
          if (!have || greater_frac(cur, high)) high = cur, have = true;
        }
        if (have && (!comp_found || less_frac(high, best)))
          best = high, comp_found = true;
      }
      if (comp_found && (!found || less_frac(best, ans)))
        ans = best, found = true;
    }
    if (!found) return {-1, -1};
    ll g0 = gcd(ans.first < 0 ? -ans.first : ans.first, ans.second);
    return {ans.first / g0, ans.second / g0};
  }
  void init(int _n) {
    n = _n; for (int i = 0; i <= n; ++i) fill_n(dp[i], n, INF);
  }
};
