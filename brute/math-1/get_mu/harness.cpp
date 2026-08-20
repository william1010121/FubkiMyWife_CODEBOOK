#include <bits/stdc++.h>
using namespace std;
int n, tot;
vector<int> p;
vector<char> flg;
vector<int> mu;
#include "../../../codebook/6_Math/getMu.cpp"

int mobius_brute(int x) {
  int distinct = 0;
  for (int d = 2; 1LL * d * d <= x; ++d) if (x % d == 0) {
    int e = 0;
    while (x % d == 0) x /= d, ++e;
    if (e >= 2) return 0;
    ++distinct;
  }
  if (x > 1) ++distinct;
  return (distinct & 1) ? -1 : 1;
}
int main() {
  for (n = 1; n <= 1000; ++n) {
    tot = 0;
    p.assign(n + 1, 0);
    flg.assign(n + 1, 0);
    mu.assign(n + 1, 0);
    getMu();
    for (int x = 1; x <= n; ++x) if (mu[x] != mobius_brute(x)) {
      cerr << "getMu mismatch n=" << n << " x=" << x
           << " got=" << mu[x] << " want=" << mobius_brute(x) << '\n';
      return 1;
    }
  }
  cout << "PASS getMu all prefixes <=1000\n";
}
