ll road[N][N]; // ll INF>0; absent=INF; |walk sums, <=n edges|<INF
struct MinimumMeanCycle { // init 0<=n<=N; fill active road entries
  ll dp[N + 1][N]; int n; // large object: static or ample storage
  static bool less_frac(pll a, pll b) { // positive denominators
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
    pll ans = {-1, -1}; // O(VE+V^2); dp[n]-dp[k] must fit ll
    for (int v = 0; v < n; ++v) if (dp[n][v] < INF) {
      pll high = {dp[n][v], n}; // shorter suffixes are reachable
      for (int k = 1; k < n; ++k) {
        pll cur = {dp[n][v] - dp[k][v], n - k};
        if (less_frac(high, cur)) high = cur;
      }
      if (ans.second < 0 || less_frac(high, ans)) ans = high;
    }
    if (ans.second < 0) return ans;
    auto [p, q] = ans; ll g = gcd(p, q); return {p / g, q / g};
  }
  void init(int _n) { n = _n; }
};
