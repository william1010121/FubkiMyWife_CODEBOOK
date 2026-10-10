#include <bits/stdc++.h>
using namespace std;
constexpr int N = 20;
static void fwt(int *a, int n, int) {
  for (int bit = 1; bit < n; bit <<= 1)
    for (int mask = 0; mask < n; ++mask)
      if (mask & bit) a[mask] += a[mask ^ bit];
}
#include "../../codebook/2_Graph/Minimum_Clique_Cover.cpp"

[[gnu::noinline]] static void reset(Clique_Cover &cc) { cc.init(N); }

static void structured_boundaries() {
  static Clique_Cover cc;
  for (int n : {20, 0, 1, 19, 2, 20}) {
    for (bool cross : {false, true}) for (int groups : {1, 2, 5, 20}) {
      cc.init(n);
      for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
        if ((u % groups != v % groups) == cross)
          cc.add_edge(u, v), cc.add_edge(v, u);
      int want = cross ? (n + groups - 1) / groups : min(n, groups);
      if (cc.solve() != want || cc.solve() != want)
        throw runtime_error("cover multipartite/disjoint cliques");
    }
    if (n < 3) continue;
    cc.init(n);
    for (int u = 0; u < n; ++u) cc.add_edge(u, (u + 1) % n);
    if (cc.solve() != (n + 1) / 2) throw runtime_error("cycle cover");
  }
}
int main() {
  structured_boundaries();
  static Clique_Cover cc;
  // init should touch adjacency only; solve fills all scratch arrays.
  for (int i = 0; i < 1024; ++i) reset(cc);
  for (int u = 0; u < N; ++u)
    for (int v = u + 1; v < N; ++v) cc.add_edge(u, v);
  if (cc.solve() != 1 || cc.solve() != 1)
    throw runtime_error("complete graph / repeated solve");
  cc.init(N);
  if (cc.solve() != N) throw runtime_error("empty graph");
  cout << "PASS cover n=20 complete/empty, repeated init/solve\n";
}
