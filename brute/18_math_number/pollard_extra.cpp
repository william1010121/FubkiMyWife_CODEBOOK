#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using u128 = __uint128_t;

ll mul(ll a, ll b, ll m) {
  return static_cast<ll>((u128)static_cast<unsigned long long>(a) *
                         static_cast<unsigned long long>(b) /
                         1 % static_cast<unsigned long long>(m));
}

#include "../../codebook/6_Math/Miller_Rabin.cpp"

bool prime(ll n) {
  if (n < 2) return false;
  for (ll p : {2LL, 3LL, 5LL, 7LL, 11LL, 13LL, 17LL, 19LL, 23LL,
               29LL, 31LL, 37LL}) {
    if (n == p) return true;
    if (n % p == 0) return false;
  }
  for (ll a : {2LL, 325LL, 9375LL, 28178LL, 450775LL, 9780504LL,
               1795265022LL})
    if (!Miller_Rabin(a, n)) return false;
  return true;
}

#include "../../codebook/6_Math/Pollard_Rho.cpp"

map<ll, int> trial_factor(ll n) {
  map<ll, int> result;
  for (ll p = 2; p <= n / p; ++p) {
    while (n % p == 0) {
      ++result[p];
      n /= p;
    }
  }
  if (n > 1) ++result[n];
  return result;
}

struct Case {
  ll n;
  map<ll, int> want;
};

void add_factor_case(vector<Case> &cases, initializer_list<ll> factors) {
  __int128 product = 1;
  map<ll, int> want;
  for (ll p : factors) {
    product *= p;
    ++want[p];
  }
  if (product <= 0 || product > LLONG_MAX) {
    cerr << "bad test factor product\n";
    exit(1);
  }
  cases.push_back({static_cast<ll>(product), want});
}

int main() {
  vector<Case> cases;
  for (ll n = 1; n <= 25000; ++n) cases.push_back({n, trial_factor(n)});

  add_factor_case(cases, {2});
  add_factor_case(cases, {3, 3, 3, 3, 3, 3, 3, 3});
  add_factor_case(cases, {999983, 999983});
  add_factor_case(cases, {1000003, 1000033, 1000037});
  add_factor_case(cases, {1000000007LL, 1000000007LL});
  add_factor_case(cases, {1000000007LL, 1000000009LL});
  add_factor_case(cases, {999999937LL, 1000000007LL});
  add_factor_case(cases, {2147483647LL, 2147483647LL});
  add_factor_case(cases, {2305843009213693951LL});
  add_factor_case(cases, {2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                          2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                          2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                          2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                          2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                          2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                          2, 2});
  add_factor_case(cases, {7, 7, 73, 127, 337, 92737, 649657});

  const vector<ll> prime_pool = {
      2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
      53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 127, 257,
      1009, 10007, 99991, 999983};
  mt19937_64 rng(0xBADC0FFEE18ULL);
  for (int tc = 0; tc < 80; ++tc) {
    __int128 product = 1;
    map<ll, int> want;
    int count = 2 + static_cast<int>(rng() % 3);
    for (int i = 0; i < count; ++i) {
      ll p = prime_pool[rng() % prime_pool.size()];
      if (product * p > LLONG_MAX / 2) {
        --i;
        continue;
      }
      product *= p;
      ++want[p];
    }
    cases.push_back({static_cast<ll>(product), want});
  }

  long long checked = 0;
  for (const Case &tc : cases) {
    cnt.clear();
    PollardRho(tc.n);
    if (cnt != tc.want) {
      cerr << "Pollard-Rho mismatch n=" << tc.n << "\nwant:";
      for (auto [p, e] : tc.want) cerr << ' ' << p << '^' << e;
      cerr << "\ngot:";
      for (auto [p, e] : cnt) cerr << ' ' << p << '^' << e;
      cerr << '\n';
      return 1;
    }
    __int128 product = 1;
    for (auto [p, e] : cnt)
      for (int i = 0; i < e; ++i) product *= p;
    if (product != tc.n) {
      cerr << "Pollard-Rho product mismatch n=" << tc.n << '\n';
      return 1;
    }
    ++checked;
  }
  cout << "PASS Pollard-Rho extra: " << checked << " factorization cases\n";
}
