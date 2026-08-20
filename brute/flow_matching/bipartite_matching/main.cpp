#include <bits/stdc++.h>
using namespace std;

#define N 12
#define SZ(x) ((int)(x).size())
#define pb push_back
#include "../../../codebook/4_Flow_Matching/Bipartite_Matching.cpp"

static int brute(const vector<vector<int>> &g, int u, int mask) {
  if (u == (int)g.size()) return 0;
  int ans = brute(g, u + 1, mask);
  for (int v : g[u]) if (!(mask >> v & 1))
    ans = max(ans, 1 + brute(g, u + 1, mask | (1 << v)));
  return ans;
}

static void check(const vector<vector<int>> &g, int r, int tc) {
  int l = (int)g.size();
  Bipartite_Matching bm;
  bm.init(l, r);
  for (int u = 0; u < l; ++u) for (int v : g[u]) bm.add_edge(u, v);
  int got = bm.matching(), want = brute(g, 0, 0);
  if (got != want) {
    cerr << "Bipartite mismatch on case " << tc << ": got=" << got
         << " want=" << want << '\n';
    exit(1);
  }
}

int main() {
  int tc = 0;
  // Exhaust every bipartite graph up to 4 x 4.
  for (int l = 1; l <= 4; ++l) for (int r = 1; r <= 4; ++r) {
    int bits = l * r;
    for (int mask = 0; mask < (1 << bits); ++mask) {
      vector<vector<int>> g(l);
      for (int u = 0; u < l; ++u) for (int v = 0; v < r; ++v)
        if (mask >> (u * r + v) & 1) g[u].push_back(v);
      check(g, r, tc++);
    }
  }
  mt19937 rng(0xB1A71E);
  for (int z = 0; z < 2500; ++z) {
    int l = 1 + rng() % 8, r = 1 + rng() % 8;
    vector<vector<int>> g(l);
    for (int u = 0; u < l; ++u) for (int v = 0; v < r; ++v)
      if (rng() % 2) g[u].push_back(v);
    check(g, r, tc++);
  }
  cout << "PASS " << tc << " exhaustive/random bipartite graphs\n";
}
