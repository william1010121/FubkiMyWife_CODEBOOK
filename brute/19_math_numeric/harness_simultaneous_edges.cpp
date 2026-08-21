#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using std::gcd;

#define MAXN 8
#include "../../codebook/6_Math/Fraction.cpp"
#include "../../codebook/6_Math/Simultaneous_Equations.cpp"

struct rat {
  ll n = 0, d = 1;
  rat() = default;
  rat(ll n_, ll d_ = 1) : n(n_), d(d_) { normalize(); }
  void normalize() {
    ll g = gcd(n < 0 ? -n : n, d < 0 ? -d : d);
    n /= g;
    d /= g;
    if (d < 0) n = -n, d = -d;
  }
  rat operator+(const rat &b) const { return rat(n * b.d + b.n * d, d * b.d); }
  rat operator-(const rat &b) const { return rat(n * b.d - b.n * d, d * b.d); }
  rat operator*(const rat &b) const { return rat(n * b.n, d * b.d); }
  rat operator/(const rat &b) const { return rat(n * b.d, d * b.n); }
  bool operator==(const rat &b) const { return n == b.n && d == b.d; }
  bool zero() const { return n == 0; }
};

struct oracle_result {
  int rank;
  bool inconsistent;
  vector<rat> solution;
};

oracle_result solve_oracle(const vector<vector<int>> &input, int m) {
  const int n = static_cast<int>(input.size());
  vector<vector<rat>> a(n, vector<rat>(m + 1));
  for (int i = 0; i < n; ++i)
    for (int j = 0; j <= m; ++j) a[i][j] = rat(input[i][j]);

  vector<int> where(m, -1);
  int row = 0;
  for (int col = 0; col < m && row < n; ++col) {
    int pivot = row;
    while (pivot < n && a[pivot][col].zero()) ++pivot;
    if (pivot == n) continue;
    swap(a[pivot], a[row]);
    where[col] = row;
    const rat inv = a[row][col];
    for (int j = 0; j <= m; ++j) a[row][j] = a[row][j] / inv;
    for (int i = 0; i < n; ++i) if (i != row && !a[i][col].zero()) {
      const rat mul = a[i][col];
      for (int j = 0; j <= m; ++j) a[i][j] = a[i][j] - mul * a[row][j];
    }
    ++row;
  }

  for (int i = 0; i < n; ++i) {
    bool lhs_nonzero = false;
    for (int j = 0; j < m; ++j) lhs_nonzero |= !a[i][j].zero();
    if (!lhs_nonzero && !a[i][m].zero()) return {-1, true, {}};
  }

  vector<rat> solution(m);
  for (int col = 0; col < m; ++col)
    if (where[col] != -1) solution[col] = a[where[col]][m];
  return {row, false, solution};
}

void check_case(const vector<vector<int>> &input) {
  const int n = static_cast<int>(input.size());
  const int m = static_cast<int>(input[0].size()) - 1;
  const oracle_result want = solve_oracle(input, m);

  matrix got{};
  got.n = n;
  got.m = m;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j <= m; ++j) got.M[i][j] = fraction(input[i][j]);
  const int rank = got.solve();
  const int expected = want.inconsistent ? -1 : want.rank;
  if (rank != expected) {
    cerr << "Simultaneous status/rank mismatch n=" << n << " m=" << m
         << " got=" << rank << " want=" << expected << '\n';
    exit(1);
  }
  if (want.inconsistent) return;

  for (int j = 0; j < m; ++j) {
    if (got.sol[j].n != want.solution[j].n || got.sol[j].d != want.solution[j].d) {
      cerr << "Simultaneous solution mismatch n=" << n << " m=" << m
           << " variable=" << j << " got=" << got.sol[j].n << '/'
           << got.sol[j].d << " want=" << want.solution[j].n << '/'
           << want.solution[j].d << '\n';
      exit(1);
    }
  }
  for (int i = 0; i < n; ++i) {
    rat lhs;
    for (int j = 0; j < m; ++j)
      lhs = lhs + rat(input[i][j]) * rat(got.sol[j].n, got.sol[j].d);
    if (!(lhs == rat(input[i][m]))) {
      cerr << "Simultaneous returned solution does not satisfy row " << i
           << " n=" << n << " m=" << m << '\n';
      exit(1);
    }
  }
}

void exhaustive(int n, int m) {
  const int cells = n * (m + 1);
  long long total = 1;
  for (int i = 0; i < cells; ++i) total *= 3;
  for (long long mask = 0; mask < total; ++mask) {
    long long code = mask;
    vector<vector<int>> a(n, vector<int>(m + 1));
    for (int i = 0; i < n; ++i) for (int j = 0; j <= m; ++j) {
      a[i][j] = static_cast<int>(code % 3) - 1;
      code /= 3;
    }
    check_case(a);
  }
}

int main() {
  for (int n = 1; n <= 3; ++n) for (int m = 1; m <= 3; ++m)
    exhaustive(n, m);

  mt19937_64 rng(190029);
  for (int tc = 0; tc < 3000; ++tc) {
    const int n = 1 + rng() % 6;
    const int m = 1 + rng() % 6;
    vector<vector<int>> a(n, vector<int>(m + 1));
    for (auto &row : a) for (int &x : row) x = static_cast<int>(rng() % 7) - 3;
    check_case(a);
  }

  cout << "PASS Simultaneous_Equations exact exhaustive 3x3 and signed random systems\n";
}
