#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll, ll>;
#define X first
#define Y second
#define pb push_back
#define N 128
#include "../../codebook/3_Data_Structure/Centroid_Decomposition.cpp"

namespace {

struct Edge {
  int u, v, w;
};

vector<vector<ll>> distances(int n, const vector<Edge> &edges) {
  vector<vector<pair<int, int>>> g(n + 1);
  for (auto e : edges)
    g[e.u].push_back({e.v, e.w}), g[e.v].push_back({e.u, e.w});
  vector<vector<ll>> d(n + 1, vector<ll>(n + 1));
  for (int s = 1; s <= n; ++s) {
    vector<int> parent(n + 1, -1), st = {s};
    parent[s] = 0;
    while (!st.empty()) {
      int u = st.back();
      st.pop_back();
      for (auto [v, w] : g[u])
        if (v != parent[u])
          parent[v] = u, d[s][v] = d[s][u] + w, st.push_back(v);
    }
  }
  return d;
}

[[noreturn]] void mismatch(const string &name, int mark_count, int u,
                           ll got, ll want) {
  cerr << "centroid mismatch case=" << name << " marks=" << mark_count
       << " query=" << u << " got=" << got << " want=" << want << '\n';
  exit(1);
}

void check(const string &name, Cent_Dec &cd, const vector<vector<ll>> &d,
           const vector<int> &marked) {
  int n = static_cast<int>(d.size()) - 1;
  for (int u = 1; u <= n; ++u) {
    ll want = 0;
    for (int v : marked) want += d[u][v];
    ll got = cd.query(u);
    if (got != want) mismatch(name, marked.size(), u, got, want);
  }
}

void run_case(const string &name, int n, const vector<Edge> &edges) {
  auto d = distances(n, edges);
  Cent_Dec cd;

  auto rebuild = [&] {
    cd.init(n);
    for (auto e : edges) cd.add_edge(e.u, e.v, e.w);
    cd.build();
    vector<int> component(n + 1);
    for (int u = 1; u <= n; ++u)
      for (int a = u; a; a = cd.pa[a]) ++component[a];
    for (int u = 1; u <= n; ++u) {
      assert(cd.done[u] == 0);
      assert(cd.layer[u] <= __lg(n));
      if (cd.pa[u]) assert(2 * component[u] <= component[cd.pa[u]]);
      for (int a = u; a; a = cd.pa[a]) {
        assert(cd.dis[cd.layer[a]][u] == d[a][u]);
        if (cd.pa[a]) assert(cd.layer[cd.pa[a]] + 1 == cd.layer[a]);
      }
    }
  };

  rebuild();
  vector<int> marked;
  check(name, cd, d, marked);

  // Mark every vertex once, then stress multiplicity and unusual update order.
  for (int u = 1; u <= n; ++u) {
    cd.modify(u), marked.push_back(u);
    check(name, cd, d, marked);
  }
  for (int q = 0; q < 4 * n + 9; ++q) {
    int u = 1 + (q * q + 7 * q + 3) % n;
    cd.modify(u), marked.push_back(u);
    check(name, cd, d, marked);
  }

  // init() must clear all accumulated information when an object is reused.
  rebuild();
  marked.clear();
  check(name + ":reinit", cd, d, marked);
  for (int q = 0; q < 3 * n + 1; ++q) {
    int u = n - (q * 5 + 1) % n;
    cd.modify(u), marked.push_back(u);
    check(name + ":reinit", cd, d, marked);
  }
}

vector<Edge> chain(int n) {
  vector<Edge> e;
  for (int v = 2; v <= n; ++v) e.push_back({v - 1, v, (7 * v) % 23});
  return e;
}

vector<Edge> star(int n) {
  vector<Edge> e;
  for (int v = 2; v <= n; ++v) e.push_back({1, v, (11 * v) % 29});
  return e;
}

}  // namespace

int main() {
  {
    Cent_Dec cd;
    for (int n : {N - 1, 1, 7, N - 1, 2}) {
      cd.init(n);
      auto e = n == 7 ? star(n) : chain(n);
      for (auto edge : e) cd.add_edge(edge.u, edge.v, edge.w);
      cd.build();
      auto d = distances(n, e);
      check("resize-empty", cd, d, {});
      for (int u = n; u >= 1; --u) cd.modify(u);
      vector<int> marked(n);
      iota(marked.begin(), marked.end(), 1);
      check("resize-marked", cd, d, marked);
    }
  }
  run_case("singleton", 1, {});
  run_case("two-vertex-zero-edge", 2, {{1, 2, 0}});
  run_case("chain", 63, chain(63));
  run_case("star", 63, star(63));
  run_case("capacity-chain", N - 1, chain(N - 1));
  run_case("capacity-star", N - 1, star(N - 1));
  run_case("signed-path", 5,
           {{1, 2, -7}, {2, 3, 0}, {3, 4, 7}, {4, 5, -11}});
  run_case("wide-signed-weights", 5,
           {{1, 2, INT_MAX}, {2, 3, INT_MAX},
            {3, 4, INT_MIN}, {4, 5, INT_MIN}});

  vector<Edge> balanced;
  for (int v = 2; v <= 63; ++v)
    balanced.push_back({v / 2, v, (v * 13) % 31});
  run_case("balanced", 63, balanced);

  // All labeled trees through n=6 via independent Pruefer decoding.
  for (int n = 2; n <= 6; ++n) {
    int count = 1;
    for (int i = 0; i < n - 2; ++i) count *= n;
    for (int code = 0; code < count; ++code) {
      int digits = code;
      vector<int> sequence(n - 2), degree(n + 1, 1);
      for (int &u : sequence) u = 1 + digits % n, digits /= n;
      for (int u : sequence) ++degree[u];
      vector<Edge> edges;
      for (int u : sequence) {
        int leaf = 1;
        while (degree[leaf] != 1) ++leaf;
        edges.push_back({leaf, u, (leaf + u + code) % 7 - 3});
        --degree[leaf], --degree[u];
      }
      vector<int> ends;
      for (int u = 1; u <= n; ++u) if (degree[u] == 1) ends.push_back(u);
      edges.push_back({ends[0], ends[1], code % 5 - 2});
      if (code & 1) reverse(edges.begin(), edges.end());
      run_case("exhaustive-labeled-" + to_string(n) + "-" +
               to_string(code), n, edges);
    }
  }

  mt19937 rng(0x11C0DE);
  for (int tc = 0; tc < 140; ++tc) {
    int n = 2 + rng() % 48;
    vector<Edge> e;
    for (int v = 2; v <= n; ++v) {
      int p = 1 + rng() % (v - 1);
      int w = rng() % 41;  // zero-weight edges are intentional.
      e.push_back({p, v, w});
    }
    run_case("random-" + to_string(tc), n, e);
  }
  cout << "centroid edge cases: PASS\n";
}
