#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128_t;

#include "../../codebook/6_Math/floor_sum.cpp"

ll floor_div(i128 x, ll m) {
  i128 q = x / m, r = x % m;
  if (r < 0) --q;
  return static_cast<ll>(q);
}

ll direct(ll n, ll m, ll a, ll b) {
  i128 ans = 0;
  for (ll i = 0; i < n; ++i)
    ans += floor_div(static_cast<i128>(a) * i + b, m);
  return static_cast<ll>(ans);
}

void check(ll n, ll m, ll a, ll b) {
  if (m <= 0 || n < 0 || a < 0 || b < 0) {
    cerr << "internal invalid supported case\n";
    exit(1);
  }
  const ll got = floor_sum(n, m, a, b);
  const ll want = direct(n, m, a, b);
  if (got != want) {
    cerr << "floor_sum mismatch n=" << n << " m=" << m << " a=" << a
         << " b=" << b << " got=" << got << " want=" << want << '\n';
    exit(1);
  }
}

int main() {
  // Dense small exhaustive coverage, including n=0, m=1, and zero a/b.
  for (ll n = 0; n <= 20; ++n)
    for (ll m = 1; m <= 35; ++m)
      for (ll a = 0; a <= 80; ++a)
        for (ll b = 0; b <= 80; ++b) check(n, m, a, b);

  // Exact multiples and values immediately around each reduction boundary.
  for (ll m = 1; m <= 500; ++m) {
    const vector<ll> ns = {0, 1, 2, 3, m - 1, m, m + 1, 2 * m + 1,
                           100000};
    const vector<ll> vals = {0, 1, m - 1, m, m + 1, 2 * m - 1, 2 * m,
                             100000};
    for (ll n : ns) for (ll a : vals) for (ll b : vals) check(n, m, a, b);
  }

  mt19937_64 rng(190021);
  for (int tc = 0; tc < 20000; ++tc) {
    const ll n = rng() % 1001;
    const ll m = 1 + rng() % 1000;
    const ll a = rng() % 5001;
    const ll b = rng() % 5001;
    check(n, m, a, b);
  }

  cout << "PASS floor_sum nonnegative contract, zero/exact/boundary/random cases\n";
}
