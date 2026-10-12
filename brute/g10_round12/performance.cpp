#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define pb push_back
#define ALL(a) (a).begin(), (a).end()
#define SZ(a) ((int)(a).size())
#include "codebook/9_Else/All_LCS.cpp"
#include "codebook/9_Else/min_plus_convolution.cpp"
namespace cir {
const int N = 128;
const ll INF = 1LL << 60;
#include "codebook/4_Flow_Matching/MinCostCirculation.cpp"
}
int main() {
  string s, t;
  for (int i = 0; i < 16; ++i) {
    s += char(32 + 2 * i), s += char(33 + 2 * i);
    t += char(33 + 2 * i), t += char(32 + 2 * i);
  }
  vector<string> want;
  for (int mask = 0; mask < (1 << 16); ++mask) {
    string x;
    for (int i = 0; i < 16; ++i)
      x += char(32 + 2 * i + (mask >> i & 1));
    want.pb(x);
  }
  sort(ALL(want));
  assert(all_lcs(s, t) == want);
  // Linear a: prefix/suffix minima give an independent O(n+m) oracle.
  int n = 200000;
  vector<ll> a(n), b(n), prefix(n), suffix(n);
  mt19937 rng(712312);
  for (int i = 0; i < n; ++i)
    a[i] = 7LL * i, b[i] = int(rng() % 1000000) - 500000;
  for (int i = 0; i < n; ++i) {
    prefix[i] = b[i] - 7LL * i;
    if (i) prefix[i] = min(prefix[i], prefix[i - 1]);
  }
  for (int i = n - 1; i >= 0; --i) {
    suffix[i] = b[i] - 7LL * i;
    if (i + 1 < n) suffix[i] = min(suffix[i], suffix[i + 1]);
  }
  auto c = min_plus_convolution(a, b);
  for (int k = 0; k < 2 * n - 1; ++k)
    assert(c[k] == 7LL * k + (k < n ? prefix[k] : suffix[k - n + 1]));
  // Dense signed loops: optimal flow saturates exactly negative loops.
  auto &g = cir::mcmf;
  g.init(1);
  for (int i = 0; i < 4000; ++i) g.add_edge(0, 0, 1023, i % 3 - 1);
  g.solve(9);
  for (auto &e : g.G[0]) {
    assert(e.flow == -g.G[e.to][e.rev].flow);
    if (e.fcap) assert(e.flow == (e.cost < 0 ? e.fcap : 0));
  }
  // Interleave loops and ordinary edges; shortest paths must reset.
  g.init(128);
  for (int i = 0; i < 128; ++i) {
    g.add_edge(i, i, 1023, -1), g.add_edge(i, i, 1023, 1);
    g.add_edge(i, (i + 1) % 128, 1023, -1);
  }
  g.solve(9);
  for (int u = 0; u < 128; ++u) for (auto &e : g.G[u]) {
    assert(e.flow == -g.G[e.to][e.rev].flow);
    if (e.fcap) assert(e.flow == (e.cost < 0 ? e.fcap : 0));
  }
  cout << "PASS 65536 LCS outputs, 200k convolution, dense/mixed loops\n";
}
