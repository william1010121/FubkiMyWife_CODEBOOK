#include <bits/stdc++.h>
using namespace std;
struct mint {
  static inline int P;
  int v;
  mint(long long x = 0) : v((x % P + P) % P) {}
  mint &operator+=(mint b) { v = (v + b.v) % P; return *this; }
  mint &operator*=(mint b) { v = v * b.v % P; return *this; }
  mint operator-() const { return -v; }
  friend mint operator+(mint a, mint b) { return a += b; }
  friend mint operator-(mint a, mint b) { return a + -b; }
  friend mint operator*(mint a, mint b) { return a.v * b.v; }
  friend bool operator==(mint a, mint b) { return a.v == b.v; }
};
vector<mint> ifac, inegfac;
#define SZ(a) int((a).size())
#define pb push_back
#include "../../codebook/7_Polynomial/Value_Poly.cpp"
#undef pb
#undef SZ
mint eval(const vector<mint> &c, mint x) {
  mint r;
  for (int i = int(c.size()) - 1; i >= 0; --i) r = r * x + c[i];
  return r;
}
int main() {
  mt19937 rng(0x50fa);
  int cases = 0;
  for (int p : {2, 3, 5, 7, 17}) {
    mint::P = p; ifac.assign(p, 1); inegfac.assign(p, 1);
    int fac = 1;
    for (int i = 0; i < p; ++i) {
      if (i) fac = fac * i % p;
      int inv = 1; while (inv * fac % p != 1) ++inv;
      ifac[i] = inv; inegfac[i] = (i & 1) ? -ifac[i] : ifac[i];
    }
    for (int base = 0; base < p; ++base)
      for (int n = 1; n <= p; ++n) for (int tc = 0; tc < 24; ++tc) {
        vector<mint> coeff(n);
        for (auto &x : coeff) x = rng() % p;
        Poly f(base); f.poly.resize(n);
        for (int i = 0; i < n; ++i) f.poly[i] = eval(coeff, base + i);
        for (int x = -p; x < 2 * p; ++x)
          if (!(f.get_val(x) == eval(coeff, x))) abort();
        // n=p covers every field point; n=p-1 is the last legal lift.
        if (n < p) {
          vector<mint> want(p);
          for (int i = 0; i < p; ++i)
            want[i] = (i ? want[i - 1] : mint(0)) +
                      eval(coeff, mint(base + i));
          f.raise();
          for (int i = 0; i < p; ++i)
            if (!(f.get_val(base + i) == want[i])) abort();
          // Repeated lifts exercise updated tables and wrapped samples.
          for (int k = n + 1; k < p; ++k) {
            for (int i = 1; i < p; ++i) want[i] += want[i - 1];
            f.raise();
            for (int i = 0; i < p; ++i)
              if (!(f.get_val(base + i) == want[i])) abort();
          }
        }
        ++cases;
      }
  }
  // Exhaust arbitrary sample vectors, rather than generated coefficients.
  // Product-form Lagrange independently checks public poly mutation.
  for (int p : {2, 3, 5}) {
    mint::P = p; ifac.assign(p, 1); inegfac.assign(p, 1);
    int fac = 1;
    for (int i = 0; i < p; ++i) {
      if (i) fac = fac * i % p;
      int inv = 1; while (inv * fac % p != 1) ++inv;
      ifac[i] = inv; inegfac[i] = (i & 1) ? -ifac[i] : ifac[i];
    }
    for (int n = 1, total = p; n <= p; ++n, total *= p)
      for (int base = 0; base < p; ++base)
        for (int mask = 0; mask < total; ++mask) {
          Poly f(base); f.poly.resize(n); int z = mask;
          for (auto &x : f.poly) x = z % p, z /= p;
          vector<mint> want(p);
          for (int t = 0; t < p; ++t) {
            for (int i = 0; i < n; ++i) {
              mint term = f.poly[i];
              for (int j = 0; j < n; ++j) if (i != j) {
                mint d = i - j; int inv = 1;
                while (inv * d.v % p != 1) ++inv;
                term *= mint(t - j) * mint(inv);
              }
              want[t] += term;
            }
            if (!(f.get_val(base + t) == want[t])) abort();
          }
          if (n < p) {
            f.raise();
            for (int t = 1; t < p; ++t) want[t] += want[t - 1];
            for (int t = 0; t < p; ++t)
              if (!(f.get_val(base + t) == want[t])) abort();
          }
          ++cases;
        }
  }
  cout << "g08_value_poly_small_fields: PASS " << cases
       << " full-field interpolation and repeated wrapped lifts\n";
}
