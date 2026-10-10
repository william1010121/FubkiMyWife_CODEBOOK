ll road[N][N]; // input; INF if absent, |walk sums| < INF
struct MinimumMeanCycle {
  ll dp[N + 1][N]; int n;
  static bool less_frac(pll a, pll b) {
    return (__int128)a.first * b.second < (__int128)b.first * a.second;
  }
  pll solve() {
    vector<tuple<int, int, ll>> edges;
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
        if (road[u][v] < INF) edges.emplace_back(u, v, road[u][v]);
    fill_n(dp[0], n, 0); // every vertex may be the start
    for (int k = 1; k <= n; ++k) {
      fill_n(dp[k], n, INF);
      for (auto [u, v, w] : edges)
        if (dp[k - 1][u] < INF)
          dp[k][v] = min(dp[k][v], dp[k - 1][u] + w);
    }
    pll ans = {-1, -1}; // O(VE+V^2); differences must fit ll
    for (int v = 0; v < n; ++v) if (dp[n][v] < INF) {
      pll high = {dp[n][v], n}; // k=0 is always reachable
      for (int k = 1; k < n; ++k) if (dp[k][v] < INF) {
        pll cur = {dp[n][v] - dp[k][v], n - k};
        if (less_frac(high, cur)) high = cur;
      }
      if (ans.second < 0 || less_frac(high, ans)) ans = high;
    }
    if (ans.second < 0) return ans;
    ll g = gcd(ans.first, ans.second);
    return {ans.first / g, ans.second / g};
  }
  void init(int _n) { n = _n; }
};
