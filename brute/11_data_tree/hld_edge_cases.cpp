#include <bits/stdc++.h>
using namespace std;
#define N 256
#define pb push_back
#include "../../codebook/3_Data_Structure/Dynamic_2D_Segment_Tree.cpp"
#include "../../codebook/3_Data_Structure/Heavy_light_Decomposition.cpp"

namespace {

struct Edge {
  int u, v;
};

[[noreturn]] void mismatch(const string &name, int u, int v, long long got,
                           long long want) {
  cerr << "hld mismatch case=" << name << " u=" << u << " v=" << v
       << " got=" << got << " want=" << want << '\n';
  exit(1);
}

void check(const string &name, Heavy_light_Decomposition &h,
           const vector<Edge> &edges, const vector<int> &values) {
  int n = static_cast<int>(values.size()) - 1;
  vector<vector<int>> g(n + 1);
  for (auto e : edges)
    g[e.u].push_back(e.v), g[e.v].push_back(e.u);

  // For each source, a tree DFS gives the exact sum on every source-target
  // path without relying on any LCA/HLD logic.
  vector<vector<long long>> want(n + 1, vector<long long>(n + 1));
  for (int s = 1; s <= n; ++s) {
    vector<int> parent(n + 1, -1), st = {s};
    parent[s] = 0;
    want[s][s] = values[s];
    while (!st.empty()) {
      int u = st.back();
      st.pop_back();
      for (int v : g[u])
        if (v != parent[u])
          parent[v] = u, want[s][v] = want[s][u] + values[v], st.push_back(v);
    }
  }

  for (int u = 1; u <= n; ++u)
    for (int v = 1; v <= n; ++v) {
      int got = h.query(u, v);
      if (got != want[u][v]) mismatch(name, u, v, got, want[u][v]);
    }
}

void run_case(const string &name, int n, const vector<Edge> &edges,
              const vector<int> &values) {
  Heavy_light_Decomposition h;
  auto rebuild = [&](const vector<int> &vals) {
    h.init(n);
    for (int u = 1; u <= n; ++u) h.val[u] = vals[u];
    for (auto e : edges) h.add_edge(e.u, e.v);
    h.build();
  };

  rebuild(values);
  check(name, h, edges, values);
  for (int u = 1; u <= n; ++u)
    if (h.query(u, u) != values[u]) mismatch(name, u, u, h.query(u, u), values[u]);

  // Reusing the same object catches stale graph/son/segment-tree state.
  vector<int> changed = values;
  for (int u = 1; u <= n; ++u) changed[u] = -values[u] + (u % 5) - 2;
  rebuild(changed);
  check(name + ":reinit", h, edges, changed);
}

vector<Edge> chain(int n) {
  vector<Edge> e;
  for (int v = 2; v <= n; ++v) e.push_back({v - 1, v});
  return e;
}

vector<Edge> star(int n) {
  vector<Edge> e;
  for (int v = 2; v <= n; ++v) e.push_back({1, v});
  return e;
}

vector<int> values(int n, int mode) {
  vector<int> v(n + 1);
  for (int u = 1; u <= n; ++u) {
    if (mode == 0) v[u] = 0;
    else if (mode == 1) v[u] = (u & 1) ? 1000 : -1000;
    else if (mode == 2) v[u] = (u % 4 == 0) ? 17 : -3;
    else v[u] = (u * 7919) % 2000001 - 1000000;
  }
  return v;
}

}  // namespace

int main() {
  run_case("singleton", 1, {}, values(1, 0));
  run_case("two-vertex", 2, {{1, 2}}, values(2, 1));
  run_case("chain-zero", 127, chain(127), values(127, 0));
  run_case("chain-alternating", 127, chain(127), values(127, 1));
  run_case("star-duplicate", 127, star(127), values(127, 2));

  mt19937 rng(0x11A1D);
  for (int tc = 0; tc < 180; ++tc) {
    int n = 2 + rng() % 78;
    vector<Edge> e;
    for (int u = 2; u <= n; ++u)
      e.push_back({1 + static_cast<int>(rng() % (u - 1)), u});
    vector<int> v(n + 1);
    for (int u = 1; u <= n; ++u) v[u] = (int)(rng() % 2001) - 1000;
    run_case("random-" + to_string(tc), n, e, v);
  }
  cout << "hld edge cases: PASS\n";
}
