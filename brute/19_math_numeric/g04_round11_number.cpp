#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/6_Math/ax+by=gcd.cpp"
#include "../../codebook/6_Math/chineseRemainder.cpp"
#include "../../codebook/6_Math/Linear_Congruence.cpp"
namespace mobius {
int n, tot;
vector<int> mu, flg, p;
#include "../../codebook/6_Math/getMu.cpp"
}
using i128 = __int128;
ll norm(i128 x, ll m) { return (x % m + m) % m; }

void crt_edges() {
  vector<ll> divisors;
  for (ll d : {1, 7, 49, 73, 127, 337, 92737, 649657})
    if (LLONG_MAX % d == 0) divisors.pb(d);
  for (ll d : divisors) for (ll e : divisors)
    for (ll anchor : {LLONG_MIN, LLONG_MIN + 1, -1LL, 0LL, LLONG_MAX}) {
      ll a = LLONG_MAX / d, b = LLONG_MAX / e;
      ll lcm = a / gcd(a, b) * b;
      ll x = norm(anchor, a), y = norm(anchor, b);
      ll want = norm(anchor, lcm);
      assert(solve(x, a, y, b) == want);
      assert(solve(y, b, x, a) == want);
      // Merge order must agree when a third modulus is the whole lcm.
      assert(solve(want, lcm, anchor, LLONG_MAX) ==
             norm(anchor, LLONG_MAX));
      ll reverse = solve(y, b, anchor, LLONG_MAX);
      assert(solve(x, a, reverse, LLONG_MAX) ==
             norm(anchor, LLONG_MAX));
      if (gcd(a, b) > 1) assert(solve(x, a, y + 1, b) == -1);
    }
  // Consecutive Fibonacci moduli maximize Euclidean recursion depth.
  for (ll a = 1, b = 2; b <= 3037000499LL;) {
    for (auto [x, y] : {pll{LLONG_MIN, LLONG_MAX}, pll{-1, 1}}) {
      ll got = solve(x, a, y, b);
      assert(got >= 0 && got < a * b);
      assert(got % a == norm(x, a) && got % b == norm(y, b));
    }
    ll next = a + b; a = b, b = next;
  }
}

void congruence_edges() {
  for (ll g : {1, 2, 7, 31, 100})
    for (ll step : {1LL, 2LL, 3LL, 999983LL, LLONG_MAX / g}) {
      ll n = g * step;
      for (ll b : {LLONG_MIN, LLONG_MIN + 1, -1LL, 0LL,
                   1LL, LLONG_MAX - 1, LLONG_MAX}) {
        ll normalized = norm(b, n);
        auto answer = mod_leq(-g, b, n);
        if (normalized % g) assert(answer.empty());
        else {
          ll first = norm(-i128(normalized / g), step);
          assert(answer.size() == (size_t)g);
          for (ll i = 0; i < g; ++i) {
            assert(answer[i] == first + i * step);
            assert(answer[i] >= 0 && answer[i] < n);
            assert(norm(-i128(g) * answer[i], n) == normalized);
          }
        }
      }
    }
  for (ll n : {1LL, 2LL, 127LL, 4096LL}) {
    auto answer = mod_leq(LLONG_MIN, 0, n);
    ll g = gcd(norm(LLONG_MIN, n), n);
    assert(answer.size() == (size_t)g);
    for (ll i = 0; i < g; ++i) assert(answer[i] == i * (n / g));
    assert(mod_leq(0, LLONG_MAX - LLONG_MAX % n, n).size() ==
           (size_t)n);
  }
}

void mu_edges() {
  vector<int> limits{1, 2, 3, 4, 8, 9, 24, 25, 48, 49, 50};
  for (int p : {11, 31, 97, 257})
    for (int d : {-1, 0, 1}) limits.pb(p * p + d);
  // Shrinking calls must initialize every value even with poisoned mu.
  reverse(ALL(limits));
  for (int n : limits) {
    vector<int> spf(n + 1), primes;
    for (int p = 2; p <= n; ++p) if (!spf[p]) {
      primes.pb(p);
      for (int x = p; x <= n; x += p) if (!spf[x]) spf[x] = p;
    }
    mobius::n = n, mobius::tot = 0;
    mobius::mu.assign(n + 2, 12345);
    mobius::flg.assign(n + 2, 0); mobius::flg[n + 1] = 67890;
    mobius::p.assign(primes.size() + 2, -54321);
    mobius::getMu();
    assert(mobius::tot == (int)primes.size());
    assert(mobius::p.front() == -54321 && mobius::p.back() == -54321);
    assert(equal(ALL(primes), mobius::p.begin() + 1));
    assert(mobius::mu.front() == 12345 && mobius::mu.back() == 12345);
    assert(mobius::flg.front() == 0 && mobius::flg.back() == 67890);
    for (int x = 1; x <= n; ++x) {
      int y = x, want = 1;
      while (y > 1) {
        int p = spf[y]; y /= p;
        want = -want;
        if (y % p == 0) { want = 0; break; }
      }
      assert(mobius::mu[x] == want);
      assert(bool(mobius::flg[x]) == (x > 1 && spf[x] != x));
    }
  }
}

int main() {
  crt_edges(); congruence_edges(); mu_edges();
  cout << "g04 round11 number PASS: CRT merge order/near-limit lcm, "
       << "signed short periods, exact prime capacity and sentinels\n";
}
