#include <bits/stdc++.h>
using namespace std;

#define MAXN 13
#include "../../codebook/2_Graph/Maximum_Clique.cpp"

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

static void add_graph(Maximum_Clique &mc,
                      const vector<unsigned long long> &adj) {
  const int n = (int)adj.size();
  for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
    if (adj[u] >> v & 1ULL) {
      mc.add_edge(u, v);
      mc.add_edge(u, v); // parallel insertion must remain harmless.
      if ((u + v) & 1) mc.add_edge(v, u); // also exercise reversed input.
    }
}

static void check(const vector<unsigned long long> &adj, int tc) {
  const int want = oracle(adj);
  Maximum_Clique mc;
  mc.init((int)adj.size());
  add_graph(mc, adj);
  const int got = mc.solve();
  if (got != want) {
    cerr << "Maximum_Clique mismatch case=" << tc
         << " n=" << adj.size() << " got=" << got
         << " want=" << want << '\n';
    for (auto row : adj) cerr << row << '\n';
    exit(1);
  }
  // solve() is documented as a query; it must not depend on the first
  // random vertex ordering.
  if (mc.solve() != want) {
    cerr << "Maximum_Clique repeated-solve mismatch case=" << tc << '\n';
    exit(1);
  }
}

static vector<unsigned long long> random_graph(int n, mt19937 &rng,
                                                int percent) {
  vector<unsigned long long> adj(n);
  for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
    if ((int)(rng() % 100) < percent)
      adj[u] |= 1ULL << v, adj[v] |= 1ULL << u;
  return adj;
}

int main() {
  int cases = 0;

  // Empty-edge, complete, and star graphs include isolated and universal
  // vertices.  The template's valid domain is n >= 1.
  for (int n = 1; n <= 12; ++n) {
    check(vector<unsigned long long>(n), ++cases);
    vector<unsigned long long> complete(n);
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      complete[u] |= 1ULL << v, complete[v] |= 1ULL << u;
    check(complete, ++cases);
    vector<unsigned long long> star(n);
    for (int v = 1; v < n; ++v)
      star[0] |= 1ULL << v, star[v] |= 1ULL;
    check(star, ++cases);
  }

  // Exhaust every simple undirected graph through six vertices.
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

  mt19937 rng(0xC11C0Eu);
  for (int tc = 0; tc < 1200; ++tc) {
    const int n = 7 + (int)(rng() % 6);
    check(random_graph(n, rng, (int)(rng() % 101)), ++cases);
  }
  cout << "PASS Maximum_Clique cases=" << cases
       << " oracle=exhaustive-subset-clique-check\n";
}
