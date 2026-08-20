ll road[N][N]; // input here
struct MinimumMeanCycle {
  ll dp[N + 5][N], n;
  pll solve() {
    ll a = 0, b = 1, L = n + 1; bool found = false;
    for (int i = 2; i <= L; ++i)
      for (int k = 0; k < n; ++k)
        for (int j = 0; j < n; ++j)
          if (dp[i - 1][k] < INF && road[k][j] < INF)
            dp[i][j] = min(dp[i - 1][k] + road[k][j], dp[i][j]);
    for (int i = 0; i < n; ++i) {
      if (dp[L][i] >= INF) continue;
      ll ta = 0, tb = 0;
      for (int j = 1; j < n; ++j)
        if (dp[j][i] < INF &&
          (!tb || ta * (L - j) < (dp[L][i] - dp[j][i]) * tb))
          ta = dp[L][i] - dp[j][i], tb = L - j;
      if (!tb) continue;
      if (!found || a * tb > ta * b) a = ta, b = tb, found = true;
    } if (found) {
      ll g = std::gcd(a < 0 ? -a : a, b);
      return {a / g, b / g};
    } return {-1, -1};
  }
  void init(int _n) {
    n = _n; for (int i = 1; i <= n + 1; ++i)
      fill_n(dp[i], n, i == 1 ? 0 : INF);
  }
};
