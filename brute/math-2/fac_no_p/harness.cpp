#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define MAXP 200000

ll mpow(ll a, ll e, ll m) {
  ll r = 1 % m;
  for (; e; e >>= 1, a = a * a % m) if (e & 1) r = r * a % m;
  return r;
}
#include "../../../codebook/6_Math/fac_no_p.cpp"

ll oracle(ll n, ll p, ll pk) {
  ll r = 1 % pk;
  for (ll i = 1; i <= n; ++i) {
    ll x = i;
    while (x % p == 0) x /= p;
    r = r * (x % pk) % pk;
  }
  return r;
}

int main() {
  long long checked = 0;
  for (ll p : {2LL, 3LL, 5LL, 7LL, 11LL, 13LL, 17LL, 19LL}) {
    ll pk = 1;
    for (int k = 1; k <= 4; ++k) {
      pk *= p;
      for (ll n = 0; n <= 250; ++n) {
        ll got = fac_no_p(n, p, pk), want = oracle(n, p, pk);
        if (got != want) {
          cerr << "mismatch n=" << n << " p=" << p << " pk=" << pk
               << " want=" << want << " got=" << got << '\n';
          return 1;
        }
        ++checked;
      }
      for (ll n : {1000000000000LL, LLONG_MAX}) {
        ll got = fac_no_p(n, p, pk), old = 1;
        // Independent exponentiation path checks all high exponent bits.
        for (ll m = n; m; m /= p)
          old = old * mpow(prod[pk], m / pk, pk) % pk * prod[m % pk] % pk;
        if (got != old) {
          cerr << "large-n mismatch n=" << n << " p=" << p << " pk=" << pk
               << " want=" << old << " got=" << got << '\n';
          return 1;
        }
        ++checked;
      }
    }
  }
  cout << "fac_no_p OK: " << checked << " exhaustive cases\n";
}
