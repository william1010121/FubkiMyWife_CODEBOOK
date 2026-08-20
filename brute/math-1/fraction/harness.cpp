#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using std::gcd;
#include "../../../codebook/6_Math/Fraction.cpp"

pair<ll,ll> norm(ll n, ll d) {
  ll g = std::gcd(n, d); n /= g; d /= g;
  if (d < 0) n = -n, d = -d;
  return {n, d};
}
bool eq(const fraction &x, pair<ll,ll> y) { return x.n == y.first && x.d == y.second; }
int main() {
  vector<fraction> v;
  for (ll n = -9; n <= 9; ++n) for (ll d = 1; d <= 9; ++d) v.emplace_back(n, d);
  for (const auto &a : v) for (const auto &b : v) {
    auto na = norm(a.n, a.d), nb = norm(b.n, b.d);
    if (!eq(-a, norm(-a.n, a.d)) ||
        !eq(a + b, norm(na.first * nb.second + nb.first * na.second,
                         na.second * nb.second)) ||
        !eq(a - b, norm(na.first * nb.second - nb.first * na.second,
                         na.second * nb.second)) ||
        !eq(a * b, norm(na.first * nb.first, na.second * nb.second))) {
      cerr << "fraction arithmetic mismatch\n";
      return 1;
    }
    if (b.n != 0 && !eq(a / b, norm(na.first * nb.second, na.second * nb.first))) {
      cerr << "fraction division mismatch\n";
      return 1;
    }
  }
  cout << "PASS Fraction bounded rational cross-product\n";
}
