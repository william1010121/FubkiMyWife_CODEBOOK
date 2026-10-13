#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define SZ(a) ((int)(a).size())
#define pb push_back
#define BITSET_NO_MAIN
#include "../../codebook/1_Basic/bitset.cpp"
#include "../../codebook/6_Math/Mod_Arithmetic.cpp"
#include "../../codebook/6_Math/Mod_Binomial.cpp"
#include "../../codebook/6_Math/Gaussian_Eliminatin.cpp"
int fpow(int a, int k, int m) {
  int r = 1 % m;
  for (; k; k >>= 1, a = 1LL * a * a % m)
    if (k & 1) r = 1LL * r * a % m;
  return r;
}
#include "../../codebook/6_Math/DiscreteLog.cpp"
namespace value {
struct mint {
  static const int P = 1000000007;
  int v;
  mint(ll x = 0) : v((x % P + P) % P) {}
  mint &operator+=(mint b) { v = (v + b.v) % P; return *this; }
  friend mint operator+(mint a, mint b) { return a += b; }
  friend mint operator-(mint a, mint b) { return ll(a.v) - b.v; }
  friend mint operator*(mint a, mint b) { return 1LL * a.v * b.v; }
  mint &operator*=(mint b) { return *this = *this * b; }
  friend bool operator==(mint a, mint b) { return a.v == b.v; }
};
vector<mint> ifac, inegfac;
#include "../../codebook/7_Polynomial/Value_Poly.cpp"
mint eval(const vector<mint> &c, mint x) {
  mint r;
  for (int i = SZ(c) - 1; i >= 0; --i) r = r * x + c[i];
  return r;
}
}
namespace walsh {
#include "../../codebook/7_Polynomial/Fast_Walsh_Transform.cpp"
}
void require(bool ok, const char *msg) {
  if (!ok) { cerr << "FAIL round11 " << msg << '\n'; exit(1); }
}
// Exact decimal-limb binomial, independent of modular factorials/Lucas.
int exact_binomial_mod(ll n, int k, int p) {
  const ll radix = 1000000000; vector<ll> digits{1};
  for (int i = 1; i <= k; ++i) {
    __int128 carry = 0;
    for (ll &v : digits) {
      __int128 z = (__int128)v * (n - i + 1) + carry;
      v = z % radix; carry = z / radix;
    }
    while (carry) digits.pb(carry % radix), carry /= radix;
    ll rem = 0;
    for (int j = SZ(digits) - 1; j >= 0; --j) {
      ll z = rem * radix + digits[j]; digits[j] = z / i; rem = z % i;
    }
    require(rem == 0, "exact binomial division");
  }
  ll r = 0;
  for (int j = SZ(digits) - 1; j >= 0; --j)
    r = (r * radix + digits[j]) % p;
  return r;
}
void binomial() {
  for (ll p : {2LL, 3LL, 31LL, 1000000007LL,
               2305843009213693951LL}) {
    int n = min(256LL, p - 1); ModBinomial b(n, p);
    vector<ll> row(n + 1); row[0] = 1;
    for (int i = 0; i <= n; ++i) {
      for (int j = -1; j <= i + 1; ++j)
        require(b.C(i, j) == (j < 0 || j > i ? 0 : row[j]),
                "binomial vs Pascal at wide prime");
      for (int j = min(i + 1, n); j > 0; --j)
        row[j] = ((__int128)row[j] + row[j - 1]) % p;
    }
    require(b.C(-1, 0) == 0, "negative binomial query");
  }
  for (int p : {2, 3, 5, 31, 97, 257}) {
    ModBinomial b(p - 1, p);
    vector<ll> ns{0, 1, LLONG_MAX, LLONG_MAX - 1, 1LL << 60};
    for (ll q = p; q < LLONG_MAX / p; q *= p)
      ns.insert(ns.end(), {q - 1, q, q + 1});
    for (ll n : ns) for (int k = 0; k <= min(32LL, n); ++k) {
      ll want = exact_binomial_mod(n, k, p);
      require(b.lucas(n, k) == want && b.lucas(n, n - k) == want,
              "Lucas vs exact big integer, carries and LLONG_MAX");
    }
  }
}
void discrete() {
  for (int shift : {0, 1, 10, 14}) for (int base : {2, 6}) {
    int m = 65537 << shift;
    unordered_map<int, int> first; vector<int> orbit;
    for (int y = 1; !first.count(y); y = 1LL * y * base % m) {
      first[y] = SZ(orbit); orbit.pb(y);
    }
    int step = int(ceil(sqrt(m)));
    for (int k : {0, 1, 14, 31, 32, 99, 100, 101,
                  step - 1, step, step + 1, SZ(orbit) - 1}) {
      int y = fpow(base, k, m);
      require(DiscreteLog(base, y, m) == first.at(y),
              "mixed prime-power orbit minimal exponent");
    }
    for (int y : {0, 2, m - 1, m / 2, m / 2 + 1}) {
      auto it = first.find(y);
      require(DiscreteLog(base, y, m) ==
              (it == first.end() ? -1 : it->second),
              "mixed prime-power absent or repeated residue");
    }
  }
}
void gaussian(mt19937 &rng) {
  for (int tc = 0; tc < 1500; ++tc) {
    int n = tc % 13, w = n + 3, rank = (tc / 13) % (n + 1);
    vector<int> cols(n); iota(cols.begin(), cols.end(), 0);
    shuffle(cols.begin(), cols.end(), rng); cols.resize(rank);
    sort(cols.begin(), cols.end());
    vector<vector<double>> want(n, vector<double>(w));
    for (int i = 0; i < rank; ++i) {
      want[i][cols[i]] = 1;
      for (int j = cols[i] + 1; j < w; ++j)
        if (!binary_search(cols.begin(), cols.end(), j))
          want[i][j] = (int(rng() % 17) - 8) / 8.0;
    }
    auto a = want;
    for (int t = 0; n > 1 && t < 20; ++t) {
      int i = rng() % n, j = rng() % n;
      if (i != j) for (int k = 0; k < w; ++k) a[i][k] += a[j][k];
    }
    for (auto &row : a) {
      double s = ldexp(rng() & 1 ? 1.0 : -1.0, int(rng() % 11) - 5);
      for (double &x : row) x *= s;
    }
    shuffle(a.begin(), a.end(), rng); GAS(a);
    for (int i = 0; i < n; ++i) for (int j = 0; j < w; ++j)
      require(abs(a[i][j] - want[i][j]) < 1e-7,
              "known rectangular RREF, all ranks and pivot gaps");
  }
}
void polynomial(mt19937 &rng) {
  using namespace value;
  ifac.resize(32); inegfac.resize(32); ll fac = 1;
  for (int i = 0; i < 32; ++i) {
    if (i) fac = fac * i % mint::P;
    ifac[i] = fpow(fac, mint::P - 2, mint::P);
    inegfac[i] = i & 1 ? mint(0) - ifac[i] : ifac[i];
  }
  for (int tc = 0; tc < 200; ++tc) {
    int n = 1 + tc % 12;
    vector<mint> coeff(n);
    for (auto &x : coeff) x = rng() % mint::P;
    if (tc % 7 == 0) fill(coeff.begin(), coeff.end(), 0);
    mint base = mint::P - 3; Poly f(base); f.poly.resize(n);
    for (int i = 0; i < n; ++i) f.poly[i] = eval(coeff, base + i);
    for (int k = 0; k < 4; ++k) {
      auto before = f; f.raise();
      require(f.get_val(base - 1) == 0, "lift anchor before base");
      for (int i = 0; i < 30; ++i) {
        mint x = i < 10 ? base + i : mint(rng() % mint::P);
        if (k == 0)
          require(before.get_val(x) == eval(coeff, x),
                  "large-field wrapped Horner evaluation");
        require(f.get_val(x) - f.get_val(x - 1) == before.get_val(x),
                "repeated lift discrete derivative outside samples");
      }
    }
  }
}
void fwt() {
  // Rank-product sums are at most C(32,16)=601080390, fitting int.
  for (int L : {16, 1, 10, 0}) {
    int n = 1 << L; vector<int> a(n), want(n);
    for (int i = 0; i < n; ++i) {
      int k = popcount(unsigned(i));
      a[i] = k & 1 ? -1 : 1; want[i] = (k & 1 ? -1 : 1) * (1 << k);
    }
    walsh::subset_convolution(a.data(), a.data(), a.data(), L);
    require(a == want, "dense signed rank cancellation, all aliases");
  }
  for (size_t shift : {size_t(0), size_t(1), size_t(64), SIZE_MAX}) {
    bitset<1> b(1);
    require((b << shift).count() == (shift == 0), "one-bit huge shift");
    require((b >> shift).count() == (shift == 0), "one-bit right shift");
  }
  bitset<129> wide; wide.set(0).set(63).set(64).set(127).set(128);
  for (size_t s : {size_t(0), size_t(1), size_t(63), size_t(64),
                   size_t(128), size_t(129), SIZE_MAX}) {
    auto left = wide << s, right = wide >> s;
    for (size_t i = 0; i < 129; ++i) {
      require(left[i] == (s <= i && wide[i - s]), "wide shift left");
      require(right[i] == (s < 129 - i && wide[i + s]),
              "wide shift right");
    }
  }
}
int main() {
  mt19937 rng(1106);
  binomial(); discrete(); gaussian(rng); polynomial(rng); fwt();
  cout << "g06 round11: PASS binomial, discrete log, Gaussian, "
          "value polynomial, FWT, bitset adversarial checks\n";
}
