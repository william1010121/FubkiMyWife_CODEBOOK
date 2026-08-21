#include <bits/stdc++.h>
using namespace std;

#define N 13
#define ALL(v) (v).begin(), (v).end()
#define SZ(v) ((int)(v).size())
#define pb push_back
#include "../../codebook/2_Graph/Maximum_Clique_Dyn.cpp"

static int oracle(const vector<unsigned long long> &adj) {
  const int n = (int)adj.size();
  int best = 0;
  for (unsigned long long mask = 0; mask < (1ULL << n); ++mask) {
    bool clique = true;
    for (int v = 0; v < n && clique; ++v) if (mask >> v & 1ULL) {
      const auto without_v = mask ^ (1ULL << v);
      clique = (adj[v] & without_v) == without_v;
    }
    if (clique) best = max(best, __builtin_popcountll(mask));
  }
  return best;
}

static void add_graph(MaxClique &mc,
                      const vector<unsigned long long> &adj) {
  const int n = (int)adj.size();
  for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
    if (adj[u] >> v & 1ULL) {
      mc.add_edge(u, v);
      mc.add_edge(u, v);
      if ((u ^ v) & 1) mc.add_edge(v, u);
    }
}

static void check(const vector<unsigned long long> &adj, int tc) {
  const int n = (int)adj.size();
  const int want = oracle(adj);
  MaxClique mc;
  mc.init(n);
  add_graph(mc, adj);
  const int got = mc.solve();
  if (got != want || got < 0 || got > n) {
    cerr << "Maximum_Clique_Dyn mismatch case=" << tc
         << " n=" << n << " got=" << got << " want=" << want << '\n';
    exit(1);
  }

  vector<int> seen(n);
  for (int i = 0; i < got; ++i) {
    const int v = mc.sol[i];
    if (v < 0 || v >= n || seen[v]++) {
      cerr << "Maximum_Clique_Dyn invalid solution case=" << tc << '\n';
      exit(1);
    }
    for (int j = 0; j < i; ++j)
      if (!(adj[v] >> mc.sol[j] & 1ULL)) {
        cerr << "Maximum_Clique_Dyn non-clique solution case=" << tc << '\n';
        exit(1);
      }
  }
}

int main() {
  int cases = 0;
  check({}, ++cases); // empty vertex set is handled by this dynamic variant.
  for (int n = 1; n <= 12; ++n) {
    check(vector<unsigned long long>(n), ++cases);
    vector<unsigned long long> complete(n);
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      complete[u] |= 1ULL << v, complete[v] |= 1ULL << u;
    check(complete, ++cases);
  }

  // Exhaust every simple graph through six vertices.
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

  mt19937 rng(0xD1C7C1u);
  for (int tc = 0; tc < 1500; ++tc) {
    const int n = 7 + (int)(rng() % 6);
    vector<unsigned long long> adj(n);
    const int percent = (int)(rng() % 101);
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      if ((int)(rng() % 100) < percent)
        adj[u] |= 1ULL << v, adj[v] |= 1ULL << u;
    check(adj, ++cases);
  }
  cout << "PASS Maximum_Clique_Dyn cases=" << cases
       << " oracle=exhaustive-subset-clique-check-and-solution-validation\n";
}
