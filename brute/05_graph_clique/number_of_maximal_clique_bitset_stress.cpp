#include <bits/stdc++.h>
using namespace std;
constexpr int N = 151;
#define SZ(v) ((int)(v).size())
#include "../../codebook/2_Graph/NumberofMaximalClique.cpp"

int main() {
  static BronKerbosch bk;
  // Audit the retained vector helper against an independent P/X oracle.
  int cases = 0, n = 4;
  for (int edges = 0; edges < (1 << 6); ++edges) {
    bk.init(n);
    bool adj[4][4] = {};
    int e = 0;
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      if (edges >> e++ & 1) {
        bk.add_edge(u + 1, v + 1); adj[u][v] = adj[v][u] = true;
      }
    for (int assignment = 0; assignment < 81; ++assignment) {
      int code = assignment, pm = 0, xm = 0;
      vector<int> P, X;
      for (int v = 0; v < n; ++v, code /= 3) {
        if (code % 3 == 1) pm |= 1 << v, P.push_back(v + 1);
        if (code % 3 == 2) xm |= 1 << v, X.push_back(v + 1);
      }
      int want = 0;
      for (int sub = 0; sub < (1 << n); ++sub) {
        if (sub & ~pm) continue;
        bool clique = true, maximal = true;
        for (int u = 0; u < n; ++u) if (sub >> u & 1)
          for (int v = u + 1; v < n; ++v)
            if ((sub >> v & 1) && !adj[u][v]) clique = false;
        if (!clique) continue;
        for (int u = 0; u < n; ++u)
          if (((pm | xm) >> u & 1) && !(sub >> u & 1)) {
            bool extends = true;
            for (int v = 0; v < n; ++v)
              if ((sub >> v & 1) && !adj[u][v]) extends = false;
            if (extends) maximal = false;
          }
        want += maximal;
      }
      bk.S = 7; bk.dfs_vec(P, X);
      if (bk.S != 7 + want) throw runtime_error("P/X helper oracle");
      ++cases;
    }
  }
  // Word-boundary vertices, shrinking init and repeated solving.
  for (int size : {150, 0, 1, 64, 65, 127, 128, 129, 150}) {
    bk.init(size);
    for (int u = 1; u <= size; ++u)
      for (int v = u + 1; v <= size; ++v) bk.add_edge(u, v);
    if (bk.solve() != 1 || bk.solve() != 1)
      throw runtime_error("word-boundary complete graph");
  }
  bk.init(36);
  for (int u = 1; u <= 36; ++u) for (int v = u + 1; v <= 36; ++v)
    if ((u - 1) / 3 != (v - 1) / 3) bk.add_edge(u, v);
  if (bk.solve() != 531441) throw runtime_error("3^12 stress");
  cout << "PASS P/X oracle cases=" << cases << " and 3^12 stress\n";
}
