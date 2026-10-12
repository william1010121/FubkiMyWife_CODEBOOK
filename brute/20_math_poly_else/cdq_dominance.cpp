#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define SZ(a) ((int)a.size())
#define ALL(v) v.begin(), v.end()
#define pb push_back
#include "../../codebook/9_Else/CDQ_Dominance.cpp"

int main() {
  mt19937 rng(7);
  for (int it = 0; it < 30000; ++it) {
    int n = rng() % 25, k = rng() % 6, V = 1 + rng() % 5;
    vector<vector<int>> a(n, vector<int>(k));
    for (auto &p : a) for (auto &x : p) x = (int)(rng() % V) - 2;
    auto got = dominance(a);
    for (int i = 0; i < n; ++i) {
      long long want = 0;
      for (int j = 0; j < n; ++j) if (j != i) {
        bool ok = true;
        for (int t = 0; t < k; ++t) ok &= a[j][t] <= a[i][t];
        want += ok;
      }
      if (got[i] != want) { printf("FAIL n=%d k=%d i=%d got=%lld want=%lld\n", n, k, i, got[i], want); return 1; }
    }
  }
  puts("OK");
}
