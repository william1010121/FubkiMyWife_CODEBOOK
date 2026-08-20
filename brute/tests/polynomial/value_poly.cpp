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
  friend mint operator*(mint a, mint b) { return a *= b; }
  friend bool operator==(mint a, mint b) { return a.v == b.v; }
  friend bool operator!=(mint a, mint b) { return !(a == b); }
  friend bool operator>=(mint a, mint b) { return a.v >= b.v; }
  friend bool operator<(mint a, mint b) { return a.v < b.v; }
};
#define SZ(x) ((int)(x).size())
#define pb push_back
vector<mint> ifac, inegfac;
#include "codebook/7_Polynomial/Value_Poly.cpp"

static mint eval(const vector<mint> &c, mint x) {
  mint r = 0;
  for (int i = (int)c.size() - 1; i >= 0; --i) r = r * x + c[i];
  return r;
}
int main() {
  const int K = 32;
  vector<mint> fac(K + 1, 1), invfac(K + 1, 1);
  for (int i = 1; i <= K; ++i) fac[i] = fac[i - 1] * i;
  auto mpow = [](mint a, int e) { mint r = 1; for (; e; e >>= 1, a *= a) if (e & 1) r *= a; return r; };
  for (int i = 0; i <= K; ++i) invfac[i] = mpow(fac[i], mint::MOD - 2);
  ifac = invfac; inegfac.resize(K + 1);
  for (int i = 0; i <= K; ++i) inegfac[i] = (i & 1) ? mint(0) - invfac[i] : invfac[i];
  mt19937 rng(0xface);
  for (int tc = 0; tc < 300; ++tc) {
    int base = 1 + rng() % 20, n = 1 + rng() % 12;
    vector<mint> coeff(n);
    for (mint &x : coeff) x = rng() % 1000;
    coeff[0] = 1;
    Poly p(base, eval(coeff, base));
    p.poly.resize(n);
    for (int i = 0; i < n; ++i) p.poly[i] = eval(coeff, base + i);
    for (int i = 0; i < n; ++i) {
      if (p.get_val(base + i) != p.poly[i]) { cerr << "Value_Poly inside mismatch\n"; return 1; }
    }
    for (int x : {base - 3, base + n + 2, 100})
      if (p.get_val(x) != eval(coeff, x)) { cerr << "Value_Poly Lagrange mismatch\n"; return 1; }
    auto before = p.poly;
    mint next = eval(coeff, base + n);
    p.raise();
    mint pref = 0;
    for (int i = 0; i < n + 1; ++i) {
      pref += eval(coeff, base + i);
      if (p.poly[i] != pref) { cerr << "Value_Poly raise mismatch\n"; return 1; }
    }
    if (p.poly.back() != pref || next != eval(coeff, base + n)) return 1;
    (void)before;
  }
  cout << "value_poly: PASS\n";
}
