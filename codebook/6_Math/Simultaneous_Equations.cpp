struct matrix { // All Fraction.cpp intermediates valid; 0<=n,m<=MAXN.
  int n, m;
  fraction M[MAXN][MAXN + 1], sol[MAXN];
  int solve() { // -1: inconsistent; else rank, free variables=0.
    int rank = 0, piv[MAXN];
    for (int col = 0; col < m && rank < n; ++col) {
      int row = rank; while (row < n && !M[row][col].n) ++row;
      if (row == n) continue;
      for (int k = col; k <= m; ++k) swap(M[rank][k], M[row][k]);
      for (int j = 0; j < n; ++j) if (j != rank && M[j][col].n) {
        fraction t = -M[j][col] / M[rank][col]; M[j][col] = {};
        for (int k = col + 1; k <= m; ++k) if (M[rank][k].n)
          M[j][k] = t*M[rank][k]+M[j][k];
      }
      piv[rank++] = col;
    }
    for (int i = rank; i < n; ++i) if (M[i][m].n) return -1;
    fill_n(sol, m, fraction());
    for (int i=0; i<rank; ++i) sol[piv[i]]=M[i][m]/M[i][piv[i]];
    return rank;
  }
};
