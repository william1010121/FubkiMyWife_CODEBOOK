#include <bits/stdc++.h>
using namespace std;
#define N 12
#define ALL(v) (v).begin(), (v).end()
#define SZ(v) ((int)(v).size())
#define pb push_back
#include "../../../codebook/2_Graph/Maximum_Clique_Dyn.cpp"

static int oracle(const vector<unsigned> &adj) {
  int n = (int)adj.size(), ans = 0;
  for (unsigned mask = 0; mask < (1u << n); ++mask) {
    bool ok = true;
    for (int i = 0; i < n && ok; ++i) if (mask >> i & 1u)
      if ((mask & ~(1u << i) & ~adj[i]) != 0) ok = false;
    if (ok) ans = max(ans, __builtin_popcount(mask));
  }
  return ans;
}

int main() {
  mt19937 rng(0x434C4951);
  int cases = 0;
  for (int n = 1; n <= 12; ++n) {
    for (int tc = 0; tc < 350; ++tc) {
      vector<unsigned> adj(n);
      for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
        if ((int)(rng() % 100) < 43) adj[i] |= 1u << j, adj[j] |= 1u << i;
      MaxClique mc;
      mc.init(n);
      for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
        if (adj[i] >> j & 1u) mc.add_edge(i, j);
      int got = mc.solve(), want = oracle(adj);
      ++cases;
      if (got != want) {
        cerr << "maximum_clique_dyn mismatch case=" << cases << " n=" << n
             << " got=" << got << " want=" << want << '\n';
        return 1;
      }
    }
  }
  cout << "PASS cases=" << cases << " oracle=enumerated-vertex-subsets\n";
}
