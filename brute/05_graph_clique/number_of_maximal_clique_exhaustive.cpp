#include <bits/stdc++.h>
using namespace std;

#define N 13
#define ALL(v) (v).begin(), (v).end()
#define SZ(v) ((int)(v).size())
#define pb push_back
#include "../../codebook/2_Graph/NumberofMaximalClique.cpp"

static int oracle(const vector<unsigned long long> &adj) {
  const int n = (int)adj.size();
  int answer = 0;
  for (unsigned long long mask = 0; mask < (1ULL << n); ++mask) {
    bool clique = true;
    for (int v = 0; v < n && clique; ++v) if (mask >> v & 1ULL) {
      const auto without_v = mask ^ (1ULL << v);
      clique = (adj[v] & without_v) == without_v;
    }
    if (!clique) continue;
    bool maximal = true;
    for (int v = 0; v < n && maximal; ++v)
      if (!(mask >> v & 1ULL) &&
          (adj[v] & mask) == mask)
        maximal = false;
    answer += maximal;
  }
  return answer;
}

static void add_graph(BronKerbosch &bk,
                      const vector<unsigned long long> &adj) {
  const int n = (int)adj.size();
  for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
    if (adj[u] >> v & 1ULL) {
      bk.add_edge(u + 1, v + 1);
      bk.add_edge(u + 1, v + 1);
      if ((u ^ v) & 1) bk.add_edge(v + 1, u + 1);
    }
}

static void check(const vector<unsigned long long> &adj, int tc) {
  const int want = oracle(adj);
  BronKerbosch bk;
  bk.init((int)adj.size());
  add_graph(bk, adj);
  const int got = bk.solve();
  if (got != want) {
    cerr << "NumberofMaximalClique mismatch case=" << tc
         << " n=" << adj.size() << " got=" << got
         << " want=" << want << '\n';
    exit(1);
  }
}

int main() {
  int cases = 0;
  // The subset oracle counts the empty clique as maximal only when n == 0;
  // this matches the implementation's mathematically natural convention.
  check({}, ++cases);
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

  mt19937 rng(0xB0C10u);
  for (int tc = 0; tc < 1500; ++tc) {
    const int n = 7 + (int)(rng() % 6);
    vector<unsigned long long> adj(n);
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      if ((int)(rng() % 100) < 48)
        adj[u] |= 1ULL << v, adj[v] |= 1ULL << u;
    check(adj, ++cases);
  }
  cout << "PASS NumberofMaximalClique cases=" << cases
       << " oracle=exhaustive-maximal-subset-check\n";
}
