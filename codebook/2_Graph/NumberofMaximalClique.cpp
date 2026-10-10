struct BronKerbosch { // 1-base, 0 <= n < N; count fits int
  // Simple undirected; enough stack; n=0 empty clique; GCC/libstdc++.
  int n, S; bitset<N> g[N];
  void init(int _n) {
    n = _n; for (int i = 1; i <= n; ++i) g[i].reset();
  }
  void add_edge(int u, int v) { g[u][v] = g[v][u] = 1; }
  void dfs(bitset<N> P, bitset<N> X) { // initialize S; active P/X disjoint
    if (P.none()) { if (X.none()) ++S; return; }
    auto U = P | X; int pivot = 0, best = -1, bound = P.count() - X.none();
    for (int u = U._Find_first(); u < N; u = U._Find_next(u)) {
      int score = (P & g[u]).count();
      if (score > best) best = score, pivot = u;
      if (score == bound) break; // Loopless: |P| - (X empty).
    }
    auto cand = P & ~g[pivot];
    for (int v = cand._Find_first(); v < N; v = cand._Find_next(v)) {
      dfs(P & g[v], X & g[v]);
      P[v] = 0, X[v] = 1;
    }
  }
  void dfs_vec(vector<int> P, vector<int> X) { // same contract as dfs
    bitset<N> p, x; for (int v : P) p[v] = 1;
    for (int v : X) x[v] = 1; dfs(p, x);
  }
  int solve() {
    bitset<N> P; for (int i = 1; i <= n; ++i) P[i] = 1;
    S = 0, dfs(P, {}); return S;
  }
}; // Pivot/intersections: O(n ceil(N/word_bits) 3^(n/3)).
