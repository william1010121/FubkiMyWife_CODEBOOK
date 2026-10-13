// Held-Karp, O(2^n n^2) time, 8n2^n bytes; 1<=n<=~20.
// d[i][j]: cost i->j, INF if none; 2*INF fits ll.
// cycle: tour from 0 back to 0; else path, any ends.
// {cost, order of n vertices}; {INF, {}} if impossible.
pair<ll, vector<int>> tsp(const vector<vector<ll>> &d,
                          bool cycle = true) {
  int n = SZ(d), F = (1 << n) - 1;
  if (n == 1) return {0, {0}};
  vector<ll> dp(size_t(n) << n, INF); // dp[s*n+v]
  for (int v = 0; v < (cycle ? 1 : n); ++v)
    dp[(1 << v) * n + v] = 0;
  for (int s = 1; s <= F; ++s)
    for (int v = 0; v < n; ++v) if (dp[s*n+v] < INF)
      for (int u = 0; u < n; ++u) if (!(s >> u & 1)) {
        ll &t = dp[(s | 1 << u) * n + u];
        t = min(t, dp[s*n+v] + d[v][u]);
      }
  auto end = [&](int v) {
    return dp[F*n+v] + (cycle ? d[v][0] : 0); };
  int v = 0;
  for (int u = 1; u < n; ++u) if (end(u) < end(v)) v = u;
  ll best = end(v); if (best >= INF) return {INF, {}};
  vector<int> ord = {v};
  for (int s = F, t; (t = s ^ 1 << v); s = t) {
    for (int u = 0; u < n; ++u)
      if ((t >> u & 1) && dp[t*n+u] < INF &&
          dp[t*n+u] + d[u][v] == dp[s*n+v]) {
        v = u; break; }
    ord.pb(v);
  }
  reverse(ALL(ord)); return {best, ord};
}
