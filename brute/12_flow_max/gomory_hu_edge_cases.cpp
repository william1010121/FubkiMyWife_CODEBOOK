#include <bits/stdc++.h>
using namespace std;

#define MAXN 20
#define pb push_back
const int INF = 1'000'000'000;
#include "../../codebook/4_Flow_Matching/Dinic.cpp"
#include "../../codebook/4_Flow_Matching/Gomory_Hu_tree.cpp"
#include "flow_oracle.hpp"

static void load_graph(const vector<vector<int>> &weight,
                       const vector<tuple<int, int, int>> &edges) {
  const int n = static_cast<int>(weight.size());
  Dinic.init(n);
  for (const auto &[u, v, w] : edges) {
    flow12::require(0 <= u && u < n && 0 <= v && v < n && u != v,
                    "invalid Gomory-Hu test edge");
    flow12::require(w >= 0, "negative Gomory-Hu test weight");
    // The tree template consumes a directed max-flow object, so an undirected
    // edge is represented by two directed capacity arcs.
    Dinic.add_edge(u, v, w);
    Dinic.add_edge(v, u, w);
  }
}

static int tree_pair_cut(int n, int s, int t) {
  if (s == t) return 0;
  vector<vector<pair<int, int>>> tree(n);
  for (int i = 1; i < n; ++i) {
    flow12::require(0 <= g[i] && g[i] < n && g[i] != i,
                    "Gomory-Hu produced an invalid parent");
    tree[i].push_back({g[i], gh_w[i]});
    tree[g[i]].push_back({i, gh_w[i]});
  }
  vector<int> seen(n);
  queue<pair<int, int>> q;
  q.push({s, INF});
  seen[s] = 1;
  while (!q.empty()) {
    const auto [u, value] = q.front();
    q.pop();
    if (u == t) return value;
    for (const auto &[v, w] : tree[u]) if (!seen[v]) {
      seen[v] = 1;
      q.push({v, min(value, w)});
    }
  }
  return -1;
}

static void check_case(const vector<vector<int>> &weight,
                       const vector<tuple<int, int, int>> &edges,
                       int case_id) {
  const int n = static_cast<int>(weight.size());
  load_graph(weight, edges);
  GomoryHu(n);
  for (int s = 0; s < n; ++s) for (int t = s; t < n; ++t) {
    const int got = tree_pair_cut(n, s, t);
    const int want = flow12::undirected_pair_mincut(weight, s, t);
    if (got != want) {
      cerr << "Gomory-Hu mismatch case=" << case_id << " pair=" << s << ',' << t
           << " got=" << got << " want=" << want
           << " weight=" << flow12::matrix_string(weight) << '\n';
      exit(1);
    }
  }
}

static vector<pair<int, int>> undirected_pairs(int n) {
  vector<pair<int, int>> pairs;
  for (int u = 0; u < n; ++u)
    for (int v = u + 1; v < n; ++v) pairs.push_back({u, v});
  return pairs;
}

static void exhaustive_binary_graphs() {
  int case_id = 0;
  for (int n = 2; n <= 5; ++n) {
    const auto pairs = undirected_pairs(n);
    for (int mask = 0; mask < (1 << pairs.size()); ++mask) {
      vector<vector<int>> weight(n, vector<int>(n));
      vector<tuple<int, int, int>> edges;
      for (int i = 0; i < static_cast<int>(pairs.size()); ++i) {
        const auto [u, v] = pairs[i];
        const int w = (mask >> i) & 1;
        weight[u][v] = weight[v][u] = w;
        edges.emplace_back(u, v, w);  // Include zero-capacity undirected arcs.
      }
      check_case(weight, edges, case_id++);
    }
  }
  cout << "Gomory-Hu exhaustive binary undirected graphs: " << case_id << '\n';
}

static void parallel_and_random_graphs() {
  int case_id = 100000;
  {
    vector<vector<int>> weight(5, vector<int>(5));
    vector<tuple<int, int, int>> edges = {
        {0, 1, 0}, {0, 1, 2}, {0, 1, 3}, {1, 2, 1}, {1, 2, 0},
        {2, 3, 4}, {3, 4, 0}, {3, 4, 2}, {0, 4, 1}, {1, 4, 2}};
    for (const auto &[u, v, w] : edges) weight[u][v] += w, weight[v][u] += w;
    check_case(weight, edges, case_id++);
  }

  mt19937 rng(0x6F6D12);
  for (int tc = 0; tc < 1000; ++tc) {
    const int n = 2 + static_cast<int>(rng() % 7);
    vector<vector<int>> weight(n, vector<int>(n));
    vector<tuple<int, int, int>> edges;
    for (auto [u, v] : undirected_pairs(n)) {
      const int copies = 1 + static_cast<int>(rng() % 4);
      for (int k = 0; k < copies; ++k) {
        const int w = static_cast<int>(rng() % 8);
        edges.emplace_back(u, v, w);
        weight[u][v] += w;
        weight[v][u] += w;
      }
    }
    check_case(weight, edges, case_id++);
  }
  cout << "Gomory-Hu parallel/disconnected random graphs: 1001\n";
}

int main() {
  exhaustive_binary_graphs();
  parallel_and_random_graphs();

  // GomoryHu builds a tree for n >= 1; a same-vertex pair has empty path and
  // conventional cut value zero, checked explicitly in tree_pair_cut().
  vector<vector<int>> singleton(1, vector<int>(1));
  load_graph(singleton, {});
  GomoryHu(1);
  flow12::require(tree_pair_cut(1, 0, 0) == 0,
                  "Gomory-Hu singleton/same-vertex query is not zero");
  cout << "CONTRACT: Gomory-Hu same-vertex query is handled by the caller as zero\n";
  cout << "PASS Gomory-Hu edge-case oracle suite\n";
}
