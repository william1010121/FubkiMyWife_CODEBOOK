struct Clique_Cover { // 0-base, O(n2^n); may overestimate (mod 2^32)
  // Simple undirected, valid endpoints; 0<=n<=N<31; feasible arrays.
  int co[1 << N], n, E[N];
  unsigned dp[1 << N]; // 32-bit unsigned; OR-zeta fwt(int*,int,int).
  void init(int _n) { n = _n; fill_n(E, n, 0); }
  void add_edge(int u, int v) { E[u] |= 1 << v, E[v] |= 1 << u; }
  int solve() {
    int m = 1 << n;
    co[0] = 1, dp[0] = (n & 1) ? 1 : -1u;
    for (int i = 1; i < m; ++i) {
      int t = i & -i, s = i ^ t;
      dp[i] = -dp[s];
      co[i] = co[s] && (E[__builtin_ctz(t)] & s) == s;
    }
    fwt(co, m, 1); // OR subset-sum (zeta) transform
    for (int ans = 1; ans < n; ++ans) {
      unsigned sum = 0;
      for (int i = 0; i < m; ++i) sum += (dp[i] *= co[i]);
      if (sum) return ans;
    }
    return n;
  }
};
