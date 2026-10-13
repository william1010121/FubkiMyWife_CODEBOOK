#include <bits/stdc++.h>
using namespace std;
#define int long long
#define sz(a) ((int)(a).size())
const int mod = 10000007;
string s; int dp[64][2][64];
#include "../../codebook/9_Else/digit_dp.cpp"
signed main() {
  // calc(x) = prod_{v=1..x} popcount(v) mod p; calc(0) = 0.
  assert(calc(0) == 0);
  int want = 1;
  for (int x = 1; x <= 200000; ++x) {
    want = want * __builtin_popcountll(x) % mod;
    if (x <= 5000 || x % 997 == 0) assert(calc(x) == want);
  }
  mt19937_64 rng(1);
  for (int t = 0; t < 20; ++t) { // brute over a random window
    int x = rng() % 3000000 + 1, w = 1;
    for (int v = 1; v <= x; ++v) w = w * __builtin_popcountll(v) % mod;
    assert(calc(x) == w);
  }
  cout << "digit_dp_master PASS\n";
}
