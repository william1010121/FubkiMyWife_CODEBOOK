#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using std::gcd;

#include "../../codebook/6_Math/Fraction.cpp"

pair<ll, ll> norm(ll n, ll d) {
  if (d == 0) {
    cerr << "oracle received zero denominator\n";
    exit(1);
  }
  ll g = gcd(n < 0 ? -n : n, d < 0 ? -d : d);
  n /= g;
  d /= g;
  if (d < 0) n = -n, d = -d;
  return {n, d};
}

bool eq_fraction(const fraction &x, pair<ll, ll> y) {
  return x.n == y.first && x.d == y.second;
}

void fail(const char *op, const fraction &a, const fraction &b,
          pair<ll, ll> want, const fraction &got) {
  cerr << "Fraction " << op << " mismatch a=" << a.n << '/' << a.d
       << " b=" << b.n << '/' << b.d << " got=" << got.n << '/' << got.d
       << " want=" << want.first << '/' << want.second << '\n';
  exit(1);
}

int main() {
  vector<fraction> values;
  for (ll n = -24; n <= 24; ++n)
    for (ll d = -24; d <= 24; ++d)
      if (d != 0) values.emplace_back(n, d);

  for (const fraction &a : values) {
    if (!eq_fraction(a, norm(a.n, a.d))) {
      cerr << "Fraction constructor failed for " << a.n << '/' << a.d
           << '\n';
      return 1;
    }
    if (!eq_fraction(-a, norm(-a.n, a.d))) {
      cerr << "Fraction negation failed for " << a.n << '/' << a.d << '\n';
      return 1;
    }
  }

  for (const fraction &a : values) for (const fraction &b : values) {
    const auto na = norm(a.n, a.d), nb = norm(b.n, b.d);
    const auto add = norm(na.first * nb.second + nb.first * na.second,
                          na.second * nb.second);
    const auto sub = norm(na.first * nb.second - nb.first * na.second,
                          na.second * nb.second);
    const auto mul = norm(na.first * nb.first, na.second * nb.second);
    if (!eq_fraction(a + b, add)) fail("addition", a, b, add, a + b);
    if (!eq_fraction(a - b, sub)) fail("subtraction", a, b, sub, a - b);
    if (!eq_fraction(a * b, mul)) fail("multiplication", a, b, mul, a * b);
    if (b.n != 0) {
      const auto div = norm(na.first * nb.second, na.second * nb.first);
      if (!eq_fraction(a / b, div)) fail("division", a, b, div, a / b);
    }
  }

  mt19937_64 rng(190025);
  auto nonzero_denominator = [&]() {
    ll d;
    do {
      d = static_cast<ll>(rng() % 81) - 40;
    } while (d == 0);
    return d;
  };
  for (int tc = 0; tc < 20000; ++tc) {
    fraction a(static_cast<ll>(rng() % 81) - 40, nonzero_denominator());
    fraction b(static_cast<ll>(rng() % 81) - 40, nonzero_denominator());
    fraction c(static_cast<ll>(rng() % 81) - 40, nonzero_denominator());
    const auto lhs = (a + b) + c;
    const auto rhs = a + (b + c);
    const auto want = norm((a.n * b.d + b.n * a.d) * c.d +
                               c.n * (a.d * b.d),
                           a.d * b.d * c.d);
    if (!eq_fraction(lhs, want) || !eq_fraction(rhs, want)) {
      cerr << "Fraction associativity mismatch\n";
      return 1;
    }
  }

  cout << "PASS Fraction signed denominators, zero numerators, exhaustive pairs, random triples\n";
}
