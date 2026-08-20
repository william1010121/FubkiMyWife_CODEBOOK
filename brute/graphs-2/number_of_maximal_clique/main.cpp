#include <bits/stdc++.h>
using namespace std;
#define N 11
#define ALL(v) (v).begin(), (v).end()
#define SZ(v) ((int)(v).size())
#define pb push_back
#include "../../../codebook/2_Graph/NumberofMaximalClique.cpp"

static int oracle(const vector<unsigned> &adj) {
  int n = (int)adj.size(), ans = 0;
  for (unsigned mask = 1; mask < (1u << n); ++mask) {
    bool clique = true;
    for (int i = 0; i < n && clique; ++i) if (mask >> i & 1u)
      if ((mask & ~(1u << i) & ~adj[i]) != 0) clique = false;
    if (!clique) continue;
    bool maximal = true;
    for (int v = 0; v < n; ++v) if (!(mask >> v & 1u) &&
        (adj[v] & mask) == mask) maximal = false;
    ans += maximal;
  }
  return ans;
}

int main() {
  mt19937 rng(0x4D415849);
  int cases = 0;
  for (int n = 1; n <= 10; ++n) for (int tc = 0; tc < 350; ++tc) {
    vector<unsigned> adj(n);
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
      if ((int)(rng() % 100) < 47) adj[i] |= 1u << j, adj[j] |= 1u << i;
    BronKerbosch bk;
    bk.init(n);
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
      if (adj[i] >> j & 1u) bk.add_edge(i + 1, j + 1);
    int got = bk.solve(), want = oracle(adj);
    ++cases;
    if (got != want) {
      cerr << "number_of_maximal_clique mismatch case=" << cases << " n=" << n
           << " got=" << got << " want=" << want << '\n';
      return 1;
    }
  }
  cout << "PASS cases=" << cases << " oracle=enumerated-maximal-subsets\n";
}
