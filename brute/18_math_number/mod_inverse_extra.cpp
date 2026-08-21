#include <bits/stdc++.h>
using namespace std;

int N, mod;
vector<int> inv;

long long inverse_oracle(long long a, long long m) {
  long long b = m, u = 1, v = 0;
  while (b) {
    long long t = a / b;
    tie(a, b) = make_pair(b, a - t * b);
    tie(u, v) = make_pair(v, u - t * v);
  }
  u %= m;
  if (u < 0) u += m;
  return u;
}

void run_case(int modulus, int limit) {
  mod = modulus;
  N = limit;
  inv.assign(N + 1, 0);
  {
#include "../../codebook/6_Math/Mod_Inverse.cpp"
  }
  for (int i = 1; i <= N; ++i) {
    long long want = inverse_oracle(i, mod);
    if (inv[i] != want || 1LL * i * inv[i] % mod != 1) {
      cerr << "Mod_Inverse mismatch mod=" << mod << " i=" << i
           << " want=" << want << " got=" << inv[i] << '\n';
      exit(1);
    }
  }
}

int main() {
  for (int modulus : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
                      41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83,
                      89, 97, 101, 127, 257, 509, 997})
    run_case(modulus, modulus - 1);
  run_case(1009, 1008);
  run_case(65537, 65536);
  run_case(999983, 999982);
  run_case(1000003, 1000002);
  run_case(1000000007, 250000);
  run_case(2147483647, 100000);
  cout << "PASS Mod_Inverse extra: small full tables and large prime prefixes\n";
}
