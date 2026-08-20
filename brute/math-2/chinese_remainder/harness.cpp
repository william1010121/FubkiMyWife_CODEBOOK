#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll, ll>;

pll exgcd(ll a, ll b) {
  if (b == 0) return {1, 0};
  auto q = exgcd(b, a % b);
  return {q.second, q.first - q.second * (a / b)};
}
#include "../../../codebook/6_Math/chineseRemainder.cpp"

ll oracle(ll x1, ll m1, ll x2, ll m2) {
  ll g = gcd(m1, m2), lcm = m1 / g * m2;
  for (ll z = 0; z < lcm; ++z)
    if ((z - x1) % m1 == 0 && (z - x2) % m2 == 0) return z;
  return -1;
}

int main() {
  long long checked = 0;
  for (ll m1 = 1; m1 <= 24; ++m1)
    for (ll m2 = 1; m2 <= 24; ++m2)
      for (ll x1 = -m1; x1 < 2 * m1; ++x1)
        for (ll x2 = -m2; x2 < 2 * m2; ++x2) {
          ll want = oracle(x1, m1, x2, m2);
          ll got = solve(x1, m1, x2, m2);
          if (got != want) {
            cerr << "mismatch x1=" << x1 << " m1=" << m1
                 << " x2=" << x2 << " m2=" << m2 << " want=" << want
                 << " got=" << got << '\n';
            return 1;
          }
          ++checked;
        }
  cout << "Chinese remainder OK: " << checked << " exhaustive cases\n";
}
