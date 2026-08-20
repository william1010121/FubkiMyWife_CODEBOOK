#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using u128 = __uint128_t;

ll mul(ll a, ll b, ll m) {
  return (ll)((u128)(unsigned long long)a * (unsigned long long)b %
              (unsigned long long)m);
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

#include "../../../codebook/6_Math/Pollard_Rho.cpp"

map<ll, int> trial_factor(ll n) {
  map<ll, int> got;
  for (ll p = 2; p <= n / p; ++p) {
    while (n % p == 0) ++got[p], n /= p;
  }
  if (n > 1) ++got[n];
  return got;
}

int main() {
  vector<ll> cases;
  for (ll n = 1; n <= 100000; ++n) cases.push_back(n);
  cases.insert(cases.end(), {
      1000000007LL * 1000000009LL,
      999999937LL * 1000000007LL,
      1000003LL * 1000033LL,
      1000000007LL * 1000000033LL,
      2305843009213693951LL});
  for (ll n : cases) {
    cnt.clear();
    PollardRho(n);
    auto want = trial_factor(n);
    if (cnt != want) {
      cerr << "factor mismatch n=" << n << "\nwant:";
      for (auto [p, e] : want) cerr << ' ' << p << '^' << e;
      cerr << "\ngot:";
      for (auto [p, e] : cnt) cerr << ' ' << p << '^' << e;
      cerr << '\n';
      return 1;
    }
  }
  cout << "Pollard-Rho OK: " << cases.size() << " factorization cases\n";
}
