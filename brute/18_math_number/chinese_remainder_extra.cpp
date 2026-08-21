#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128_t;
using pll = pair<ll, ll>;

pll exgcd(ll a, ll b) {
  if (b == 0) return {1, 0};
  pll q = exgcd(b, a % b);
  return {q.second, q.first - q.second * (a / b)};
}

#include "../../codebook/6_Math/chineseRemainder.cpp"

struct BigEG {
  i128 g, x, y;
};

BigEG big_exgcd(i128 a, i128 b) {
  if (b == 0) return {a, 1, 0};
  BigEG q = big_exgcd(b, a % b);
  return {q.g, q.y, q.x - (a / b) * q.y};
}

i128 norm(i128 x, i128 m) {
  x %= m;
  if (x < 0) x += m;
  return x;
}

i128 oracle(ll x1, ll m1, ll x2, ll m2) {
  ll g = gcd(m1, m2);
  i128 diff = static_cast<i128>(x2) - static_cast<i128>(x1);
  if (diff % g != 0) return -1;
  ll reduced1 = m1 / g, reduced2 = m2 / g;
  BigEG q = big_exgcd(static_cast<i128>(reduced1),
                      static_cast<i128>(reduced2));
  if (q.g != 1) {
    cerr << "oracle internal gcd failure\n";
    exit(1);
  }
  i128 t = norm((diff / g) * q.x, static_cast<i128>(reduced2));
  i128 lcm = static_cast<i128>(m1) * reduced2;
  return norm(static_cast<i128>(x1) + static_cast<i128>(m1) * t, lcm);
}

void check(ll x1, ll m1, ll x2, ll m2) {
  i128 want = oracle(x1, m1, x2, m2);
  ll got = solve(x1, m1, x2, m2);
  if (want == -1) {
    if (got != -1) {
      cerr << "CRT infeasible mismatch x1=" << x1 << " m1=" << m1
           << " x2=" << x2 << " m2=" << m2 << " got=" << got << '\n';
      exit(1);
    }
    return;
  }
  i128 lcm = static_cast<i128>(m1 / gcd(m1, m2)) * m2;
  if (lcm > numeric_limits<ll>::max()) {
    cerr << "test case has no ll-sized answer\n";
    exit(1);
  }
  if (static_cast<i128>(got) != want) {
    cerr << "CRT mismatch x1=" << x1 << " m1=" << m1
         << " x2=" << x2 << " m2=" << m2 << " want="
         << static_cast<long long>(want)
         << " got=" << got << '\n';
    exit(1);
  }
  if ((static_cast<i128>(got) - x1) % m1 != 0 ||
      (static_cast<i128>(got) - x2) % m2 != 0 || got < 0 ||
      static_cast<i128>(got) >= lcm) {
    cerr << "CRT result is not normalized or does not satisfy congruences\n";
    exit(1);
  }
}

int main() {
  long long checked = 0;
  for (ll m1 = 1; m1 <= 35; ++m1)
    for (ll m2 = 1; m2 <= 35; ++m2)
      for (ll x1 = -2 * m1; x1 <= 2 * m1; ++x1)
        for (ll x2 = -2 * m2; x2 <= 2 * m2; ++x2) {
          check(x1, m1, x2, m2);
          ++checked;
        }

  const vector<array<ll, 4>> boundary = {
      {0, 1, -123, 1},
      {-1, 6, 3, 8},
      {1, 6, 4, 8},
      {-5, 6, 7, 9},
      {-1, 1000000007LL, 2, 1000000009LL},
      {-123, 1000000007LL, -120, 1000000009LL},
      {-1, 1000000000LL, 1, 1000000002LL},
      {-1, 3037000499LL, 0, 3037000497LL},
      {-100, 1000000000LL, 100, 1000000002LL},
      {-1000000006LL, 1000000007LL, 1000000008LL, 1000000009LL},
  };
  for (auto z : boundary) {
    check(z[0], z[1], z[2], z[3]);
    ++checked;
  }

  mt19937_64 rng(0xC7A18ULL);
  for (int tc = 0; tc < 800; ++tc) {
    ll m1 = 1 + rng() % 90, m2 = 1 + rng() % 90;
    ll x1 = static_cast<ll>(rng() % (4 * m1 + 1)) - 2 * m1;
    ll x2 = static_cast<ll>(rng() % (4 * m2 + 1)) - 2 * m2;
    check(x1, m1, x2, m2);
    ++checked;
  }
  cout << "PASS Chinese remainder extra: " << checked
       << " exact-oracle cases\n";
}
