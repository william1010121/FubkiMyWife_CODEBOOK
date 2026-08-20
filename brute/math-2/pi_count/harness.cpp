#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#include "../../../codebook/6_Math/PiCount.cpp"

int main() {
  const int N = 2000000;
  vector<bool> composite(N + 1);
  vector<int> pi(N + 1);
  for (int p = 2; 1LL * p * p <= N; ++p)
    if (!composite[p])
      for (int q = p * p; q <= N; q += p) composite[q] = true;
  for (int i = 1; i <= N; ++i)
    pi[i] = pi[i - 1] + (i >= 2 && !composite[i]);
  for (int n = 0; n <= 20000; ++n)
    if (PrimeCount(n) != pi[n]) {
      cerr << "small mismatch n=" << n << " want=" << pi[n]
           << " got=" << PrimeCount(n) << '\n';
      return 1;
    }
  vector<pair<ll, ll>> known = {
      {1000000000LL, 50847534LL},
      {1000000000000LL, 37607912018LL},
      {10000000000000LL, 346065536839LL},
  };
  for (auto [n, want] : known)
    if (PrimeCount(n) != want) {
      cerr << "large mismatch n=" << n << " want=" << want
           << " got=" << PrimeCount(n) << '\n';
      return 1;
    }
  cout << "PrimeCount OK: 2,000,001 sieve cases + 3 large references\n";
}
