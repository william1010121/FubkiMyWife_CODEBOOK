#include <bits/stdc++.h>
using namespace std;

int fpow(int a, int e, int m) {
  long long r = 1 % m;
  for (; e; e >>= 1, a = (long long)a * a % m) if (e & 1) r = r * a % m;
  return (int)r;
}
#include "../../../codebook/6_Math/DiscreteLog.cpp"

int brute(int x, int y, int m) {
  int cur = 1 % m;
  for (int k = 0; k <= 2 * m + 5; ++k) {
    if (cur == y) return k;
    cur = (long long)cur * x % m;
  }
  return -1;
}

int main() {
  long long checked = 0;
  for (int m = 1; m <= 12; ++m)
    for (int x = 0; x < m; ++x)
      for (int y = 0; y < m; ++y) {
        int want = brute(x, y, m);
        int got = DiscreteLog(x, y, m);
        if (got != want) {
          cerr << "mismatch x=" << x << " y=" << y << " m=" << m
               << " want=" << want << " got=" << got << '\n';
          return 1;
        }
        ++checked;
      }
  mt19937 rng(0xD10510);
  for (int tc = 0; tc < 2500; ++tc) {
    int m = 1 + rng() % 500, x = rng() % m, y = rng() % m;
    int want = brute(x, y, m), got = DiscreteLog(x, y, m);
    if (got != want) {
      cerr << "random mismatch x=" << x << " y=" << y << " m=" << m
           << " want=" << want << " got=" << got << '\n';
      return 1;
    }
    ++checked;
  }

  // The four-argument helper returns a valid exponent for s*x^k == y
  // in the unit group; it is not required to return the smallest one.
  mt19937 unit_rng(0xD10A);
  for (int tc = 0; tc < 500; ++tc) {
    int m = vector<int>{5, 7, 11, 13, 17, 19, 23, 29, 31, 37}[unit_rng() % 10];
    int s = 1 + unit_rng() % (m - 1), x = 1 + unit_rng() % (m - 1);
    int k = unit_rng() % (m - 1);
    {
          int y = (long long)s * fpow(x, k, m) % m;
          int got = DiscreteLog(s, x, y, m);
          if (got < 0 || (long long)s * fpow(x, got, m) % m != y) {
            cerr << "four-argument mismatch s=" << s << " x=" << x
                 << " y=" << y << " m=" << m << " got=" << got << '\n';
            return 1;
        }
      }
  }
  cout << "DiscreteLog OK: " << checked << " exhaustive triples + unit cases\n";
}
