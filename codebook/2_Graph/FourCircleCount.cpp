long long count_four_cycles(int _n, const vector<pair<int, int>> &edges) {
  n = _n, m = edges.size(), total = 0;
  for (int i = 0; i <= n; ++i) E[i].clear(), E1[i].clear(), deg[i] = cnt[i] = 0;
  for (auto [u, v] : edges) {
    E[u].push_back(v);
    E[v].push_back(u);
    deg[u]++, deg[v]++;
  } for (int u = 1; u <= n; u++)
    for (int v : E[u])
      if (deg[u] > deg[v] || (deg[u] == deg[v] && u > v)) E1[u].push_back(v);
  for (int a = 1; a <= n; a++) {
    for (int b : E1[a])
      for (int c : E[b]) {
        if (deg[a] < deg[c] || (deg[a] == deg[c] && a <= c)) continue;
        total += cnt[c]++;
      } for (int b : E1[a])
      for (int c : E[b]) cnt[c] = 0;
  } return total;
}

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  cin >> n >> m;
  vector<pair<int, int>> edges(m);
  for (auto &[u, v] : edges) cin >> u >> v;
  cout << count_four_cycles(n, edges) << '\n';
}
