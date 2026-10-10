ll road[N][N]; // N>0; ll INF>0; absent=INF; |walk sums, <=n edges|<INF
struct MinimumMeanCycle { // init 0<=n<=N; fill active road entries
  ll dp[N + 1][N]; int n; // large object: static or ample storage
  static bool less_frac(pll a, pll b) { // positive denominators
    return (__int128)a.first * b.second < (__int128)b.first * a.second; }
  pll solve() {
    vector<vector<pair<int, ll>>> edges(n);
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
        if (road[u][v] < INF) edges[u].emplace_back(v, road[u][v]);
    fill_n(dp[0], n, 0); // every vertex may be the start
    for (int k = 1; k <= n; ++k) {
      fill_n(dp[k], n, INF);
      for (int u = 0; u < n; ++u) if (ll d = dp[k - 1][u]; d < INF)
        for (auto [v, w] : edges[u])
          dp[k][v] = min(dp[k][v], d + w);
    }
    vector<pll> high(n); vector<int> active;
    for (int v = 0; v < n; ++v) if (dp[n][v] < INF)
      high[v] = {dp[n][v], n}, active.push_back(v);
    for (int k = 1; k < n; ++k) for (int v : active) {
      pll cur = {dp[n][v] - dp[k][v], n - k};
      if (less_frac(high[v], cur)) high[v] = cur;
    }
    pll ans = {-1, -1}; // O(VE+V^2); dp[n]-dp[k] must fit ll
    for (int v : active)
      if (ans.second < 0 || less_frac(high[v], ans)) ans = high[v];
    if (ans.second < 0) return ans;
    auto [p, q] = ans; ll g = gcd(p, q); return {p / g, q / g}; }
  void init(int _n) { n = _n; }
};
