#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll, ll>;
#define X first
#define Y second
#include "../../../codebook/6_Math/ax+by=gcd.cpp"

int main() {
  for (ll a = 0; a <= 200; ++a) for (ll b = 0; b <= 200; ++b) {
    if (a == 0 && b == 0) continue;
    pll q = exgcd(a, b);
    ll g = std::gcd(a, b);
    if (a * q.X + b * q.Y != g) {
      cerr << "exgcd mismatch a=" << a << " b=" << b << '\n';
      return 1;
    }
  }
  cout << "PASS ax+by=gcd exhaustive nonnegative 200x200\n";
}
