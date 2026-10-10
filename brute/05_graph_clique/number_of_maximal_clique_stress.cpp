#include <bits/stdc++.h>
using namespace std;
constexpr int N = 151;
#include "../../codebook/2_Graph/NumberofMaximalClique.cpp"

int main() {
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
