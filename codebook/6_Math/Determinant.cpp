// 0<=n=m<=MAXN, entries in [0,P), P>0; overwrites M.
struct Matrix {
  int n, m; ll M[MAXN][MAXN];
  int row_swap(int i, int j) { if (i == j) return 0;
    swap_ranges(M[i], M[i] + m, M[j]); return 1;
  }
  ll det() { // Euclidean; composite P works. O(n^3 max(1,log P)).
    ll rt = 1 % P;
    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) while (M[j][i]) {
        ll q = M[i][i] / M[j][i]; // q=1 needs no remainder.
        if (q) for (int k = i; k < m; ++k)
          M[i][k] = q == 1 ? (M[i][k] >= M[j][k] ?
            M[i][k] - M[j][k] : P - M[j][k] + M[i][k]) :
            P <= INT_MAX ? (M[j][k] * (P-q) + M[i][k]) % P :
            ((__int128)M[j][k] * (P-q) + M[i][k]) % P;
        // Earlier columns in both rows are zero.
        swap_ranges(M[i]+i, M[i]+m, M[j]+i), rt = rt ? P - rt : 0;
      }
      if (!M[i][i]) return 0;
      rt = (__int128)rt * M[i][i] % P;
    }
    return rt; }
};
