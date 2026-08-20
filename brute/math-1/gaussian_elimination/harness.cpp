#include <bits/stdc++.h>
using namespace std;
template<class T> using V = vector<T>;
#include "../../../codebook/6_Math/Gaussian_Eliminatin.cpp"

long long det(vector<vector<long long>> a) {
  int n = (int)a.size();
  vector<int> p(n);
  iota(p.begin(), p.end(), 0);
  long long ans = 0;
  do {
    int inv = 0;
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
      inv += p[i] > p[j];
    long long term = (inv & 1) ? -1 : 1;
    for (int i = 0; i < n; ++i) term *= a[i][p[i]];
    ans += term;
  } while (next_permutation(p.begin(), p.end()));
  return ans;
}
int main() {
  mt19937 rng(712367);
  for (int n = 1; n <= 5; ++n) for (int tc = 0; tc < 500; ++tc) {
    vector<vector<long long>> z(n, vector<long long>(n));
    do {
      for (auto &row : z) for (auto &x : row) x = (int)(rng() % 7) - 3;
    } while (det(z) == 0);
    vector<vector<double>> a(n, vector<double>(n + 1));
    vector<long long> rhs(n);
    for (int i = 0; i < n; ++i) {
      rhs[i] = (int)(rng() % 13) - 6;
      for (int j = 0; j < n; ++j) a[i][j] = z[i][j];
      a[i][n] = rhs[i];
    }
    long long D = det(z);
    GAS(a);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j)
        if (fabs(a[i][j] - (i == j)) > 1e-8) {
          cerr << "GAS identity mismatch n=" << n << " tc=" << tc << '\n';
          return 1;
        }
      auto repl = z;
      for (int k = 0; k < n; ++k) repl[k][i] = rhs[k];
      long long want_num = det(repl);
      if (fabs(a[i][n] - (double)want_num / D) > 1e-8) {
        cerr << "GAS solution mismatch n=" << n << " tc=" << tc
             << " variable=" << i << " got=" << setprecision(17) << a[i][n]
             << " want=" << (double)want_num / D << " D=" << D << '\n';
        return 1;
      }
    }
  }
  cout << "PASS Gaussian_Eliminatin unique square systems n<=5\n";
}
