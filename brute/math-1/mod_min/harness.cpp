#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#include "../../../codebook/6_Math/ModMin.cpp"

ll brute(ll a, ll m, ll l, ll r) {
  for (ll k = 0; k <= 2 * m + 2; ++k)
    if ((a * k) % m >= l && (a * k) % m <= r) return k;
  return -1;
}
int main() {
  for (ll m = 1; m <= 80; ++m) for (ll a = 0; a < m; ++a)
    for (ll l = 0; l < m; ++l) for (ll r = l; r < m; ++r) {
      ll got = mod_min(a, m, l, r), want = brute(a, m, l, r);
      if (got != want) {
        cerr << "mod_min mismatch a=" << a << " m=" << m
             << " l=" << l << " r=" << r << " got=" << got
             << " want=" << want << '\n';
        return 1;
      }
    }
  cout << "PASS ModMin exhaustive m<=80\n";
}
