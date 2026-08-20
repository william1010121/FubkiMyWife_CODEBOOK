#include <bits/stdc++.h>
using namespace std;
using u64 = uint64_t;
static u64 power(u64 a, u64 e, u64 p) {
  u64 r = 1;
  for (; e; e >>= 1, a = (u64)((__uint128_t)a * a % p))
    if (e & 1) r = (u64)((__uint128_t)r * a % p);
  return r;
}
static bool prime(u64 n) {
  if (n < 2) return false;
  for (u64 d = 2; d * d <= n; ++d) if (n % d == 0) return n == d;
  return true;
}
int main() {
  const pair<u64, u64> table[] = {
    {7681,17},{12289,11},{40961,3},{65537,3},{786433,10},
    {5767169,3},{7340033,3},{23068673,3},{469762049,3},
    {167772161,3},{104857601,3},{985661441,3},{998244353,3},
    {1107296257,10},{2013265921,31},{2810183681ULL,11},
    {2885681153ULL,3},{605028353,3}
  };
  for (auto [p, g] : table) {
    if (!prime(p) || power(g, p - 1, p) != 1) {
      cerr << "invalid NTT prime/root pair: " << p << '\n'; return 1;
    }
    u64 x = p - 1;
    vector<u64> factors;
    for (u64 d = 2; d * d <= x; ++d) if (x % d == 0) {
      factors.push_back(d); while (x % d == 0) x /= d;
    }
    if (x > 1) factors.push_back(x);
    for (u64 q : factors) if (power(g, (p - 1) / q, p) == 1) {
      cerr << "root is not primitive for " << p << '\n'; return 1;
    }
  }
  cout << "ntt_prime: PASS\n";
}
