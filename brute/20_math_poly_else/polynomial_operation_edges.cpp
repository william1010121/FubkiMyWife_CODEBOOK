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

#include "../../codebook/6_Math/QuadraticResidue.cpp"

#include "../../codebook/7_Polynomial/Polynomial_Operation.cpp"

using P = Poly_t;
static constexpr ll MOD = 998244353;

static P from(initializer_list<ll> xs) {
  P p(xs.size());
  int i = 0;
  for (ll x : xs) p[i++] = (x % MOD + MOD) % MOD;
  return p;
}

static P naive_mul(const P &a, const P &b, int lim = 1000000) {
  P c(min(lim, a.n() + b.n() - 1));
  for (int i = 0; i < a.n(); ++i) for (int j = 0; j < b.n() && i + j < lim; ++j)
    c[i + j] = (c[i + j] + a[i] * b[j]) % MOD;
  return c;
}

static void same(P a, P b, const string &where) {
  if (a.n() != b.n() || !equal(a.begin(), a.end(), b.begin())) {
    cerr << "Polynomial mismatch at " << where << '\n';
    exit(1);
  }
}

static vector<ll> eval_naive(const P &p, const vector<ll> &xs) {
  vector<ll> ret;
  for (ll x : xs) {
    ll y = 0;
    for (int i = p.n() - 1; i >= 0; --i) y = (y * x + p[i]) % MOD;
    ret.push_back(y);
  }
  return ret;
}

int main() {
  mt19937 rng(0x70e1);
  for (int n = 1; n <= 20; ++n) for (int m = 1; m <= 18; ++m) {
    P a(n), b(m);
    for (ll &x : a) x = ((int)(rng() % 41) - 20 + MOD) % MOD;
    for (ll &x : b) x = ((int)(rng() % 41) - 20 + MOD) % MOD;
    same(a.Mul(b), naive_mul(a, b), "Mul exhaustive sizes");
    if (a[0] == 0) a[0] = 1;
    auto inv = a.Inv();
    P unit = naive_mul(a, inv, n); unit.isz(n);
    P one(n); one[0] = 1;
    if (unit.n() != one.n() || !equal(unit.begin(), unit.end(), one.begin())) {
      cerr << "Inv case n=" << n << " unit=";
      for (ll x : unit) cerr << x << ',';
      cerr << " a=";
      for (ll x : a) cerr << x << ',';
      cerr << " inv=";
      for (ll x : inv) cerr << x << ',';
      cerr << '\n';
      return 1;
    }
  }

  for (int tc = 0; tc < 120; ++tc) {
    int qn = 1 + rng() % 8, dn = 1 + rng() % 7;
    P q(qn), d(dn), r(max(1, dn - 1));
    for (ll &x : q) x = rng() % 100;
    for (ll &x : d) x = rng() % 100;
    for (ll &x : r) x = rng() % 100;
    if (d.back() == 0) d.back() = 1;
    if (q.back() == 0) q.back() = 1;
    if (dn == 1) r[0] = 0;
    if (r.n() >= d.n()) r.isz(max(1, d.n() - 1));
    P f = naive_mul(q, d); f.isz(max(f.n(), r.n()));
    for (int i = 0; i < r.n(); ++i) f[i] = (f[i] + r[i]) % MOD;
    auto dm = f.DivMod(d);
    if (dm.first.n() != q.n() || !equal(dm.first.begin(), dm.first.end(), q.begin())) {
      cerr << "DivMod qn=" << q.n() << " dn=" << d.n() << " fn=" << f.n() << " got=";
      for (ll x : dm.first) cerr << x << ',';
      cerr << " want=";
      for (ll x : q) cerr << x << ',';
      cerr << " d=";
      for (ll x : d) cerr << x << ',';
      cerr << " r=";
      for (ll x : r) cerr << x << ',';
      cerr << '\n';
      return 1;
    }
    same(dm.second, r, "DivMod remainder");
  }
  same(from({1}).DivMod(from({1, 2})).first, from({0}), "DivMod lower degree quotient");
  same(from({1}).DivMod(from({1, 2})).second, from({1}), "DivMod lower degree remainder");

  for (int n = 1; n <= 13; ++n) {
    P g(n);
    g[0] = 0;
    for (int i = 1; i < n; ++i) g[i] = rng() % 500;
    P e = g.Exp(); e.isz(n);
    P l = e.Ln(); l.isz(n);
    same(l, g, "Ln/Exp");

    P root(n);
    root[0] = 1;
    for (int i = 1; i < n; ++i) root[i] = rng() % 20;
    P square = naive_mul(root, root, n); square.isz(n);
    P got = square.Sqrt(); got.isz(n);
    same(naive_mul(got, got, n), square, "Sqrt");

    P p(n); p[0] = 1;
    for (int i = 1; i < n; ++i) p[i] = rng() % 20;
    for (int k = 0; k <= 4; ++k) {
      P want(1); want[0] = 1;
      for (int z = 0; z < k; ++z) want = naive_mul(want, p, n);
      want.isz(n);
      same(p.Pow(k), want, "Pow");
    }
  }
  P leading(7); leading[0] = 0; leading[1] = 3; leading[2] = 5;
  P want(7); want[0] = 1;
  want = naive_mul(want, leading, 7);
  want = naive_mul(want, leading, 7); want.isz(7);
  same(leading.Pow(2), want, "Pow with leading zero");
  P zero(5); zero[0] = 0;
  same(zero.Pow(0), from({1, 0, 0, 0, 0}), "zero to zero");

  for (int n = 1; n <= 12; ++n) {
    P p(n);
    for (ll &x : p) x = rng() % 1000;
    vector<ll> xs;
    for (int i = 0; i < n + 3; ++i) xs.push_back((i * 97 + 11) % MOD);
    if (p.Eval(xs) != eval_naive(p, xs)) { cerr << "Eval mismatch\n"; return 1; }
    auto ip = P::Interpolate(xs, eval_naive(p, xs));
    ip.isz(n);
    same(ip, p, "Interpolate");
  }
  vector<ll> init = {2, 3, 5};
  vector<ll> coef = {0, 1, 1, 1};
  ll a0 = init[0], a1 = init[1], a2 = init[2];
  for (ll n = 0; n < 120; ++n) {
    ll want = n == 0 ? a0 : n == 1 ? a1 : n == 2 ? a2 : 0;
    if (n >= 3) {
      want = (a2 + a1 + a0) % MOD;
      a0 = a1; a1 = a2; a2 = want;
    }
    if (P::LinearRecursion(init, coef, n) != want) {
      cerr << "LinearRecursion mismatch n=" << n << '\n'; return 1;
    }
  }
  if (!P::Interpolate({}, {}).empty()) {
    cerr << "Interpolate empty mismatch\n"; return 1;
  }
  if (!P().Eval({}).empty()) { cerr << "Eval empty mismatch\n"; return 1; }
  cout << "polynomial_operation_edges: PASS operations, singular/truncation boundaries\n";
}
