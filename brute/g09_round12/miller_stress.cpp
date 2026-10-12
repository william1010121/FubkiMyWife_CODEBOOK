#include <bits/stdc++.h>
using namespace std;
using ll = long long;
ll mul(ll a, ll b, ll n) { return (__int128)a * b % n; }
#include "../../codebook/6_Math/Miller_Rabin.cpp"

// Independent strong-test power by low-to-high binary exponentiation.
bool reference(ll a, ll n) {
  if (n < 2) return false;
  if ((a %= n) == 0) return true;
  if (!(n & 1)) return n == 2;
  ll d = n - 1, x = 1;
  int s = 0;
  while (!(d & 1)) d /= 2, ++s;
  for (ll e = d, b = a; e; e /= 2, b = mul(b, b, n))
    if (e & 1) x = mul(x, b, n);
  if (x == 1 || x == n - 1) return true;
  for (int i = 1; i < s; ++i) {
    x = mul(x, x, n);
    if (x == n - 1) return true;
  }
  return false;
}
int main() {
  mt19937_64 rng(1209);
  for (int i = 0; i < 200000; ++i) {
    ll n = (rng() & LLONG_MAX) | 1;
    ll a = i % 2 ? 2 : rng() & LLONG_MAX;
    assert(Miller_Rabin(a, n) == reference(a, n));
  }
  for (int k = 1; k <= 62; ++k) {
    ll n = (1LL << k) + 1;
    for (ll a : {0LL, 1LL, 2LL, n - 1, n, LLONG_MAX})
      assert(Miller_Rabin(a, n) == reference(a, n));
  }
  for (ll n : {LLONG_MIN, -1LL, 0LL, 1LL, 2LL, 4LL, LLONG_MAX})
    for (ll a : {0LL, 1LL, 2LL, LLONG_MAX})
      assert(Miller_Rabin(a, n) == reference(a, n));
  cout << "PASS 200k full-width witnesses and one-bit exponents\n";
}
