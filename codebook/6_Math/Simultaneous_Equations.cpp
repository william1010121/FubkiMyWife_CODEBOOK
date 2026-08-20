struct matrix { //m variables, n equations
  int n, m;
  fraction M[MAXN][MAXN + 1], sol[MAXN];
	int solve() { //-1: inconsistent, >= 0: rank
		int rank = 0;
	    for (int col = 0; col < m && rank < n; ++col) { int row = rank; while (row < n && !M[row][col].n) ++row;
      if (row == n) continue;
      for (int k = 0; k <= m; ++k) swap(M[rank][k], M[row][k]);
		for (int j = 0; j < n; ++j) {
			if (rank == j) continue; fraction tmp = -M[j][col] / M[rank][col];
			for (int k = 0; k <= m; ++k) M[j][k] = tmp * M[rank][k] + M[j][k];
      }
		++rank; }
	for (int i = 0; i < n; ++i) { int piv = 0; while (piv < m && !M[i][piv].n) ++piv; if (piv == m && M[i][m].n) return -1; }
	fill_n(sol, m, fraction()); for (int i = 0; i < n; ++i) { int piv = 0; while (piv < m && !M[i][piv].n) ++piv; if (piv < m) sol[piv] = M[i][m] / M[i][piv]; }
	return rank; }
};
