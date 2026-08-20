#include <bits/stdc++.h>
using namespace std;
int N, mod;
vector<int> inv;

bool prime(int x) {
  if (x < 2) return false;
  for (int d = 2; d * d <= x; ++d) if (x % d == 0) return false;
  return true;
}
int main() {
  for (mod = 2; mod <= 300; ++mod) if (prime(mod)) {
    for (N = 1; N < mod; ++N) {
      inv.assign(N + 1, 0);
      auto build = [&]() {
        #include "../../../codebook/6_Math/Mod_Inverse.cpp"
      };
      build();
      for (int i = 1; i <= N; ++i)
        if (1LL * i * inv[i] % mod != 1) {
          cerr << "inverse mismatch mod=" << mod << " i=" << i << '\n';
          return 1;
        }
    }
  }
  cout << "PASS Mod_Inverse all primes <=300\n";
}
