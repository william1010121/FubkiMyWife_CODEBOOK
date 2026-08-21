struct BronKerbosch { // 1-base
  int n, S, g[N][N];
  void init(int _n) {
    n = _n; for (int i = 1; i <= n; ++i) fill_n(g[i], n + 1, 0);
  }
  void add_edge(int u, int v) { g[u][v] = g[v][u] = 1; }
  void dfs_vec(vector<int> P, vector<int> X) {
    if (P.empty() && X.empty()) { ++S; return; }
    int pivot = -1, best = -1;
    vector<int> both = P;
    both.insert(both.end(), X.begin(), X.end());
    for (int u : both) {
      int score = 0;
      for (int v : P) score += g[u][v];
      if (score > best) best = score, pivot = u;
    }
    vector<int> cand;
    for (int v : P) if (pivot == -1 || !g[pivot][v]) cand.push_back(v);
    for (int v : cand) {
      vector<int> nP, nX;
      for (int u : P) if (g[v][u]) nP.push_back(u);
      for (int u : X) if (g[v][u]) nX.push_back(u);
      dfs_vec(nP, nX);
      P.erase(find(P.begin(), P.end(), v));
      X.push_back(v);
    }
  }
  int solve() {
    vector<int> P(n), X; iota(P.begin(), P.end(), 1);
    S = 0, dfs_vec(P, X);
    return S;
  }
};
