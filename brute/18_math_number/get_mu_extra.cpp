#include <bits/stdc++.h>
using namespace std;

int n, tot;
vector<int> p;
vector<char> flg;
vector<int> mu;

#include "../../codebook/6_Math/getMu.cpp"

vector<int> mobius_oracle(int limit) {
  vector<int> spf(limit + 1);
  for (int d = 2; d <= limit; ++d)
    if (!spf[d])
      for (int x = d; x <= limit; x += d)
        if (!spf[x]) spf[x] = d;

  vector<int> answer(limit + 1);
  answer[1] = 1;
  for (int x = 2; x <= limit; ++x) {
    int y = x, distinct = 0;
    bool square = false;
    while (y > 1) {
      int q = spf[y], exponent = 0;
      while (y % q == 0) y /= q, ++exponent;
      if (exponent >= 2) square = true;
      ++distinct;
    }
    answer[x] = square ? 0 : ((distinct & 1) ? -1 : 1);
  }
  return answer;
}

void run_case(int limit) {
  n = limit;
  tot = 0;
  p.assign(n + 1, 0);
  flg.assign(n + 1, 0);
  mu.assign(n + 1, 0);
  getMu();

  vector<int> want = mobius_oracle(n);
  for (int x = 1; x <= n; ++x) {
    if (mu[x] != want[x]) {
      cerr << "getMu mismatch n=" << n << " x=" << x
           << " want=" << want[x] << " got=" << mu[x] << '\n';
      exit(1);
    }
  }
  // The sieve's prime counter is independently checked by trial division.
  int prime_count = 0;
  for (int x = 2; x <= n; ++x) {
    bool prime = true;
    for (int d = 2; 1LL * d * d <= x; ++d)
      if (x % d == 0) {
        prime = false;
        break;
      }
    prime_count += prime;
  }
  if (tot != prime_count) {
    cerr << "getMu prime count mismatch n=" << n << " want="
         << prime_count << " got=" << tot << '\n';
    exit(1);
  }

  vector<int> divisor_sum(n + 1);
  for (int d = 1; d <= n; ++d)
    for (int x = d; x <= n; x += d) divisor_sum[x] += mu[d];
  for (int x = 1; x <= n; ++x) {
    int want_sum = (x == 1);
    if (divisor_sum[x] != want_sum) {
      cerr << "getMu divisor identity mismatch n=" << n << " x=" << x
           << " want=" << want_sum << " got=" << divisor_sum[x] << '\n';
      exit(1);
    }
  }
}

int main() {
  run_case(1);
  run_case(2);
  run_case(99991);
  run_case(500000);
  cout << "PASS getMu extra: exact factor oracle and divisor identity through 500000\n";
}
