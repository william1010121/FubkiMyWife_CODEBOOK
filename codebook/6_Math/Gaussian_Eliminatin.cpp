// Rectangular: 0<=n<=w<=INT_MAX; pivot cols <n. O(n^2*w).
// Max-abs pivot; eps=1e-12 scale-sensitive; finite math.
void GAS(vector<vector<double>> &vc) {
  int len = vc.size(), row = 0;
  for (int col = 0; col < len && row < len; ++col) {
    int idx = row;
    for (int j = row + 1; j < len; ++j)
      if (fabs(vc[j][col]) > fabs(vc[idx][col])) idx = j;
    if (fabs(vc[idx][col]) < 1e-12) continue;
    swap(vc[idx], vc[row]); double pivot = vc[row][col];
    for (double &x : vc[row]) x /= pivot;
    for (int j = 0; j < len; ++j) {
      if (j == row || fabs(vc[j][col]) < 1e-12) continue;
      double mul = vc[j][col];
      for (int k = 0; k < (int)vc[j].size(); ++k)
        vc[j][k] -= mul * vc[row][k];
    } ++row;
  }
}
