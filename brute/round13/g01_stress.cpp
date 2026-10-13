#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace qr {
#include "../../codebook/6_Math/QuadraticResidue.cpp"
}
constexpr int N = 151;
namespace clique {
#include "../../codebook/2_Graph/NumberofMaximalClique.cpp"
}
int main() {
  mt19937 rng(13); srand(13);
  for (int p : {998244353, 1000000007, INT_MAX}) {
    clock_t start = clock();
    for (int i = 0; i < 200000; ++i) {
      ll x = rng() % p;
      int a = x * x % p, root = qr::QuadraticResidue(a, p);
      if (root < 0 || root >= p || 1LL * root * root % p != a)
        throw runtime_error("Cipolla known square stress");
    }
    cout << "Cipolla " << p << ": PASS, CPU ms="
         << 1000. * (clock() - start) / CLOCKS_PER_SEC << '\n';
  }
  clique::BronKerbosch graph; graph.init(150);
  for (int u = 1; u <= 150; ++u)
    for (int v = u + 1; v <= 150; ++v) graph.add_edge(u, v);
  bitset<N> P, X; P[1] = true;
  for (int v = 2; v <= 150; ++v) X[v] = true;
  graph.S = 7; clock_t start = clock();
  // Every candidate clique extends through X, so none is maximal.
  for (int i = 0; i < 200000; ++i) graph.dfs(P, X);
  if (graph.S != 7 || graph.solve() != 1)
    throw runtime_error("complete graph with exclusions");
  cout << "Universal X pivot: PASS, CPU ms="
       << 1000. * (clock() - start) / CLOCKS_PER_SEC << '\n';
}
