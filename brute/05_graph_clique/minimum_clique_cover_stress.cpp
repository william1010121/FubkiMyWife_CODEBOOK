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
int main() {
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
