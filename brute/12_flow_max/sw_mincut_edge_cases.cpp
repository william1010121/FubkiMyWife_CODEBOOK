#include <bits/stdc++.h>
using namespace std;

#include "../../codebook/4_Flow_Matching/SW-mincut.cpp"
#include "flow_oracle.hpp"

static void check_case(const vector<vector<int>> &weight,
                       const vector<tuple<int, int, int>> &edges,
                       int case_id) {
  const int n = static_cast<int>(weight.size());
  sw.init(n);
  for (const auto &[u, v, w] : edges) {
    flow12::require(0 <= u && u < n && 0 <= v && v < n && u != v,
                    "invalid Stoer-Wagner test edge");
    flow12::require(w >= 0, "negative Stoer-Wagner test weight");
    sw.addEdge(u, v, w);  // Repeated calls deliberately test parallel edges.
  }
  const int want = flow12::undirected_mincut(weight);
  const int got = sw.solve(n);
  if (got != want) {
    cerr << "Stoer-Wagner mismatch case=" << case_id << " got=" << got
         << " want=" << want << " weight=" << flow12::matrix_string(weight)
         << '\n';
    exit(1);
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
        edges.emplace_back(u, v, w);  // Includes zero-weight edges.
      }
      check_case(weight, edges, case_id++);
    }
  }
  cout << "Stoer-Wagner exhaustive binary undirected graphs: " << case_id << '\n';
}

static void parallel_and_random_graphs() {
  int case_id = 100000;
  {
    vector<vector<int>> weight(5, vector<int>(5));
    vector<tuple<int, int, int>> edges = {
        {0, 1, 0}, {0, 1, 2}, {0, 1, 3}, {1, 2, 0}, {1, 2, 4},
        {2, 3, 1}, {3, 4, 0}, {3, 4, 2}, {0, 4, 1}, {1, 4, 2}};
    for (const auto &[u, v, w] : edges) weight[u][v] += w, weight[v][u] += w;
    check_case(weight, edges, case_id++);
  }

  mt19937 rng(0x57A612);
  for (int tc = 0; tc < 1800; ++tc) {
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
  cout << "Stoer-Wagner parallel/disconnected random graphs: 1801\n";
}

int main() {
  exhaustive_binary_graphs();
  parallel_and_random_graphs();

  // A one-vertex global cut has conventional value zero, but the template's
  // documented loop is for n >= 2.  Probe and report that precondition rather
  // than pretending INT_MAX is a useful global-cut value.
  vector<vector<int>> singleton(1, vector<int>(1));
  sw.init(1);
  const int singleton_value = sw.solve(1);
  if (singleton_value == 0)
    cout << "CONTRACT: Stoer-Wagner n=1 returns conventional zero\n";
  else
    cout << "CONTRACT: Stoer-Wagner n=1 returns " << singleton_value
         << "; callers must require n >= 2 for a finite global cut\n";
  cout << "PASS Stoer-Wagner edge-case oracle suite\n";
}
