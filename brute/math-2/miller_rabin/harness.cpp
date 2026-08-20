#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using u128 = __uint128_t;

ll mul(ll a, ll b, ll m) {
  return (ll)((u128)(unsigned long long)a * (unsigned long long)b /
              1 % (unsigned long long)m);
}

#include "../../../codebook/6_Math/Miller_Rabin.cpp"

bool prime(ll n) {
  if (n < 2) return false;
  for (ll p : {2LL, 3LL, 5LL, 7LL, 11LL, 13LL, 17LL, 19LL, 23LL, 29LL,
               31LL, 37LL}) {
    if (n == p) return true;
    if (n % p == 0) return false;
  }
  for (ll a : {2LL, 325LL, 9375LL, 28178LL, 450775LL, 9780504LL,
               1795265022LL})
    if (!Miller_Rabin(a, n)) return false;
  return true;
}

bool trial(ll n) {
  if (n < 2) return false;
  for (ll d = 2; d <= n / d; ++d) if (n % d == 0) return n == d;
  return true;
}

int main() {
  for (ll n = 0; n <= 1000000; ++n)
    if (prime(n) != trial(n)) {
      cerr << "small mismatch n=" << n << '\n';
      return 1;
    }
  vector<pair<ll, bool>> boundary = {
      {1000000007LL, true}, {1000000009LL, true},
      {1000000007LL * 1000000009LL, false}, {341550071728321LL, false},
      {3825123056546413051LL, false}, {2305843009213693951LL, true},
      {4611686018427387847LL, true}, {9223372036854775783LL, true},
      {LLONG_MAX, false}};
  for (auto [n, want] : boundary)
    if (prime(n) != want) {
      cerr << "boundary mismatch n=" << n << '\n';
      return 1;
    }
  cout << "Miller-Rabin OK: 1,000,001 trial cases + 9 boundary cases\n";
}
