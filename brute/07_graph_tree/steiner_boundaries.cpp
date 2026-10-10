#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int N = 8, T = 4, INF = INT_MAX;
#define ALL(v) (v).begin(), (v).end()
#define SZ(v) int((v).size())
#include "../../codebook/2_Graph/MinimumSteinerTree.cpp"

struct Edge { int u, v, w; };
// Edge-subset oracle uses wide arithmetic and caps only the final answer.
static int oracle(int n, vector<Edge> es, vector<int> term,
                  vector<int> vc) {
  if (term.empty()) return 0;
  ll ans = INF;
  for (int mask = 0; mask < (1 << SZ(es)); ++mask) {
    vector<vector<int>> g(n);
    vector<bool> used(n);
    ll cost = 0;
    for (int u : term) used[u] = true;
    for (int i = 0; i < SZ(es); ++i) if (mask >> i & 1) {
      auto [u, v, w] = es[i];
      cost += w, used[u] = used[v] = true;
      g[u].push_back(v);
    }
    for (int u = 0; u < n; ++u) if (used[u]) cost += vc[u];
    if (cost >= ans) continue;
    for (int sink = 0; sink < n; ++sink) {
      bool ok = true;
      for (int src : term) {
        vector<bool> seen(n);
        vector<int> q{src}; seen[src] = true;
        for (int i = 0; i < SZ(q); ++i)
          for (int v : g[q[i]]) if (!seen[v])
            seen[v] = true, q.push_back(v);
        ok &= seen[sink];
      }
      if (ok) ans = cost;
    }
  }
  return int(ans);
}
static SteinerTree st;
static int cases;
static void check(int n, vector<Edge> es, vector<int> term,
                  vector<int> vc) {
  st.init(n);
  for (int u = 0; u < n; ++u) st.vcst[u] = vc[u];
  for (auto [u, v, w] : es) st.add_edge(u, v, w);
  int got = st.solve(term), want = oracle(n, es, term, vc);
  ++cases;
  if (got != want) {
    cerr << "Steiner boundary case=" << cases << " got=" << got
         << " want=" << want << '\n';
    exit(1);
  }
}
int main() {
  check(0, {}, {}, {});
  check(1, {{0, 0, 0}}, {0, 0}, {INF - 1});
  check(2, {}, {0, 1}, {0, 0}); // INF + INF must not wrap.
  check(2, {{0, 1, INF - 10}}, {0, 1}, {1, 1});
  check(2, {{0, 1, INF - 10}}, {0, 1}, {20, 20});
  check(4, {{0, 2, 0}, {1, 2, 0}, {2, 3, 0}},
        {0, 1, 3, 0}, {200000000, 200000000, 300000000, 1});
  mt19937 rng(0xB005);
  const vector<int> costs{0, 1, 7, INF / 4, INF / 2, INF - 1};
  for (int tc = 0; tc < 2200; ++tc) {
    int n = 1 + rng() % 6;
    vector<int> vc(n), term;
    vector<Edge> es;
    for (int &x : vc) x = costs[rng() % SZ(costs)];
    for (int i = 0; i < 8; ++i)
      es.push_back({int(rng() % n), int(rng() % n),
                    costs[rng() % SZ(costs)]});
    for (int i = 0, k = rng() % (T + 1); i < k; ++i)
      term.push_back(rng() % n);
    check(n, es, term, vc);
  }
  cout << "PASS Steiner boundaries cases=" << cases << '\n';
}
