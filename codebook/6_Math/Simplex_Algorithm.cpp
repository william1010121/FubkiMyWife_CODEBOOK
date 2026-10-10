const int MAXN = 11000, MAXM = 405;
const double eps = 1E-10;
double a[MAXN][MAXM], b[MAXN], c[MAXM], d[MAXN][MAXM], x[MAXM];
int ix[MAXN + MAXM];
// max cx: Ax<=b, x>=0; 0-indexed; n constraints, m variables.
// n,m>=0; n+2<=MAXN,m+2<=MAXM; x[] optimal on success.
// -1: infeasible/unbounded or valid optimum; well-scaled arithmetic.
// Finite arithmetic; absolute eps; O((n+1)(m+1))/pivot, exponential.
double simplex(int n, int m) {
  ++m; fill_n(d[n], m + 1, 0), fill_n(d[n + 1], m + 1, 0);
  iota(ix, ix + n + m, 0); int r = n, s = m - 1;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m - 1; ++j) d[i][j] = -a[i][j];
    d[i][m - 1] = 1, d[i][m] = b[i];
    if (d[r][m] > d[i][m]) r = i;
  }
  copy_n(c, m - 1, d[n]); d[n + 1][m - 1] = -1;
  for (double dd;;) {
    if (r < n) {
      swap(ix[s], ix[r + m]); d[r][s] = 1 / d[r][s];
      for (int j = 0; j <= m; ++j)
        if (j != s) d[r][j] *= -d[r][s];
      for (int i = 0; i <= n + 1; ++i) if (i != r && d[i][s]) {
        for (int j = 0; j <= m; ++j)
          if (j != s) d[i][j] += d[r][j] * d[i][s];
        d[i][s] *= d[r][s];
      }
    }
    r = s = -1; // Bland's rule: smallest variable index.
    for (int j = 0; j < m; ++j) if (s < 0 || ix[s] > ix[j])
      if (d[n + 1][j] > eps ||
          (d[n + 1][j] > -eps && d[n][j] > eps)) s = j;
    if (s < 0) break;
    for (int i = 0; i < n; ++i) if (d[i][s] < -eps)
      if (r < 0 || (dd = d[r][m] / d[r][s] -
          d[i][m] / d[i][s]) < -eps ||
          (dd < eps && ix[r + m] > ix[i + m])) r = i;
    if (r < 0) return -1;
  }
  if (d[n + 1][m] < -eps) return -1;
  double ans = 0; fill_n(x, m, 0);
  for (int i = m; i < n + m; ++i) if (ix[i] < m - 1)
    ans += d[i - m][m] * c[ix[i]], x[ix[i]] = d[i - m][m];
  return ans;
}
