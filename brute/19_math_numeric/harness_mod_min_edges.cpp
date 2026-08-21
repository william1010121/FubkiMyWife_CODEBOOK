#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "../../codebook/6_Math/ModMin.cpp"

ll direct(ll a, ll m, ll l, ll r) {
  const ll period = a == 0 ? 1 : m / gcd(a, m);
  for (ll k = 0; k < period; ++k)
    if (l <= (a * k) % m && (a * k) % m <= r) return k;
  return -1;
}

void check(ll a, ll m, ll l, ll r) {
  const ll got = mod_min(a, m, l, r);
  const ll want = direct(a, m, l, r);
  if (got != want) {
    cerr << "mod_min mismatch a=" << a << " m=" << m << " l=" << l
         << " r=" << r << " got=" << got << " want=" << want << '\n';
    exit(1);
  }
}

int main() {
  // Exhaust all intervals for small moduli; this emphasizes singleton and
  // empty-residue cases as well as a=0 and m=1.
  for (ll m = 1; m <= 45; ++m)
    for (ll a = 0; a < m; ++a)
      for (ll l = 0; l < m; ++l)
        for (ll r = l; r < m; ++r) check(a, m, l, r);

  mt19937_64 rng(190023);
  for (int tc = 0; tc < 30000; ++tc) {
    const ll m = 1 + rng() % 500;
    const ll a = rng() % m;
    const ll l = rng() % m;
    const ll r = l + rng() % (m - l);
    check(a, m, l, r);
  }

  cout << "PASS ModMin exhaustive small intervals and seeded random cases\n";
}
