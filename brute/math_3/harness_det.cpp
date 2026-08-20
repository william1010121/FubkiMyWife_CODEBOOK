#include "common.hpp"
namespace tested {
const int MAXN = 8;
const ll P = 1000000007LL;
#include "../../codebook/6_Math/Determinant.cpp"
}
static ll det_brute(vector<vector<ll>> a) {
  int n = a.size(); if (!n) return 1;
  if (n == 1) return (a[0][0] % tested::P + tested::P) % tested::P;
  ll ans = 0;
  for (int j = 0; j < n; ++j) {
    vector<vector<ll>> b(n - 1, vector<ll>(n - 1));
    for (int r = 1; r < n; ++r) for (int c = 0, cc = 0; c < n; ++c) if (c != j) b[r - 1][cc++] = a[r][c];
    ll term = a[0][j] * det_brute(b) % tested::P;
    if (j & 1) ans = (ans - term) % tested::P; else ans = (ans + term) % tested::P;
  }
  return (ans + tested::P) % tested::P;
}
int main() {
  mt19937 rng(91);
  for (int tc = 0; tc < 400; ++tc) {
    int n = 1 + rng() % 6; vector<vector<ll>> a(n, vector<ll>(n));
    tested::Matrix M; M.n = M.m = n;
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) M.M[i][j] = a[i][j] = rng() % 17;
    require_ok(M.det() == det_brute(a), "determinant");
  }
  cout << "determinant: OK\n";
}
