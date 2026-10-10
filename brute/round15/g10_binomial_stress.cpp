#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#include "../../codebook/6_Math/Mod_Arithmetic.cpp"
#include "../../codebook/6_Math/Mod_Binomial.cpp"

int main() {
  mt19937 rng(151010);
  constexpr int N = 200000, Q = 2000000;
  for (ll p : {1000000007LL, 2147483647LL, 9223372036854775783LL}) {
    clock_t t = clock(); ModBinomial b(N, p);
    for (int q = 0; q < Q; ++q) {
      int n = 1 + rng()%N, k = rng()%(n + 1);
      ll x = b.C(n, k), left = b.C(n - 1, k),
         right = b.C(n - 1, k - 1);
      assert(x == b.C(n, n - k));
      assert(x == (__int128(left) + right)%p);
    }
    assert(b.C(INT_MIN, 0) == 0 && b.C(0, INT_MIN) == 0);
    assert(b.C(N, INT_MAX) == 0);
    cout << p << ": " << 1000.*(clock() - t)/CLOCKS_PER_SEC
         << " CPU ms; Pascal/symmetry stress PASS\n";
  }
  // C(p^d-1,k) = (-1)^k for odd p, with dense large digits.
  ModBinomial b(1008, 1009);
  ll n = 1009LL*1009*1009*1009*1009 - 1;
  for (int q = 0; q < 200000; ++q) {
    ll k = (ll(rng())<<31 | rng()) % (n + 1);
    assert(b.lucas(n, k) == (k%2 ? 1008 : 1));
  }
  cout << "full-table Lucas stress PASS\n";
}
