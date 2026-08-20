#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#include "../../../codebook/6_Math/floor_sum.cpp"

ll brute(ll n, ll m, ll a, ll b) {
  ll ans = 0;
  for (ll i = 0; i < n; ++i) ans += (a * i + b) / m;
  return ans;
}
int main() {
  for (ll n = 0; n <= 60; ++n) for (ll m = 1; m <= 50; ++m)
    for (ll a = 0; a <= 100; ++a) for (ll b = 0; b <= 100; ++b) {
      ll got = floor_sum(n, m, a, b), want = brute(n, m, a, b);
      if (got != want) {
        cerr << "floor_sum mismatch n=" << n << " m=" << m
             << " a=" << a << " b=" << b << " got=" << got
             << " want=" << want << '\n';
        return 1;
      }
    }
  cout << "PASS floor_sum exhaustive bounded nonnegative inputs\n";
}
