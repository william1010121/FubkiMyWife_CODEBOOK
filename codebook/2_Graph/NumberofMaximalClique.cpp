struct BronKerbosch { // 1-base, n < N; count must fit int
  int n, S, g[N][N];
  void init(int _n) {
    n = _n; for (int i = 1; i <= n; ++i) fill_n(g[i], n + 1, 0);
  }
  void add_edge(int u, int v) { g[u][v] = g[v][u] = 1; }
  void dfs_vec(vector<int> P, vector<int> X) {
    if (P.empty()) { if (X.empty()) ++S; return; }
    int pivot = -1, best = -1;
    for (auto set : {&P, &X}) for (int u : *set) {
      int score = 0;
      for (int v : P) score += g[u][v];
      if (score > best) best = score, pivot = u;
    }
    for (int i = SZ(P) - 1; i >= 0; --i) {
      int v = P[i];
      if (pivot != -1 && g[pivot][v]) continue;
      vector<int> nP, nX;
      for (int u : P) if (g[v][u]) nP.push_back(u);
      for (int u : X) if (g[v][u]) nX.push_back(u);
      dfs_vec(move(nP), move(nX));
      P.erase(P.begin() + i), X.push_back(v);
    }
  }
  int solve() {
    vector<int> P(n); iota(P.begin(), P.end(), 1);
    S = 0, dfs_vec(move(P), {}); return S;
  }
}; // Bron-Kerbosch with pivot; O(3^(n/3)) search, polynomial overhead
