#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pll = pair<ll, ll>;
#define X first
#define Y second

#include "../../codebook/6_Math/ax+by=gcd.cpp"

void check(ll a, ll b) {
  if (a == 0 && b == 0) return;
  pll q = exgcd(a, b);
  ll g = gcd(a, b);
  __int128 lhs = static_cast<__int128>(a) * q.X +
                 static_cast<__int128>(b) * q.Y;
  if (lhs != g) {
    cerr << "exgcd mismatch a=" << a << " b=" << b << " x=" << q.X
         << " y=" << q.Y << " want gcd=" << g << '\n';
    exit(1);
  }
}

int main() {
  for (ll a = 0; a <= 500; ++a)
    for (ll b = 0; b <= 500; ++b) check(a, b);

  const vector<pair<ll, ll>> boundary = {
      {0, 1}, {1, 0}, {0, LLONG_MAX}, {LLONG_MAX, 0},
      {1, LLONG_MAX}, {LLONG_MAX, 1}, {LLONG_MAX, LLONG_MAX},
      {LLONG_MAX, LLONG_MAX - 1}, {LLONG_MAX, LLONG_MAX - 2},
      {3037000499LL, 3037000500LL}, {1000000000000LL, 999999999999LL},
      {9999999967LL, 9999999973LL}, {720720, 1081080},
  };
  for (auto [a, b] : boundary) check(a, b);

  mt19937_64 rng(0xABCD1818ULL);
  for (int tc = 0; tc < 2500; ++tc)
    check(static_cast<ll>(rng() % 1'000'000'000'000ULL),
          static_cast<ll>(rng() % 1'000'000'000'000ULL));
  cout << "PASS ax+by=gcd extra: exhaustive zeros, boundaries, and seeded cases\n";
}
