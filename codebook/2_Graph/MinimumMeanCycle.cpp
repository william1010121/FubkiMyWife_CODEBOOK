ll road[N][N]; // input here
struct MinimumMeanCycle {
  ll dp[N + 1][N]; int n;
  static bool less_frac(pll a, pll b) {
    return (__int128)a.first * b.second < (__int128)b.first * a.second;
  }
  pll solve() {
    fill_n(dp[0], n, 0); // every vertex may be the start
    for (int k = 1; k <= n; ++k) {
      fill_n(dp[k], n, INF);
      for (int v = 0; v < n; ++v) for (int u = 0; u < n; ++u)
        if (dp[k - 1][u] < INF && road[u][v] < INF)
          dp[k][v] = min(dp[k][v], dp[k - 1][u] + road[u][v]);
    }
    bool found = false; pll ans;
    for (int v = 0; v < n; ++v) if (dp[n][v] < INF) {
      bool have = false; pll high;
      for (int k = 0; k < n; ++k) if (dp[k][v] < INF) {
        pll cur = {dp[n][v] - dp[k][v], n - k};
        if (!have || less_frac(high, cur)) high = cur, have = true;
      }
      if (have && (!found || less_frac(high, ans))) ans = high, found = true;
    }
    if (!found) return {-1, -1};
    ll g = gcd(ans.first, ans.second);
    return {ans.first / g, ans.second / g};
  }
  void init(int _n) { n = _n; }
};
