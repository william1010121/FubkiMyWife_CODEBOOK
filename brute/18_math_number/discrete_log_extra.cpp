#include <bits/stdc++.h>
using namespace std;

int fpow(int a, int e, int m) {
  long long result = 1 % m;
  for (; e; e >>= 1, a = static_cast<int>(1LL * a * a % m))
    if (e & 1) result = result * a % m;
  return static_cast<int>(result);
}

#include "../../codebook/6_Math/DiscreteLog.cpp"

int direct_log(int x, int y, int m) {
  if (m == 1) return 0;
  vector<char> seen(m, false);
  int cur = 1 % m;
  for (int k = 0; !seen[cur]; ++k) {
    if (cur == y) return k;
    seen[cur] = true;
    cur = static_cast<int>(1LL * cur * x % m);
  }
  return -1;
}

void check_three(int x, int y, int m) {
  int want = direct_log(x, y, m);
  int got = DiscreteLog(x, y, m);
  if (got != want) {
    cerr << "DiscreteLog mismatch x=" << x << " y=" << y << " m=" << m
         << " want=" << want << " got=" << got << '\n';
    exit(1);
  }
}

int main() {
  long long checked = 0;
  for (int m = 1; m <= 150; ++m)
    for (int x = 0; x < m; ++x)
      for (int y = 0; y < m; ++y) {
        check_three(x, y, m);
        ++checked;
      }

  mt19937 rng(0xD15C18ULL);
  for (int tc = 0; tc < 260; ++tc) {
    int m = 1 + rng() % 5000;
    int x = rng() % m, y = rng() % m;
    check_three(x, y, m);
    ++checked;
  }

  const vector<int> near_step = {31991, 32000, 32003, 32749, 40009};
  for (int m : near_step) {
    for (int x : {0, 1, 2, m - 1}) {
      for (int k : {0, 1, 2, m / 2, m - 1}) {
        int y = fpow(x, k, m);
        check_three(x, y, m);
        ++checked;
      }
    }
  }

  // The four-argument helper promises an exponent for s*x^k == y on
  // solvable unit-group instances; checking the equation avoids assuming a
  // particular representative when the period is not m-1.
  const vector<int> primes = {5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
                              41, 43, 47, 53, 59, 61, 67, 71};
  for (int tc = 0; tc < 700; ++tc) {
    int m = primes[rng() % primes.size()];
    int s = rng() % m, x = 1 + rng() % (m - 1), k = rng() % (m - 1);
    int y = static_cast<int>(1LL * s * fpow(x, k, m) % m);
    int got = DiscreteLog(s, x, y, m);
    if (got < 0 || static_cast<int>(1LL * s * fpow(x, got, m) % m) != y) {
      cerr << "four-argument DiscreteLog mismatch s=" << s << " x=" << x
           << " y=" << y << " m=" << m << " got=" << got << '\n';
      return 1;
    }
    ++checked;
  }

  // INT_MAX is prime, and these are all prime factors of INT_MAX-1.
  // Prove that 7 has full order before using known exponents as the oracle.
  const int large_prime = INT_MAX;
  for (int q : {2, 3, 7, 11, 31, 151, 331})
    if (fpow(7, (large_prime - 1) / q, large_prime) == 1) return 1;
  for (int k : {101, 46340, 46341, 46342, INT_MAX - 2}) {
    int y = fpow(7, k, large_prime);
    if (DiscreteLog(7, y, large_prime) != k) {
      cerr << "INT_MAX exponent mismatch k=" << k << '\n';
      return 1;
    }
    ++checked;
  }
  if (DiscreteLog(1, 7, 1, large_prime) != large_prime - 1) return 1;
  ++checked;
  for (int m : {46340 * 46340, INT_MAX - 1, INT_MAX}) {
    // x=-1 has only {1,-1} in its orbit; y=2 is impossible.
    if (DiscreteLog(m - 1, 2, m) != -1 ||
        DiscreteLog(1, m - 1, 2, m) != -1) return 1;
    ++checked;
  }
  cout << "PASS DiscreteLog extra: " << checked
       << " exhaustive, seeded, sqrt-boundary, and INT_MAX cases\n";
}
