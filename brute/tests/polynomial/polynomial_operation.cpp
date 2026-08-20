#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template<int MAXN, ll P, ll RT>
struct NTT {
  static ll mpow(ll a, ll e) {
    ll r = 1;
    for (; e; e >>= 1, a = a * a % P) if (e & 1) r = r * a % P;
    return r;
  }
  static ll minv(ll a) { return mpow((a % P + P) % P, P - 2); }
  void operator()(ll *a, int n, bool inv = false) const {
    for (int i = 1, j = 0; i < n; ++i) {
      int bit = n >> 1;
      for (; j & bit; bit >>= 1) j ^= bit;
      j ^= bit;
      if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
      ll wlen = mpow(RT, (P - 1) / len);
      if (inv) wlen = minv(wlen);
      for (int i = 0; i < n; i += len) {
        ll w = 1;
        for (int j = 0; j < len / 2; ++j) {
          ll u = a[i + j], v = a[i + j + len / 2] * w % P;
          a[i + j] = (u + v) % P;
          a[i + j + len / 2] = (u - v + P) % P;
          w = w * wlen % P;
        }
      }
    }
    if (inv) {
      ll z = minv(n);
      for (int i = 0; i < n; ++i) a[i] = a[i] * z % P;
    }
  }
};
ll QuadraticResidue(ll a, ll p) {
  a = (a % p + p) % p;
  for (ll x = 0; x <= 100; ++x) if (x * x % p == a) return x;
  return -1;
}
#include "codebook/7_Polynomial/Polynomial_Operation.cpp"

using P = Poly_t;
static P add(const P &a, const P &b) {
  P c(max(a.n(), b.n()));
  for (int i = 0; i < c.n(); ++i)
    c[i] = ((i < a.n() ? a[i] : 0) + (i < b.n() ? b[i] : 0)) % 998244353;
  return c;
}
static P naive_mul(const P &a, const P &b, int lim = 1000000) {
  P c(min(lim, a.n() + b.n() - 1));
  for (int i = 0; i < a.n(); ++i) for (int j = 0; j < b.n() && i + j < lim; ++j)
    c[i + j] = (c[i + j] + a[i] * b[j]) % 998244353;
  return c;
}
static vector<ll> eval_naive(const P &p, const vector<ll> &x) {
  vector<ll> y;
  for (ll z : x) {
    ll cur = 0;
    for (int i = p.n() - 1; i >= 0; --i) cur = (cur * z + p[i]) % 998244353;
    y.push_back(cur);
  }
  return y;
}
static void check_eq(const P &a, const P &b, const char *what) {
  if (a.n() != b.n() || !equal(a.begin(), a.end(), b.begin())) {
    cerr << "Polynomial mismatch: " << what << '\n'; exit(1);
  }
}
int main() {
  mt19937 rng(0x7017);
  for (int tc = 0; tc < 100; ++tc) {
    int n = 1 + rng() % 8, m = 1 + rng() % 8;
    P a(n), b(m);
    for (ll &x : a) x = rng() % 1000;
    for (ll &x : b) x = rng() % 1000;
    check_eq(a.Mul(b), naive_mul(a, b), "Mul");
    if (a[0] == 0) a[0] = 1;
    auto inv = a.Inv();
    P unit = naive_mul(a, inv, n); unit.isz(n);
    if (unit[0] != 1) { cerr << "Inv constant mismatch\n"; return 1; }
    for (int i = 1; i < n; ++i) if (unit[i] != 0) { cerr << "Inv mismatch\n"; return 1; }
    P q(1 + rng() % 5), d(2 + rng() % 5), r;
    r = P(1 + rng() % (d.n() - 1));
    for (ll &x : q) x = rng() % 50;
    for (ll &x : r) x = rng() % 50;
    for (ll &x : d) x = rng() % 50;
    if (d.back() == 0) d.back() = 1;
    if (r.n() >= d.n()) r.isz(d.n() - 1);
    r.isz(max(1, d.n() - 1));
    P f = add(naive_mul(d, q), r);
    auto dm = f.DivMod(d);
    if (dm.first.n() != q.n() || !equal(dm.first.begin(), dm.first.end(), q.begin())) {
      cerr << "Polynomial mismatch: DivMod quotient\n"; return 1;
    }
    if (dm.second.n() != r.n() || !equal(dm.second.begin(), dm.second.end(), r.begin())) {
      cerr << "Polynomial mismatch: DivMod remainder\n"
           << "q="; for (ll x : q) cerr << x << ',';
      cerr << " d="; for (ll x : d) cerr << x << ',';
      cerr << " r="; for (ll x : r) cerr << x << ',';
      cerr << " got="; for (ll x : dm.second) cerr << x << ',';
      cerr << '\n'; return 1;
    }
    auto dx = a.Dx(), sx = dx.Sx(); sx.isz(a.n());
    P no_constant = a; no_constant[0] = 0;
    check_eq(sx, no_constant, "Dx/Sx");
    vector<ll> xs;
    for (int i = 0; i < n + 2; ++i) xs.push_back(i + 1);
    if (eval_naive(a, xs) != a.Eval(xs)) { cerr << "Eval mismatch\n"; return 1; }
    auto ip = P::Interpolate(xs, eval_naive(a, xs));
    ip.isz(a.n()); check_eq(ip, a, "Interpolate");
  }
  for (int n = 1; n <= 10; ++n) {
    P g(n), one(n); one[0] = 1;
    for (int i = 1; i < n; ++i) g[i] = rng() % 100;
    auto e = g.Exp(); e.isz(n);
    auto l = e.Ln(); l.isz(n); check_eq(l, g, "Ln/Exp");
    P sqroot(n); sqroot[0] = 1;
    for (int i = 1; i < n; ++i) sqroot[i] = rng() % 20;
    auto sq = naive_mul(sqroot, sqroot, n);
    auto got = sq.Sqrt(); got.isz(n);
    check_eq(naive_mul(got, got, n), sq, "Sqrt");
    P pw(n); pw[0] = 1;
    for (int i = 1; i < n; ++i) pw[i] = rng() % 20;
    auto gotpow = pw.Pow(3);
    P wantpow(1); wantpow[0] = 1;
    for (int k = 0; k < 3; ++k) wantpow = naive_mul(wantpow, pw, n);
    wantpow.isz(n); check_eq(gotpow, wantpow, "Pow");
  }
  vector<ll> init = {2, 3, 5};
  vector<ll> coef = {0, 1, 1, 1};
  ll x0 = init[0], x1 = init[1], x2 = init[2];
  for (ll n = 0; n < 80; ++n) {
    ll want = n == 0 ? x0 : n == 1 ? x1 : n == 2 ? x2 : 0;
    if (n >= 3) {
      want = (x2 + x1 + x0) % 998244353;
      x0 = x1; x1 = x2; x2 = want;
    }
    if (P::LinearRecursion(init, coef, n) != want) { cerr << "LinearRecursion mismatch\n"; return 1; }
  }
  cout << "polynomial_operation: PASS\n";
}
