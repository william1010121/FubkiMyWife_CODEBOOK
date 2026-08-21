#include <bits/stdc++.h>
using namespace std;

#define ALL(x) (x).begin(), (x).end()
#define pb push_back
#include "../../codebook/4_Flow_Matching/Maximum_Simple_Graph_Matching.cpp"

static int oracle(const vector<vector<char>> &adj, int mask,
                  vector<int> &memo) {
  int n = (int)adj.size();
  if (mask == (1 << n) - 1) return 0;
  int &ans = memo[mask];
  if (ans != -1) return ans;
  int u = 0;
  while (mask >> u & 1) ++u;
  ans = oracle(adj, mask | (1 << u), memo);
  for (int v = u + 1; v < n; ++v)
    if (adj[u][v] && !(mask >> v & 1))
      ans = max(ans, 1 + oracle(adj, mask | (1 << u) | (1 << v), memo));
  return ans;
}

static void check(const vector<vector<char>> &adj, int tc, const char *label,
                  mt19937 &rng, bool duplicate) {
  int n = (int)adj.size();
  Matching matching(n);
  vector<pair<int, int>> edges;
  for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
    if (adj[u][v]) edges.push_back({u, v});
  shuffle(edges.begin(), edges.end(), rng);
  for (auto [u, v] : edges) {
    matching.add_edge(u, v);
    if (duplicate && (u + 3 * v + tc) % 4 == 0) matching.add_edge(u, v);
  }
  vector<int> memo(1 << n, -1);
  int want = oracle(adj, 0, memo);
  int got = matching.solve();
  if (got != want) {
    cerr << "General matching mismatch case " << tc << " (" << label
         << ") n=" << n << " got=" << got << " want=" << want << '\n';
    for (auto [u, v] : edges) cerr << '(' << u << ',' << v << ") ";
    cerr << '\n';
    exit(1);
  }
}

static vector<vector<char>> empty_graph(int n) {
  return vector<vector<char>>(n, vector<char>(n));
}

int main() {
  mt19937 rng(0x14B10550);
  int tc = 0;

  // Exhaustive tiny graphs, including the empty graph and every triangle.
  for (int n = 0; n <= 5; ++n) {
    int edges = n * (n - 1) / 2;
    for (int mask = 0; mask < (1 << edges); ++mask) {
      vector<vector<char>> adj = empty_graph(n);
      int bit = 0;
      for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v, ++bit)
        adj[u][v] = adj[v][u] = mask >> bit & 1;
      check(adj, tc++, "exhaustive-tiny", rng, true);
    }
  }

  auto add = [](vector<vector<char>> &a, int u, int v) {
    a[u][v] = a[v][u] = 1;
  };
  // Odd cycles, stars, cliques, disconnected components, and shared-center
  // triangles exercise the blossom contraction paths directly.
  for (int n : {3, 5, 7, 9, 11, 13, 15}) {
    auto a = empty_graph(n);
    for (int i = 0; i < n; ++i) add(a, i, (i + 1) % n);
    check(a, tc++, "odd-cycle", rng, true);
  }
  for (int n : {1, 2, 3, 7, 12, 16}) {
    auto a = empty_graph(n);
    for (int i = 1; i < n; ++i) add(a, 0, i);
    check(a, tc++, "star", rng, false);
    a = empty_graph(n);
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) add(a, i, j);
    check(a, tc++, "clique", rng, true);
  }
  for (int petals = 1; petals <= 7; ++petals) {
    int n = 1 + 2 * petals;
    auto a = empty_graph(n);
    for (int p = 0; p < petals; ++p) {
      int x = 1 + 2 * p, y = x + 1;
      add(a, 0, x); add(a, x, y); add(a, y, 0);
    }
    check(a, tc++, "shared-center-blossoms", rng, true);
  }

  mt19937 local(0x14EDE6A);
  for (int z = 0; z < 1200; ++z) {
    int n = 1 + local() % 14;
    int mode = z % 6;
    vector<vector<char>> a = empty_graph(n);
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v) {
      bool take = mode == 0 ? false : mode == 1 ? true : mode == 2 ? local() % 5 == 0 :
                  mode == 3 ? local() % 2 == 0 : mode == 4 ? local() % 5 != 0 :
                  ((u * 17 + v * 31 + z) % 11 < 5);
      if (take) add(a, u, v);
    }
    check(a, tc++, "seeded-random", local, z % 2 == 0);
  }
  cout << "PASS " << tc << " general-matching edge/oracle cases\n";
}
