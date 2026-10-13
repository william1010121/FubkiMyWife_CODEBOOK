#include <bits/stdc++.h>
using namespace std;
using ll = long long;
ll pow_mod(ll a, ll e, ll m) {
  ll r = 1 % m;
  for (; e; e >>= 1, a = a * a % m)
    if (e & 1) r = r * a % m;
  return r;
}
namespace qr {
#include "../../codebook/6_Math/QuadraticResidue.cpp"
}
namespace pc {
#include "../../codebook/6_Math/PiCount.cpp"
}
namespace dl {
int fpow(int x, int e, int m) { return pow_mod(x, e, m); }
#include "../../codebook/6_Math/DiscreteLog.cpp"
}
struct F {
  static inline int P;
  int v;
  F(ll x = 0) : v((x % P + P) % P) {}
  F &operator+=(F b) { v = (v + b.v) % P; return *this; }
  F operator-() const { return -v; }
  friend F operator+(F a, F b) { return a += b; }
  friend F operator-(F a, F b) { return a + -b; }
  friend F operator*(F a, F b) { return a.v * b.v; }
  friend F operator/(F a, F b) {
    if (!b.v) abort();
    return a * F(pow_mod(b.v, P - 2, P));
  }
  friend bool operator==(F a, F b) { return a.v == b.v; }
};
namespace bm {
#define SZ(a) int((a).size())
#include "../../codebook/6_Math/Berlekamp-Massey.cpp"
#undef SZ
}
// Existence of an order-k recurrence, via independent elimination.
bool feasible(const vector<F> &s, int k) {
  vector<vector<F>> a;
  for (int i = k; i < int(s.size()); ++i) {
    vector<F> row(k + 1); row[k] = s[i];
    for (int j = 0; j < k; ++j) row[j] = s[i - j - 1];
    a.push_back(row);
  }
  int r = 0;
  for (int j = 0; j < k && r < int(a.size()); ++j) {
    int t = r;
    while (t < int(a.size()) && a[t][j] == 0) ++t;
    if (t == int(a.size())) continue;
    swap(a[t], a[r]); F inv = F(1) / a[r][j];
    for (int c = j; c <= k; ++c) a[r][c] = a[r][c] * inv;
    for (int i = r + 1; i < int(a.size()); ++i) {
      F z = a[i][j];
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
void check_bm(const vector<F> &s) {
  auto c = bm::BerlekampMassey(s);
  int k = 0; while (!feasible(s, k)) ++k;
  if (int(c.size()) != k) abort();
  for (int i = k; i < int(s.size()); ++i) {
    F z;
    for (int j = 0; j < k; ++j) z += c[j] * s[i - j - 1];
    if (!(z == s[i])) abort();
  }
}
int jacobi_oracle(int a, int m) {
  int sign = 1;
  for (int p = 3; 1LL * p * p <= m; p += 2)
    while (m % p == 0) {
      ll x = (1LL * a % p + p) % p;
      ll v = pow_mod(x, (p - 1) / 2, p);
      sign *= v == p - 1 ? -1 : int(v); m /= p;
    }
  if (m > 1) {
    ll x = (1LL * a % m + m) % m;
    ll v = pow_mod(x, (m - 1) / 2, m);
    sign *= v == m - 1 ? -1 : int(v);
  }
  return sign;
}
int main() {
  mt19937 rng(0x508a);
  srand(508);
  for (int i = 0; i < 400; ++i) {
    int m = (rng() & INT_MAX) | 1;
    for (int a : {INT_MIN, INT_MAX, 0, -1, int(rng())})
      if (qr::Jacobi(a, m) != jacobi_oracle(a, m)) abort();
  }
  for (int p : {2, 3, 17, 65537, 998244353, 1000000007, INT_MAX})
    for (int i = 0; i < 80; ++i) {
      int a = i == 0 ? INT_MIN : i == 1 ? INT_MAX : int(rng());
      ll x = (1LL * a % p + p) % p;
      int got = qr::QuadraticResidue(a, p);
      bool residue = p == 2 || !x || pow_mod(x, (p - 1) / 2, p) == 1;
      if (residue ? got < 0 || got >= p || 1LL * got * got % p != x
                  : got != -1) abort();
    }
  // Enumerate all targets, including nonresidues, over more small primes.
  for (int p = 2; p <= 257; ++p) {
    bool prime = true;
    for (int q = 2; q * q <= p; ++q) prime &= p % q != 0;
    if (!prime) continue;
    vector<bool> square(p);
    for (int x = 0; x < p; ++x) square[x * x % p] = true;
    for (int a = -p; a < p; ++a) {
      int got = qr::QuadraticResidue(a, p), x = (a + p) % p;
      if (square[x] ? got < 0 || got >= p || got * got % p != x
                    : got != -1) abort();
    }
  }
  for (ll n : {LLONG_MIN, -100LL, -1LL, 0LL, 1LL})
    if (pc::PrimeCount(n) != 0) abort();
  const int lim = 1000000;
  vector<int> pi(lim + 1); vector<bool> composite(lim + 1);
  for (int p = 2; p * p <= lim; ++p)
    if (!composite[p])
      for (int j = p * p; j <= lim; j += p) composite[j] = true;
  for (int i = 1; i <= lim; ++i)
    pi[i] = pi[i - 1] + (i >= 2 && !composite[i]);
  // The sieve stops at p^4: hit both sides of every small cutoff.
  for (ll p = 2; p * p * p * p + 2 <= lim; ++p)
    for (int d = -2; d <= 2; ++d) {
      ll n = p * p * p * p + d;
      if (pc::PrimeCount(n) != pi[n]) abort();
    }
  for (int k = 2; k * k + 1 <= lim; ++k)
    for (int d : {-1, 0, 1})
      if (pc::PrimeCount(k * k + d) != pi[k * k + d]) abort();
  // Independent segmented sieve + known pi(10^13): large sqrt edges.
  const ll top = 10000000000000LL, low = top - 12000000;
  const int root = 3162277;
  vector<bool> base(root + 1), segment(top - low + 1);
  for (int p = 2; p <= root; ++p) if (!base[p]) {
    if (1LL * p * p <= root)
      for (int j = p * p; j <= root; j += p) base[j] = true;
    for (ll j = (low + p - 1) / p * p; j <= top; j += p)
      segment[j - low] = true;
  }
  vector<int> suffix(segment.size() + 1);
  for (int i = int(segment.size()) - 1; i >= 0; --i)
    suffix[i] = suffix[i + 1] + !segment[i];
  for (int k : {root - 1, root}) for (int d : {-1, 0, 1}) {
    ll n = 1LL * k * k + d;
    if (n < low || n >= top) abort();
    ll want = 346065536839LL - suffix[n - low + 1];
    if (pc::PrimeCount(n) != want) abort();
  }
  for (int m : {1, 2, 4, 16, 27, 81, 125, 256, 343, 625, 1024})
    for (int x = 0; x < m; ++x) {
      vector<int> first(m, -1);
      int y = 1 % m, k = 0;
      while (first[y] < 0) {
        first[y] = k++; y = 1LL * y * x % m;
      }
      // Duplicates, eventual zero, and absent targets in every orbit.
      for (int t : {0, 1 % m, m - 1, int(rng() % m)})
        if (dl::DiscreteLog(x, t, m) != first[t]) abort();
    }
  for (int tc = 0; tc < 600; ++tc) {
    int m = 1000 + rng() % 199001, x = rng() % m;
    vector<int> first(m, -1), orbit;
    for (int y = 1 % m; first[y] < 0; y = 1LL * y * x % m)
      first[y] = orbit.size(), orbit.push_back(y);
    int step = ceil(sqrt(m));
    for (int k : {0, 99, 100, 101, step, 2 * step,
                  int(orbit.size()) - 1}) {
      int y = orbit[min(k, int(orbit.size()) - 1)];
      if (dl::DiscreteLog(x, y, m) != first[y]) abort();
    }
    for (int i = 0; i < 8; ++i) {
      int y = rng() % m;
      if (dl::DiscreteLog(x, y, m) != first[y]) abort();
    }
  }
  int count = 0;
  for (int p : {2, 3, 5, 7}) {
    F::P = p;
    for (int n = 0; n <= (p <= 3 ? 7 : 4); ++n) {
      int total = pow_mod(p, n, INT_MAX);
      for (int mask = 0; mask < total; ++mask) {
        vector<F> s(n); int z = mask;
        for (auto &x : s) x = z % p, z /= p;
        check_bm(s); ++count;
      }
    }
    // Delayed first discrepancy and impulses at both ends.
    for (int n : {1, 2, 31, 64}) {
      vector<F> s(n); s.back() = 1; check_bm(s);
      s.front() = p - 1; check_bm(s);
    }
  }
  cout << "g08_math_adversarial: PASS Jacobi/Cipolla extremes, "
          "PiCount cutoffs, finite orbits, " << count
       << " small-field BM sequences\n";
}
