#include <bits/stdc++.h>
using namespace std;

struct mint {
  static constexpr int MOD = 998244353;
  int v;
  mint(long long x = 0) : v((x % MOD + MOD) % MOD) {}
  mint &operator+=(mint o) { if ((v += o.v) >= MOD) v -= MOD; return *this; }
  mint &operator-=(mint o) { if ((v -= o.v) < 0) v += MOD; return *this; }
  mint &operator*=(mint o) { v = 1LL * v * o.v % MOD; return *this; }
  friend mint operator+(mint a, mint b) { return a += b; }
  friend mint operator-(mint a, mint b) { return a -= b; }
  friend mint operator-(mint a) { return mint(-a.v); }
  friend mint operator*(mint a, mint b) { return a *= b; }
  friend bool operator==(mint a, mint b) { return a.v == b.v; }
  friend bool operator!=(mint a, mint b) { return !(a == b); }
  friend bool operator>=(mint a, mint b) { return a.v >= b.v; }
  friend bool operator<(mint a, mint b) { return a.v < b.v; }
  static mint power(mint a, int e) {
    mint r = 1;
    for (; e; e >>= 1, a *= a) if (e & 1) r *= a;
    return r;
  }
  friend mint operator/(mint a, mint b) { return a * power(b, MOD - 2); }
};

#define SZ(x) ((int)(x).size())
#define pb push_back
vector<mint> ifac, inegfac;
#include "../../codebook/7_Polynomial/Value_Poly.cpp"
#undef pb
#undef SZ

static mint eval(const vector<mint> &c, mint x) {
  mint ret = 0;
  for (int i = (int)c.size() - 1; i >= 0; --i) ret = ret * x + c[i];
  return ret;
}

static mint lagrange(const vector<mint> &y, int base, mint x) {
  mint ret = 0;
  for (int i = 0; i < (int)y.size(); ++i) {
    mint term = y[i];
    for (int j = 0; j < (int)y.size(); ++j) if (i != j)
      term = term * (x - mint(base + j)) / mint(i - j);
    ret = ret + term;
  }
  return ret;
}

int main() {
  const int K = 96;
  vector<mint> fac(K + 1, 1), invfac(K + 1, 1);
  for (int i = 1; i <= K; ++i) fac[i] = fac[i - 1] * i;
  for (int i = 0; i <= K; ++i) invfac[i] = mint(1) / fac[i];
  ifac = invfac; inegfac.resize(K + 1);
  for (int i = 0; i <= K; ++i) inegfac[i] = (i & 1) ? -invfac[i] : invfac[i];

  for (int base : {0, 3, 100, mint::MOD - 2}) for (int n = 1; n <= 18; ++n) {
    vector<mint> coeff(n);
    for (int i = 0; i < n; ++i) coeff[i] = (i * i * 17 + 5 * i + 3) % 1000;
    Poly p(base, eval(coeff, base));
    p.poly.resize(n);
    for (int i = 0; i < n; ++i) p.poly[i] = eval(coeff, base + i);
    for (int x : {base - 3, base - 1, base, base + n - 1, base + n, base + 2 * n + 1})
      if (p.get_val(x) != eval(coeff, x)) {
        cerr << "Value_Poly evaluation mismatch base=" << base << " n=" << n << '\n';
        return 1;
      }
    vector<mint> old = p.poly, pref(n + 1);
    for (int i = 0; i <= n; ++i) {
      pref[i] = (i ? pref[i - 1] : mint(0)) + eval(coeff, base + i);
    }
    p.raise();
    if (p.poly != pref) { cerr << "Value_Poly raise values mismatch\n"; return 1; }
    for (int x : {base - 2, base + n + 2, base + 2 * n + 5})
      if (p.get_val(x) != lagrange(pref, base, x)) {
        cerr << "Value_Poly raised interpolation mismatch\n"; return 1;
      }
    (void)old;
  }
  Poly zero(17, 0);
  zero.raise();
  if (zero.poly.size() != 1 || zero.poly[0] != mint(0)) {
    cerr << "Value_Poly zero raise should be a no-op\n"; return 1;
  }
  Poly one(0, 1);
  one.raise();
  if (one.poly != vector<mint>({mint(1), mint(2)})) {
    cerr << "Value_Poly constant raise mismatch\n"; return 1;
  }
  cout << "value_poly_edges: PASS consecutive interpolation/raise boundaries\n";
}
