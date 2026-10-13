#include <bits/stdc++.h>
using namespace std;
using ll = long long;
// Exact bounded rational oracle; abort rather than silently overflow.
struct Q {
  ll n, d;
  Q(__int128 a = 0, __int128 b = 1) {
    if (!b) abort();
    if (b < 0) a = -a, b = -b;
    __int128 x = a < 0 ? -a : a, y = b;
    while (y) { __int128 r = x % y; x = y; y = r; }
    a /= x; b /= x;
    if (a < LLONG_MIN || a > LLONG_MAX || b > LLONG_MAX) abort();
    n = a; d = b;
  }
  Q operator-() const { return Q(-(__int128)n, d); }
  Q &operator+=(Q b) { return *this = *this + b; }
  friend Q operator+(Q a, Q b) {
    return Q((__int128)a.n * b.d + (__int128)b.n * a.d,
             (__int128)a.d * b.d);
  }
  friend Q operator-(Q a, Q b) { return a + -b; }
  friend Q operator*(Q a, Q b) {
    return Q((__int128)a.n * b.n, (__int128)a.d * b.d);
  }
  friend Q operator/(Q a, Q b) {
    return Q((__int128)a.n * b.d, (__int128)a.d * b.n);
  }
  friend bool operator==(Q a, Q b) { return a.n == b.n && a.d == b.d; }
};
#define SZ(a) int((a).size())
#include "../../codebook/6_Math/Berlekamp-Massey.cpp"
#undef SZ

bool feasible(const vector<Q> &s, int k) {
  vector<vector<Q>> a;
  for (int i = k; i < int(s.size()); ++i) {
    vector<Q> row(k + 1); row[k] = s[i];
    for (int j = 0; j < k; ++j) row[j] = s[i - j - 1];
    a.push_back(row);
  }
  int r = 0;
  for (int j = 0; j < k && r < int(a.size()); ++j) {
    int t = r;
    while (t < int(a.size()) && a[t][j] == 0) ++t;
    if (t == int(a.size())) continue;
    swap(a[t], a[r]); Q inv = Q(1) / a[r][j];
    for (int c = j; c <= k; ++c) a[r][c] = a[r][c] * inv;
    for (int i = r + 1; i < int(a.size()); ++i) {
      Q z = a[i][j];
      for (int c = j; c <= k; ++c) a[i][c] = a[i][c] - z * a[r][c];
    }
    ++r;
  }
  for (auto &row : a) {
    bool zero = true;
    for (int j = 0; j < k; ++j) zero &= row[j] == 0;
    if (zero && !(row[k] == 0)) return false;
  }
  return true;
}
void check(const vector<Q> &s) {
  auto c = BerlekampMassey(s);
  int k = 0; while (!feasible(s, k)) ++k;
  if (int(c.size()) != k) abort();
  for (int i = k; i < int(s.size()); ++i) {
    Q z;
    for (int j = 0; j < k; ++j) z += c[j] * s[i - j - 1];
    if (!(z == s[i])) abort();
  }
}
int main() {
  int cases = 0;
  // All short signed sequences; delayed discrepancies included.
  for (int n = 0, total = 1; n <= 5; ++n, total *= 5)
    for (int mask = 0; mask < total; ++mask) {
      vector<Q> s(n); int z = mask;
      for (auto &x : s) x = z % 5 - 2, z /= 5;
      check(s); ++cases;
    }
  mt19937 rng(0x808b);
  for (int tc = 0; tc < 400; ++tc) {
    vector<Q> s(rng() % 11);
    for (auto &x : s) x = Q(int(rng() % 7) - 3, 1 + rng() % 3);
    check(s); ++cases;
  }
  // All prefixes of a recurrence with fractional coefficients.
  vector<Q> s = {Q(1), Q(-1, 2)};
  for (int i = 2; i < 20; ++i)
    s.push_back(Q(1, 2) * s[i - 1] - Q(1, 3) * s[i - 2]);
  for (int n = 0; n <= int(s.size()); ++n) {
    check(vector<Q>(s.begin(), s.begin() + n)); ++cases;
  }
  cout << "g08_bm_rational: PASS " << cases
       << " exact rational minimal-order checks\n";
}
