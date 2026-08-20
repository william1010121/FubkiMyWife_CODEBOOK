#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#include "../../../codebook/2_Graph/SCC.cpp"
#include "../../../codebook/2_Graph/2SAT.cpp"

static bool lit(int x, int mask, int n) {
  bool value = (mask >> (x % n)) & 1;
  return x < n ? value : !value;
}

static void fail(int tc, int n, const vector<pair<int,int>> &cl, const string &why) {
  cerr << "2SAT failure tc=" << tc << ": " << why << "\n";
  cerr << n << " " << cl.size() << "\n";
  for (auto [a, b] : cl) cerr << a << " " << b << "\n";
  exit(1);
}

int main() {
  mt19937 rng(0x2A7A7);
  for (int tc = 0; tc < 6000; ++tc) {
    int n = 1 + rng() % 7;
    int m = rng() % 16;
    vector<pair<int,int>> cl;
    SAT s(n);
    for (int i = 0; i < m; ++i) {
      int a = rng() % (2 * n), b = rng() % (2 * n);
      cl.push_back({a, b}); s.add_clause(a, b);
    }
    bool brute_sat = false; int witness = 0;
    for (int mask = 0; mask < (1 << n); ++mask) {
      bool ok = true;
      for (auto [a, b] : cl) ok &= lit(a, mask, n) || lit(b, mask, n);
      if (ok) brute_sat = true, witness = mask;
    }
    bool got = s.solve();
    if (got != brute_sat) fail(tc, n, cl, "satisfiability differs");
    if (got) {
      for (auto [a, b] : cl) {
        bool x = a < n ? s.istrue[a] : !s.istrue[a - n];
        bool y = b < n ? s.istrue[b] : !s.istrue[b - n];
        if (!(x || y)) fail(tc, n, cl, "returned assignment violates clause");
      }
      (void)witness;
    }
  }
  cout << "2SAT PASS 6000 randomized formulas\n";
}
