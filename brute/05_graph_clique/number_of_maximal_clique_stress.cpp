#include <bits/stdc++.h>
using namespace std;
constexpr int N = 151;
#define SZ(v) ((int)(v).size())
#include "../../codebook/2_Graph/NumberofMaximalClique.cpp"


static void structured_boundaries() {
  static BronKerbosch bk;
  for (int n : {150, 0, 1, 100, 2, 150}) {
    bk.init(n);
    for (int u = 1; u <= n; ++u) for (int v = u + 1; v <= n; ++v)
      if ((u <= n / 2) != (v <= n / 2))
        bk.add_edge(u, v), bk.add_edge(v, u);
    int want = n < 2 ? 1 : (n / 2) * (n - n / 2);
    if (bk.solve() != want || bk.solve() != want)
      throw runtime_error("maximal bipartite cliques / repeated solve");
    bk.init(n);
    for (int u = 1; u <= n; ++u) for (int v = u + 1; v <= n; ++v)
      if ((u - 1) / 3 == (v - 1) / 3) bk.add_edge(u, v);
    if (bk.solve() != (n ? (n + 2) / 3 : 1))
      throw runtime_error("disjoint maximal cliques");
  }
}
int main() {
  structured_boundaries();
  static BronKerbosch bk;
  bk.init(150);
  for (int u = 1; u <= 150; ++u)
    for (int v = u + 1; v <= 150; ++v) bk.add_edge(u, v);
  if (bk.solve() != 1) throw runtime_error("complete graph");
  bk.init(30);
  for (int u = 1; u <= 30; ++u)
    for (int v = u + 1; v <= 30; ++v)
      if ((u - 1) / 3 != (v - 1) / 3) bk.add_edge(u, v);
  if (bk.solve() != 59049) throw runtime_error("Moon-Moser graph");
  cout << "PASS maximal cliques complete-150 and 3^10 stress\n";
}
