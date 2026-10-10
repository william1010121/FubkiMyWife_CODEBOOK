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
  cout << "g08_value_poly_small_fields: PASS " << cases
       << " full-field interpolation and repeated wrapped lifts\n";
}
