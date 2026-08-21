#include <bits/stdc++.h>
using namespace std;

#define N 12
// The template requires the OR-zeta transform, despite the historical name.
static void fwt(int *a, int n, int) {
  for (int bit = 1; bit < n; bit <<= 1)
    for (int mask = 0; mask < n; ++mask)
      if (mask & bit) a[mask] += a[mask ^ bit];
}
#include "../../codebook/2_Graph/Minimum_Clique_Cover.cpp"

static int oracle(const vector<unsigned long long> &adj) {
  const int n = (int)adj.size();
  const int lim = 1 << n;
  vector<char> clique(lim, false);
  clique[0] = true;
  for (int mask = 1; mask < lim; ++mask) {
    const int bit = mask & -mask;
    const int v = __builtin_ctz((unsigned)bit);
    const int rest = mask ^ bit;
    clique[mask] = clique[rest] &&
                   ((adj[v] & (unsigned long long)rest) ==
                    (unsigned long long)rest);
  }

  vector<int> dp(lim, n + 1);
  dp[0] = 0;
  for (int mask = 1; mask < lim; ++mask) {
    const int first = mask & -mask;
    for (int sub = mask; sub; sub = (sub - 1) & mask)
      if ((sub & first) && clique[sub])
        dp[mask] = min(dp[mask], dp[mask ^ sub] + 1);
  }
  return dp.back();
}

static void add_graph(Clique_Cover &cc,
                      const vector<unsigned long long> &adj) {
  const int n = (int)adj.size();
  for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
    if (adj[u] >> v & 1ULL) {
      cc.add_edge(u, v);
      cc.add_edge(u, v);
      if ((u + v) & 1) cc.add_edge(v, u);
    }
}

static void check(const vector<unsigned long long> &adj, int tc) {
  const int want = oracle(adj);
  Clique_Cover cc;
  cc.init((int)adj.size());
  add_graph(cc, adj);
  const int got = cc.solve();
  if (got != want) {
    cerr << "Minimum_Clique_Cover mismatch case=" << tc
         << " n=" << adj.size() << " got=" << got
         << " want=" << want << '\n';
    exit(1);
  }
}

int main() {
  int cases = 0;
  check({}, ++cases);
  for (int n = 1; n <= 11; ++n) {
    check(vector<unsigned long long>(n), ++cases);
    vector<unsigned long long> complete(n);
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      complete[u] |= 1ULL << v, complete[v] |= 1ULL << u;
    check(complete, ++cases);
  }

  // Exhaust every simple graph through six vertices, including all isolated
  // and all duplicate-edge insertion patterns in add_graph().
  for (int n = 1; n <= 6; ++n) {
    const int edge_count = n * (n - 1) / 2;
    for (unsigned long long mask = 0; mask < (1ULL << edge_count); ++mask) {
      vector<unsigned long long> adj(n);
      int bit = 0;
      for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v, ++bit)
        if (mask >> bit & 1ULL)
          adj[u] |= 1ULL << v, adj[v] |= 1ULL << u;
      check(adj, ++cases);
    }
  }

  mt19937 rng(0xCC0FE2u);
  for (int tc = 0; tc < 900; ++tc) {
    const int n = 7 + (int)(rng() % 5);
    vector<unsigned long long> adj(n);
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      if (rng() % 100 < 50)
        adj[u] |= 1ULL << v, adj[v] |= 1ULL << u;
    check(adj, ++cases);
  }
  cout << "PASS Minimum_Clique_Cover cases=" << cases
       << " oracle=exact-subset-partition-dp\n";
}
