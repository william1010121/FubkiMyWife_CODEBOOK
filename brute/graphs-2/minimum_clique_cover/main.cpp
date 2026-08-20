#include <bits/stdc++.h>
using namespace std;
#define N 11
#define ALL(v) (v).begin(), (v).end()
#define SZ(v) ((int)(v).size())
#define pb push_back
static void fwt(int *a, int n, int) {
  for (int bit = 1; bit < n; bit <<= 1)
    for (int mask = 0; mask < n; ++mask)
      if (mask & bit) a[mask] += a[mask ^ bit];
}
#include "../../../codebook/2_Graph/Minimum_Clique_Cover.cpp"

static int oracle(const vector<unsigned> &adj) {
  int n = (int)adj.size(), lim = 1 << n;
  vector<char> clique(lim, 0);
  clique[0] = 1;
  for (int mask = 1; mask < lim; ++mask) {
    int b = __builtin_ctz(mask), rest = mask ^ (1 << b);
    clique[mask] = clique[rest] && ((adj[b] & (unsigned)rest) == (unsigned)rest);
  }
  vector<int> dp(lim, n + 1);
  dp[0] = 0;
  for (int mask = 1; mask < lim; ++mask) {
    int bit = mask & -mask;
    for (int sub = mask; sub; sub = (sub - 1) & mask)
      if ((sub & bit) && clique[sub]) dp[mask] = min(dp[mask], dp[mask ^ sub] + 1);
  }
  return dp.back();
}

int main() {
  mt19937 rng(0x434F5645);
  int cases = 0;
  for (int n = 1; n <= 10; ++n) for (int tc = 0; tc < 250; ++tc) {
    vector<unsigned> adj(n);
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
      if ((int)(rng() % 100) < 45) adj[i] |= 1u << j, adj[j] |= 1u << i;
    Clique_Cover cc;
    cc.init(n);
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
      if (adj[i] >> j & 1u) cc.add_edge(i, j);
    int got = cc.solve(), want = oracle(adj);
    ++cases;
    if (got != want) {
      cerr << "minimum_clique_cover mismatch case=" << cases << " n=" << n
           << " got=" << got << " want=" << want << '\n';
      return 1;
    }
  }
  cout << "PASS cases=" << cases << " oracle=exact-subset-clique-cover-dp\n";
}
