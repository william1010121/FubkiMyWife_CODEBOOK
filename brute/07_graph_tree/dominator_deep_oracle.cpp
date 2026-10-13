#include <bits/stdc++.h>
using namespace std;
#define pb push_back
constexpr int N = 514;
#include "../../codebook/2_Graph/Dominator_Tree.cpp"

static dominator_tree dt;

// Vertex deletion defines dominance without DFS numbers or semidominators.
static void check(const vector<vector<int>> &g, int root) {
  int n = int(g.size()) - 1;
  dt.tarjan(root);
  auto reach = [&](int blocked) {
    vector<bool> seen(n + 1);
    vector<int> q;
    if (root != blocked) seen[root] = true, q.push_back(root);
    for (int i = 0; i < int(q.size()); ++i)
      for (int v : g[q[i]]) if (v != blocked && !seen[v])
        seen[v] = true, q.push_back(v);
    return seen;
  };
  auto seen = reach(0);
  vector<int> parent(n + 1);
  for (int u = 1; u <= n; ++u) for (int v : dt.tree[u]) {
    if (v < 1 || v > n || parent[v] || !seen[u] || !seen[v])
      throw runtime_error("deep dominator tree edge");
    parent[v] = u;
  }
  vector<vector<bool>> ancestor(n + 1, vector<bool>(n + 1));
  for (int v = 1; v <= n; ++v) {
    if (bool(dt.dfn[v]) != seen[v])
      throw runtime_error("deep dominator reachable set");
    if (!seen[v]) {
      if (parent[v]) throw runtime_error("unreachable dominator child");
      continue;
    }
    if (v == root ? parent[v] != 0 : parent[v] == 0)
      throw runtime_error("deep dominator missing parent");
    if (v != root && dt.id[dt.idom[dt.dfn[v]]] != parent[v])
      throw runtime_error("deep dominator DFS-indexed idom");
    for (int u = v, steps = 0; u; u = parent[u]) {
      if (++steps > n) throw runtime_error("deep dominator cycle");
      ancestor[u][v] = true;
    }
  }
  for (int x = 1; x <= n; ++x) {
    auto without = reach(x);
    for (int v = 1; v <= n; ++v)
      if (ancestor[x][v] != bool(seen[v] && !without[v]))
        throw runtime_error("deep vertex-removal dominance oracle");
  }
}

int main() {
  mt19937 rng(0xD008);
  int cases = 0;
  for (int n : {513, 1, 65, 512, 2, 129, 513}) {
    for (int mode = 0; mode < 4; ++mode) {
      dt.init(n);
      vector<vector<int>> g(n + 1);
      vector<int> label(n + 1);
      iota(label.begin(), label.end(), 0);
      shuffle(label.begin() + 1, label.end(), rng);
      auto edge = [&](int u, int v) {
        dt.add_edge(label[u], label[v]);
        dt.add_edge(label[u], label[v]); // repeated predecessor entries
        g[label[u]].push_back(label[v]);
      };
      for (int i = 1; i <= n; ++i) {
        edge(i, i);
        if (i < n && (mode != 3 || i % 17)) edge(i, i + 1);
        if (mode >= 1 && i > 2) edge(i, i / 2);
        if (mode == 2) edge(1, i); // bypass the entire deep DFS chain
        if (mode == 3 && i + 3 <= n) edge(i + 3, i);
      }
      for (int root : {label[1], label[n], label[(n + 1) / 2]}) {
        check(g, root), check(g, root);
        cases += 2;
      }
    }
  }
  cout << "PASS deep dominator deletion oracle cases=" << cases << '\n';
}
