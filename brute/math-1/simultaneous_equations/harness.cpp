#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using std::gcd;
#include "../../../codebook/6_Math/Fraction.cpp"
#define MAXN 8
#include "../../../codebook/6_Math/Simultaneous_Equations.cpp"

struct Rat {
  ll n = 0, d = 1;
  Rat() = default;
  Rat(ll n_, ll d_ = 1) : n(n_), d(d_) { fix(); }
  void fix() { ll g = gcd(n < 0 ? -n : n, d < 0 ? -d : d); n /= g; d /= g; if (d < 0) n = -n, d = -d; }
  Rat operator+(Rat b) const { return Rat(n*b.d+b.n*d,d*b.d); }
  Rat operator-(Rat b) const { return Rat(n*b.d-b.n*d,d*b.d); }
  Rat operator*(Rat b) const { return Rat(n*b.n,d*b.d); }
  Rat operator/(Rat b) const { return Rat(n*b.d,d*b.n); }
  bool zero() const { return n == 0; }
  bool operator==(const Rat &o) const { return n == o.n && d == o.d; }
};
struct Oracle {
  int rank, status; // status: -1 inconsistent, 0 consistent
  vector<Rat> sol;
};
Oracle solve_exact(vector<vector<int>> a, int m) {
  int n = (int)a.size(), row = 0;
  vector<vector<Rat>> x(n, vector<Rat>(m + 1));
  for (int i = 0; i < n; ++i) for (int j = 0; j <= m; ++j) x[i][j] = a[i][j];
  vector<int> where(m, -1);
  for (int col = 0; col < m && row < n; ++col) {
    int p = row; while (p < n && x[p][col].zero()) ++p;
    if (p == n) continue;
    swap(x[p], x[row]); where[col] = row;
    Rat q = x[row][col]; for (int j = 0; j <= m; ++j) x[row][j] = x[row][j] / q;
    for (int i = 0; i < n; ++i) if (i != row && !x[i][col].zero()) {
      Rat q2 = x[i][col]; for (int j = 0; j <= m; ++j) x[i][j] = x[i][j] - q2*x[row][j];
    }
    ++row;
  }
  for (int i = 0; i < n; ++i) {
    bool lhs = false; for (int j = 0; j < m; ++j) lhs |= !x[i][j].zero();
    if (!lhs && !x[i][m].zero()) return {-1, -1, {}};
  }
  vector<Rat> sol(m);
  for (int j = 0; j < m; ++j) if (where[j] != -1) sol[j] = x[where[j]][m];
  return {row, 0, sol};
}
int main() {
  mt19937 rng(909091);
  for (int n = 1; n <= 4; ++n) for (int m = 1; m <= 4; ++m)
    for (int tc = 0; tc < 600; ++tc) {
      vector<vector<int>> a(n, vector<int>(m + 1));
      for (auto &r : a) for (int &x : r) x = (int)(rng() % 5) - 2;
      Oracle want = solve_exact(a, m);
      matrix got; got.n = n; got.m = m;
      for (int i = 0; i < n; ++i) for (int j = 0; j <= m; ++j) got.M[i][j] = fraction(a[i][j]);
      int rank = got.solve();
      int expected_rank = want.status < 0 ? -1 : want.rank;
      if (rank != expected_rank) {
        cerr << "simultaneous solve classification mismatch n=" << n
             << " m=" << m << " tc=" << tc << " got=" << rank
             << " want=" << (want.status ? -1 : want.rank) << '\n';
        return 1;
      }
      if (rank == m) for (int j = 0; j < m; ++j)
        if (got.sol[j].n != want.sol[j].n || got.sol[j].d != want.sol[j].d) {
          cerr << "simultaneous unique solution mismatch n=" << n
               << " m=" << m << " tc=" << tc << " var=" << j
               << " got=" << got.sol[j].n << '/' << got.sol[j].d
               << " want=" << want.sol[j].n << '/' << want.sol[j].d << '\n';
          return 1;
        }
    }
  cout << "PASS Simultaneous_Equations random exact oracle n,m<=4\n";
}
