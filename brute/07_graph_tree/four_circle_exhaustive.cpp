#include <bits/stdc++.h>
using namespace std;

int n, m, deg[20], cnt[20];
vector<int> E[20], E1[20];
long long total;
#define main four_circle_template_main
#include "../../codebook/2_Graph/FourCircleCount.cpp"
#undef main

static long long oracle(int vertices, const vector<pair<int, int>> &edges) {
  vector<vector<char>> adj(vertices, vector<char>(vertices));
  for (auto [u, v] : edges) {
    --u, --v;
    if (0 <= u && u < vertices && 0 <= v && v < vertices && u != v)
      adj[u][v] = adj[v][u] = 1;
  }
  long long ordered = 0;
  for (int a = 0; a < vertices; ++a) for (int b = 0; b < vertices; ++b)
    for (int c = 0; c < vertices; ++c) for (int d = 0; d < vertices; ++d)
      if (a != b && a != c && a != d && b != c && b != d && c != d &&
          adj[a][b] && adj[b][c] && adj[c][d] && adj[d][a]) ++ordered;
  return ordered / 8;
}

static int cases = 0;
static void check(int vertices, vector<pair<int, int>> edges, const string &label) {
  long long got = count_four_cycles(vertices, edges);
  long long want = oracle(vertices, edges);
  ++cases;
  if (got != want) {
    cerr << "FourCircleCount mismatch label=" << label << " case=" << cases
         << " n=" << vertices << " m=" << edges.size() << " got=" << got
         << " want=" << want << '\n';
    for (auto [u, v] : edges) cerr << u << ' ' << v << '\n';
    exit(1);
  }
}

static vector<pair<int, int>> complete_graph(int vertices) {
  vector<pair<int, int>> edges;
  for (int u = 1; u <= vertices; ++u)
    for (int v = u + 1; v <= vertices; ++v) edges.push_back({u, v});
  return edges;
}

int main() {
  check(1, {}, "one-vertex");
  check(2, {{1, 2}}, "single-edge");
  check(4, {{1, 2}, {2, 3}, {3, 4}}, "path");
  check(4, {{1, 2}, {1, 3}, {1, 4}}, "star");
  check(4, {{1, 2}, {2, 3}, {3, 4}, {4, 1}}, "one-cycle");
  check(4, {{1, 3}, {1, 4}, {2, 3}, {2, 4}}, "K2-2");
  check(6, {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6},
            {3, 4}, {3, 5}, {3, 6}}, "K3-3");
  check(4, complete_graph(4), "K4");
  check(6, complete_graph(6), "K6");
  check(8, {{1, 2}, {2, 3}, {3, 4}, {4, 1}, {5, 6}, {6, 7}, {7, 8}, {8, 5}},
        "two-disconnected-cycles");
  check(7, {{1, 2}, {2, 3}, {3, 4}, {4, 1}, {5, 6}}, "cycle-plus-disconnected");

  int exhaustive = 0;
  for (int vertices = 1; vertices <= 6; ++vertices) {
    vector<pair<int, int>> universe;
    for (int u = 1; u <= vertices; ++u)
      for (int v = u + 1; v <= vertices; ++v) universe.push_back({u, v});
    int limit = 1 << (int)universe.size();
    for (int mask = 0; mask < limit; ++mask) {
      vector<pair<int, int>> edges;
      for (int i = 0; i < (int)universe.size(); ++i)
        if (mask >> i & 1) edges.push_back(universe[i]);
      if (mask & 1) reverse(edges.begin(), edges.end());
      check(vertices, edges, "exhaustive-simple-graph");
      ++exhaustive;
    }
  }

  mt19937 rng(0x40C1E2026);
  for (int tc = 0; tc < 2600; ++tc) {
    int vertices = 7 + rng() % 6;
    vector<pair<int, int>> edges;
    for (int u = 1; u <= vertices; ++u) for (int v = u + 1; v <= vertices; ++v)
      if (rng() % 100 < 35) edges.push_back({u, v});
    shuffle(edges.begin(), edges.end(), rng);
    check(vertices, edges, "seeded-random-simple-graph");
  }
  cout << "FourCircleCount PASS cases=" << cases << " exhaustive=" << exhaustive
       << " ordered-4-tuple oracle\n";
}
