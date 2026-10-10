// 0<=n=m<=MAXN, entries in [0,P), P>0; overwrites M.
struct Matrix {
  int n, m; ll M[MAXN][MAXN];
  int row_swap(int i, int j) {
    if (i == j) return 0;
    swap_ranges(M[i], M[i] + m, M[j]); return 1;
  }
  ll det() { // Euclidean; composite P works. O(n^3 log P).
    ll rt = 0;
    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) while (M[j][i]) {
        ll q = M[i][i] / M[j][i];
        if (q) for (int k = i; k < m; ++k)
          M[i][k] = ((__int128)M[j][k] * (P - q) + M[i][k]) % P;
        rt += row_swap(i, j);
      }
      if (!M[i][i]) return 0;
    }
    rt = (rt & 1) ? P - 1 : 1 % P;
    for (int i = 0; i < n; ++i) rt = (__int128)rt * M[i][i] % P;
    return rt;
  }
};
