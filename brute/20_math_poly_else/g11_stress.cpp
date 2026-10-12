#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define SZ(a) ((int)a.size())
#define ALL(v) v.begin(), v.end()
#define pb push_back
#include "../../codebook/9_Else/CDQ_Dominance.cpp"
#include "../../codebook/9_Else/All_LCS.cpp"
#include "../../codebook/9_Else/AdaptiveSimpson.cpp"
#include "../../codebook/9_Else/BitsetLCS.cpp"
#include "../../codebook/9_Else/min_plus_convolution.cpp"

void require(bool ok, const char *msg) {
  if (!ok) { cerr << msg << '\n'; exit(1); }
}
int main() {
  auto start = chrono::steady_clock::now();
  // Each reversed pair contributes one independent choice: 2^14 LCS.
  string s, t;
  for (int i = 0; i < 14; ++i) {
    s += char('A' + 2*i), s += char('B' + 2*i);
    t += char('B' + 2*i), t += char('A' + 2*i);
  }
  auto lcs = all_lcs(s, t);
  require(SZ(lcs) == (1 << 14), "LCS exponential output size");
  require(is_sorted(ALL(lcs)), "LCS sorted output");
  require(adjacent_find(ALL(lcs)) == lcs.end(), "LCS duplicates");
  for (const string &x : lcs) {
    require(SZ(x) == 14, "LCS length");
    for (int i = 0; i < 14; ++i)
      require(x[i] == 'A'+2*i || x[i] == 'B'+2*i, "LCS pair choice");
  }
  int calls = 0;
  auto simpson = make_simpson([&](double x) { ++calls; return x*x; });
  require(abs(simpson.eval(0.0, 1.0, 1e-12)-1.0/3) < 1e-10,
          "Simpson integral");
  require(calls == 5, "Simpson must reuse the midpoint sample");
  calls = 0;
  simpson.mix({0.0, 0.0}, {1.0, 1.0}, 0.25);
  require(calls == 0, "Simpson mix with supplied sample");

  vector<int> a(20000, -7), b(20001, -7);
  require(bitset_lcs(a, b) == 20000, "LCS long multiword carry");
  b.assign(20001, 8);
  require(bitset_lcs(a, b) == 0, "LCS disjoint alphabet");
  const int n = 100000;
  vector<vector<int>> p(n, vector<int>(4));
  for (int i = 0; i < n; ++i) fill(ALL(p[i]), i/2-50000);
  auto count = dominance(p);
  for (int i = 0; i < n; ++i)
    require(count[i] == 2*(i/2)+1, "4D dominance duplicates");
  for (auto &v : p) v = {INT_MIN, INT_MAX, 0};
  count = dominance(p);
  require(count == vector<ll>(n, n-1), "3D extreme coordinates");

  a.resize(150000), b.assign(150000, 0);
  iota(ALL(a), 0);
  auto c = min_plus_convolution(a, b);
  require(SZ(c) == 299999, "min-plus output size");
  for (int i = 0; i < SZ(c); ++i)
    require(c[i] == max(0, i-149999), "large convex min-plus");
  cout << "g11 stress PASS ("
       << chrono::duration<double>(
            chrono::steady_clock::now()-start).count()
       << " seconds)\n";
}
