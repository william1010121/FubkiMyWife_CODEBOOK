#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/6_Math/ax+by=gcd.cpp"
#include "../../codebook/9_Else/BinarySearchOnFraction.cpp"

// Independently construct the right Farey neighbor: b*p-a*q=1.
// Add multiples of (a,b) until either coordinate reaches its bound.
Q successor(ll n, Q x) {
  if (!x.q) return {1, 0};
  if (!x.p) return {1, n};
  ll q = -exgcd(x.p, x.q).first % x.q;
  if (q < 0) q += x.q;
  ll p = ((__int128)x.p * q + 1) / x.q;
  ll k = min((n - p) / x.p, (n - q) / x.q);
  return {p + k * x.p, q + k * x.q};
}

void check(ll n, Q target, bool strict) {
  int calls = 0;
  auto pred = [&](Q x) {
    assert(++calls < 10000);
    if (!x.q) return true;
    __int128 diff = (__int128)x.p * target.q -
                    (__int128)target.p * x.q;
    return strict ? diff > 0 : diff >= 0;
  };
  Q want = strict ? successor(n, target) : target;
  Q got = frac_bs(n, pred);
  assert(0 <= got.p && got.p <= n && 0 <= got.q && got.q <= n);
  assert(gcd(got.p, got.q) == 1 && pred(got));
  assert((__int128)got.p * want.q == (__int128)want.p * got.q);
  assert(bool(got.q) == bool(want.q));
  if (strict && got.q) {
    assert((__int128)target.q * got.p -
           (__int128)target.p * got.q == 1);
    assert(target.p + got.p > n || target.q + got.q > n);
  }
}

int main() {
  // Avoid unsupported overflowing search intermediates: n<=10^12.
  for (ll n : {1LL, 2LL, 3LL, 31LL, 1000000000000LL}) {
    for (Q target : {Q{0, 1}, Q{1, 1}, Q{1, n}, Q{n, 1},
                     Q{n - 1, n}, Q{n, n - 1}, Q{1, 0}}) {
      if (gcd(target.p, target.q) != 1) continue;
      check(n, target, false); check(n, target, true);
    }
    for (ll a = 1, b = 2; b <= n;) {
      check(n, {a, b}, false); check(n, {a, b}, true);
      check(n, {b, a}, false); check(n, {b, a}, true);
      ll next = a + b; a = b, b = next;
    }
  }
  mt19937_64 rng(11004);
  for (int i = 0; i < 5000; ++i) {
    ll n = 1 + rng() % 1000000000000LL;
    ll p = 1 + rng() % n, q = 1 + rng() % n, g = gcd(p, q);
    check(n, {p / g, q / g}, false);
    check(n, {p / g, q / g}, true);
  }
  cout << "g04 round11 fraction PASS: strict Farey successor, "
       << "long Fibonacci paths, zero/infinity, N=1 and N=10^12\n";
}
