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
  vector<vector<double>> empty;
  GAS(empty); assert(empty.empty());
  vector<vector<double>> zero(3, vector<double>(5));
  auto unchanged = zero; GAS(zero); assert(zero == unchanged);
  for (double pivot : {1e-13,1e-12,-1e-12}) {
    vector<vector<double>> a = {{pivot,2*pivot}};
    GAS(a);
    if (abs(pivot)<1e-12) assert(a[0][0]==pivot);
    else assert(a[0][0]==1 && a[0][1]==2);
  }
  vector<vector<double>> duplicate = {{0,2,4},{0,2,4}};
  GAS(duplicate);
  assert((duplicate == vector<vector<double>>{{0,1,2},{0,0,0}}));
  // Well-conditioned systems: tiny first candidate, large second pivot.
  // Two RHS columns check the complete rectangular row operation.
  for (double eps : {1e-12,2e-12,1e-10,1e-8,1e-6})
    for (int sign : {-1,1}) for (int order : {0,1}) {
      double t = sign*eps;
      vector<vector<double>> a = {{t,1,1+t,3-2*t},{1,1,2,1}};
      if (order) swap(a[0],a[1]);
      GAS(a);
      for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j)
          assert(abs(a[i][j]-(i==j))<1e-10);
        if (abs(a[i][2]-1)>1e-10 ||
            abs(a[i][3]-(i==0 ? -2 : 3))>1e-10) {
          cerr << "GAS small-pivot regression t=" << t
               << " row=" << i << " order=" << order << '\n';
          return 1;
        }
      }
    }
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
  cout << "PASS Gaussian_Eliminatin square systems, empty/duplicate/"
       << "epsilon cases, small pivots and multiple RHS\n";
}
