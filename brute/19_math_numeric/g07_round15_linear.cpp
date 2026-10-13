#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll,ll>;
#define X first
#define Y second
#include "../../codebook/6_Math/ax+by=gcd.cpp"
#include "../../codebook/6_Math/Linear_Congruence.cpp"

int main() {
  mt19937_64 rng(1507);
  // Contest-scale coprime solves and inconsistent even equations.
  const ll prime = 1000000007;
  ll checksum = 0;
  for (int i = 0; i < 200000; ++i) {
    ll a = 1 + rng() % (prime - 1), b = rng() % prime;
    auto v = mod_leq(a, b, prime);
    assert(v.size() == 1 && 0 <= v[0] && v[0] < prime);
    assert((__int128)a * v[0] % prime == b);
    checksum += v[0];
    assert(mod_leq(2*a, 2*b+1, 2*prime).empty());
  }
  // Bezout reconstruction uses wide products even for tiny gcd.
  for (int i = 0; i < 10000; ++i) {
    ll a = ll(rng() >> 1), n = LLONG_MAX - (rng() % 100000);
    ll g = gcd(a, n), step = n / g;
    if (g > 1000) continue; // Avoid inherently huge output.
    ll b = (rng() % step) * g;
    auto v = mod_leq(a, b, n);
    assert(ll(v.size()) == g);
    for (ll j = 0; j < g; ++j) {
      assert(0 <= v[j] && v[j] < n);
      assert((__int128)a * v[j] % n == b);
      if (j) assert(v[j] - v[j-1] == step);
    }
    if (g > 1) assert(mod_leq(a, b+1, n).empty());
  }
  cout << "PASS round15 linear 200k solves/failures, wide Bezout: "
       << checksum << '\n';
}
