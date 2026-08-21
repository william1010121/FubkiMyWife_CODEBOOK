struct Graph { // 0-base, exact minimum-weight perfect matching
  int n; ll edge[N][N];
  void init(int _n) {
    n = _n;
    for (int i = 0; i < n; ++i) fill_n(edge[i], n, 0);
  }
  void add_edge(int u, int v, ll w) { edge[u][v] = edge[v][u] = w; }
  ll solve() {
    int full = (1 << n) - 1;
    vector<ll> dp(1 << n, LLONG_MAX);
    dp[0] = 0;
    for (int mask = 0; mask <= full; ++mask) {
      if (dp[mask] == LLONG_MAX || mask == full) continue;
      int u = __builtin_ctz((unsigned)(full ^ mask));
      for (int v = u + 1; v < n; ++v) if (!(mask >> v & 1)) {
        int nxt = mask | (1 << u) | (1 << v);
        dp[nxt] = min(dp[nxt], dp[mask] + edge[u][v]);
      }
    }
    return dp[full];
  }
};
