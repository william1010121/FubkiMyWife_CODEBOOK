#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using Wide = __int128_t;
#define SZ(a) ((int)(a).size())
#define pb push_back

namespace bipartite {
const int N = 64;
#include "codebook/4_Flow_Matching/Bipartite_Matching.cpp"
}
namespace assignment {
const int N = 10;
const ll INF = 1LL << 60;
#include "codebook/4_Flow_Matching/Kuhn_Munkres.cpp"
}
namespace costflow {
const int N = 64;
const ll INF = 1LL << 60;
#include "codebook/4_Flow_Matching/MincostMaxflow.cpp"
}
namespace globalcut {
#include "codebook/4_Flow_Matching/SW-mincut.cpp"
}
namespace bounded {
const int MAXN = 64, INF = INT_MAX;
#include "codebook/4_Flow_Matching/Dinic.cpp"
#include "codebook/4_Flow_Matching/BoundedFlow.cpp"
}
namespace cuttree {
const int MAXN = 64, INF = INT_MAX;
#include "codebook/4_Flow_Matching/Dinic.cpp"
#include "codebook/4_Flow_Matching/Gomory_Hu_tree.cpp"
}
namespace circulation {
const int N = 64;
const ll INF = 4'000'000'000'000'000'000LL;
#include "codebook/4_Flow_Matching/MinCostCirculation.cpp"
}

static void require(bool ok, const char *why) {
  if (!ok) {
    cerr << "FAIL round5: " << why << '\n';
    exit(1);
  }
}

static void test_bipartite() {
  bipartite::Bipartite_Matching bm;
  for (auto [l, r] : vector<pair<int, int>>{
           {0, 0}, {64, 0}, {0, 64}, {64, 64}, {64, 1}, {1, 64}}) {
    bm.init(l, r);
    for (int u = 0; u < l; ++u) for (int v = 0; v < r; ++v)
      if (u == v || r == 1 || l == 1)
        bm.add_edge(u, v), bm.add_edge(u, v);
    for (int repeat = 0; repeat < 2; ++repeat) {
      require(bm.matching() == min(l, r), "bipartite boundary size");
      vector<int> seen(r);
      int count = 0;
      for (int u = 0; u < l; ++u) if (bm.mp[u] != -1) {
        int v = bm.mp[u];
        require(0 <= v && v < r && !seen[v]++, "bipartite injective");
        require(bm.mq[v] == u, "bipartite inverse");
        require(find(bm.G[u].begin(), bm.G[u].end(), v) !=
                    bm.G[u].end(), "bipartite edge membership");
        ++count;
      }
      require(count == min(l, r), "bipartite reported cardinality");
    }
  }
}

static void test_assignment() {
  mt19937_64 rng(50504);
  for (int tc = 0; tc < 120; ++tc) {
    int n = tc % 11;
    assignment::KM km;
    km.init(n);
    ll magnitude = assignment::INF / (2 * max(1, n)) - 1;
    vector<vector<ll>> w(n, vector<ll>(n, -assignment::INF));
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
      if (u == v || rng() % 3) {
        ll value = (ll(rng() % 3) - 1) * magnitude;
        km.add_edge(u, v, 0); // Repeated assignment: last weight wins.
        km.add_edge(u, v, value), w[u][v] = value;
      }
    const Wide unreachable = -(Wide(1) << 100);
    vector<Wide> dp(1 << n, unreachable);
    dp[0] = 0;
    for (int mask = 0; mask < (1 << n); ++mask) {
      int u = popcount((unsigned)mask);
      if (u == n || dp[mask] == unreachable) continue;
      for (int v = 0; v < n; ++v)
        if (!(mask >> v & 1) && w[u][v] != -assignment::INF)
          dp[mask | (1 << v)] = max(dp[mask | (1 << v)],
                                   dp[mask] + w[u][v]);
    }
    for (int repeat = 0; repeat < 2; ++repeat) {
      require(km.solve() == dp.back(), "KM wide assignment oracle");
      vector<int> seen(n);
      Wide sum = 0;
      for (int u = 0; u < n; ++u) {
        int v = km.fl[u];
        require(0 <= v && v < n && !seen[v]++, "KM injective");
        require(km.fr[v] == u, "KM inverse");
        require(w[u][v] != -assignment::INF, "KM chose missing edge");
        sum += w[u][v];
      }
      require(sum == dp.back(), "KM matching weight");
    }
  }
}

static void test_costflow() {
  costflow::MinCostMaxFlow mf;
  mf.init(0); // Empty construction; solve needs valid terminals.
  for (bool neg : {false, true}) {
    mf.init(1);
    mf.add_edge(0, 0, LLONG_MAX, 0);
    ll flow = -1, cost = -1;
    mf.solve(0, 0, flow, cost, neg);
    require(flow == 0 && cost == 0, "costflow singleton query");
    mf.init(2);
    ll capacity = costflow::INF + 7;
    mf.add_edge(0, 1, capacity, 1);
    mf.add_edge(0, 0, LLONG_MAX, 0);
    mf.add_edge(1, 1, LLONG_MAX, 1);
    mf.solve(0, 1, flow, cost, neg);
    require(flow == capacity && cost == capacity,
            "costflow capacity greater than INF");
    mf.solve(1, 0, flow, cost, false);
    require(flow == capacity && cost == -capacity,
            "costflow large reverse flow");
    for (int u = 0; u < 2; ++u) for (int i = 0; i < SZ(mf.G[u]); ++i) {
      const auto &e = mf.G[u][i], &rev = mf.G[e.to][e.rev];
      require(rev.to == u && rev.rev == i && rev.flow == -e.flow,
              "costflow reverse edge including self-loop");
    }
    mf.init(4);
    ll big = costflow::INF - 1;
    mf.add_edge(0, 1, 1, -big), mf.add_edge(1, 3, 1, big);
    mf.add_edge(0, 2, 1, big), mf.add_edge(2, 3, 1, -big);
    mf.add_edge(1, 2, 1, 2 * big); // A zero-cost directed cycle.
    mf.add_edge(2, 1, 1, -2 * big);
    mf.solve(0, 3, flow, cost, neg);
    require(flow == 2 && cost == 0, "costflow extreme potentials");
    mf.solve(0, 3, flow, cost, false);
    require(flow == 0 && cost == 0, "costflow exhausted graph");
    mf.solve(3, 0, flow, cost, false);
    require(flow == 2 && cost == 0, "costflow changed source");
    mf.init(4);
    mf.add_edge(2, 3, 1, -big); // Negative edge unreachable from s.
    mf.solve(0, 1, flow, cost, neg);
    require(flow == 0 && cost == 0, "costflow unreachable sink");
  }
}

static ll cut_oracle(const vector<vector<int>> &w, int s = -1,
                     int t = -1) {
  int n = SZ(w);
  if (n <= 1 || (s >= 0 && s == t)) return 0;
  ll best = LLONG_MAX;
  for (int mask = 1; mask + 1 < (1 << n); ++mask) {
    if (s >= 0 && (!(mask >> s & 1) || (mask >> t & 1))) continue;
    ll cut = 0;
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      if ((mask >> u & 1) != (mask >> v & 1)) cut += w[u][v];
    best = min(best, cut);
  }
  return best;
}

static vector<vector<int>> weighted_graph(int tc, mt19937 &rng) {
  int n = 2 + tc % 7;
  vector<vector<int>> w(n, vector<int>(n));
  int remaining = INT_MAX;
  for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v) {
    int value = rng() % (remaining / 2 + 1);
    remaining -= value;
    w[u][v] = w[v][u] = value;
  }
  return w; // Total edge weight <= INT_MAX, so every cut fits int.
}

static void test_globalcut() {
  auto check = [](const vector<vector<int>> &w, bool loops) {
    int n = SZ(w);
    globalcut::sw.init(n);
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      globalcut::sw.addEdge(u, v, w[u][v]);
    if (loops) for (int u = 0; u < n; ++u) {
      globalcut::sw.addEdge(u, u, INT_MAX);
      globalcut::sw.addEdge(u, u, INT_MAX);
    }
    require(globalcut::sw.solve(n) == cut_oracle(w),
            "SW wide cut oracle");
  };
  // This four-vertex star overflows the original contraction's diagonal,
  // although total input weight and every possible cut fit int.
  int unit = INT_MAX / 4;
  vector<vector<int>> star(4, vector<int>(4));
  for (int u = 0; u < 3; ++u)
    star[u][3] = star[3][u] = unit * (u == 2 ? 2 : 1);
  check(star, false);
  check({}, false), check({{0}}, true);
  check({{0, INT_MAX}, {INT_MAX, 0}}, false);
  vector<vector<int>> complete(5, vector<int>(5, INT_MAX / 6));
  for (int u = 0; u < 5; ++u) complete[u][u] = 0;
  check(complete, false); // Total > INT_MAX; each cut still fits int.
  mt19937 rng(50505);
  for (int tc = 0; tc < 500; ++tc) check(weighted_graph(tc, rng), true);
  globalcut::sw.init(globalcut::SW::MXN);
  require(globalcut::sw.solve(globalcut::SW::MXN) == 0,
          "SW array boundary empty graph");
}

struct BoundArc { int u, v, lo, hi; bool plain = false; };

static void check_bounded(int n, const vector<BoundArc> &arcs,
                          bool feasible, int maximum) {
  for (int mode = 0; mode < (n ? 3 : 1); ++mode) {
    bounded::BoundedFlow bf;
    bf.init(n);
    vector<pair<int, int>> refs;
    vector<int> sizes(n);
    for (auto e : arcs) {
      refs.push_back({e.u, SZ(bf.G[e.u])});
      if (e.plain) bf.add_edge(e.u, e.v, e.hi);
      else bf.add_edge(e.u, e.v, e.lo, e.hi);
    }
    for (int u = 0; u < n; ++u) {
      sizes[u] = SZ(bf.G[u]);
      for (int i = 0; i < sizes[u]; ++i) {
        const auto &e = bf.G[u][i];
        const auto &rev = bf.G[e.to][e.rev];
        require(rev.to == u && rev.rev == i && &rev != &e,
                "bounded reverse index including self-loop");
      }
    }
    int value = mode == 0 ? (bf.solve() ? 0 : -1) :
                mode == 1 ? bf.solve(0, n - 1) : bf.solve(0, 0);
    int want = mode == 1 && n > 1 ? maximum : feasible ? 0 : -1;
    require(value == want, "bounded feasibility/value oracle");
    if (value < 0) continue;
    vector<Wide> balance(n);
    for (int i = 0; i < SZ(arcs); ++i) {
      auto [u, id] = refs[i];
      const auto &e = bf.G[u][id], &rev = bf.G[e.to][e.rev];
      const auto &a = arcs[i];
      require(e.cap == a.hi && a.lo <= e.flow && e.flow <= a.hi,
              "bounded original edge bounds");
      require(e.flow + rev.flow == a.lo, "bounded reverse flow");
      balance[u] += e.flow, balance[e.to] -= e.flow;
    }
    for (int u = 0; u < n; ++u) {
      Wide expected = mode == 1 && n > 1 ?
                      (u == 0 ? value : u == n - 1 ? -value : 0) : 0;
      require(balance[u] == expected, "bounded conservation");
      require(SZ(bf.G[u]) == sizes[u], "bounded auxiliary cleanup");
    }
    require(bf.G[n + 1].empty() && bf.G[n + 2].empty(),
            "bounded super-vertex cleanup");
  }
}

static void test_bounded() {
  check_bounded(0, {}, true, 0);
  check_bounded(1, {{0, 0, 1, INT_MAX}, {0, 0, 0, INT_MAX, true}},
                true, 0);
  check_bounded(2, {{0, 1, INT_MAX, INT_MAX}}, false, INT_MAX);
  check_bounded(2, {{1, 0, 1, 1}}, false, -1);
  check_bounded(61, {}, true, 0); // Exactly MAXN = n+3.
  mt19937 rng(50506);
  for (int tc = 0; tc < 400; ++tc) {
    int n = 1 + rng() % 5, m = rng() % 8;
    vector<BoundArc> arcs;
    for (int i = 0; i < m; ++i) {
      int lo = rng() % 3;
      arcs.push_back({int(rng() % n), int(rng() % n),
                      lo, lo + int(rng() % 3), lo == 0 && rng() % 2});
    }
    vector<int> bal(n);
    bool feasible = false;
    int maximum = -1;
    function<void(int)> visit = [&](int at) {
      if (at == m) {
        feasible |= all_of(bal.begin(), bal.end(), [](int x) {
          return x == 0;
        });
        if (n > 1 && bal[0] >= 0 && bal.back() == -bal[0] &&
            all_of(bal.begin() + 1, bal.end() - 1,
                   [](int x) { return x == 0; }))
          maximum = max(maximum, bal[0]);
        return;
      }
      const auto &e = arcs[at];
      for (int flow = e.lo; flow <= e.hi; ++flow) {
        bal[e.u] += flow, bal[e.v] -= flow;
        visit(at + 1);
        bal[e.u] -= flow, bal[e.v] += flow;
      }
    };
    visit(0);
    check_bounded(n, arcs, feasible, maximum);
  }
}

static ll tree_cut(int n, int s, int t) {
  if (s == t) return 0;
  vector<vector<pair<int, int>>> tree(n);
  for (int u = 1; u < n; ++u) {
    require(0 <= cuttree::g[u] && cuttree::g[u] < n &&
                cuttree::g[u] != u, "GH valid parent");
    tree[u].push_back({cuttree::g[u], cuttree::gh_w[u]});
    tree[cuttree::g[u]].push_back({u, cuttree::gh_w[u]});
  }
  vector<int> seen(n);
  queue<pair<int, int>> q;
  q.push({s, INT_MAX}), seen[s] = 1;
  while (!q.empty()) {
    auto [u, cut] = q.front(); q.pop();
    if (u == t) return cut;
    for (auto [v, w] : tree[u]) if (!seen[v])
      q.push({v, min(cut, w)}), seen[v] = 1;
  }
  require(false, "GH disconnected tree");
  return -1;
}

static void test_cuttree() {
  mt19937 rng(50507);
  for (int tc = 0; tc < 160; ++tc) {
    auto w = tc < 2 ? vector<vector<int>>(tc, vector<int>(tc)) :
             tc == 2 ? vector<vector<int>>{{0, INT_MAX}, {INT_MAX, 0}} :
             weighted_graph(tc, rng);
    int n = SZ(w);
    cuttree::Dinic.init(n);
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v) {
      cuttree::Dinic.add_edge(u, v, w[u][v]);
      cuttree::Dinic.add_edge(v, u, w[u][v]);
    }
    for (int repeat = 0; repeat < 2; ++repeat) {
      // GomoryHu must rebuild correctly from an arbitrary existing flow.
      cuttree::Dinic.reset();
      if (n > 1) cuttree::Dinic.maxflow(0, n - 1);
      cuttree::GomoryHu(n);
      for (int u = 0; u < n; ++u) for (int v = u; v < n; ++v)
        require(tree_cut(n, u, v) == cut_oracle(w, u, v),
                "GH wide pair-cut oracle");
    }
  }
  cuttree::Dinic.init(cuttree::MAXN);
  for (int u = 1; u < cuttree::MAXN; ++u) {
    cuttree::Dinic.add_edge(0, u, u);
    cuttree::Dinic.add_edge(u, 0, u);
  }
  cuttree::GomoryHu(cuttree::MAXN);
  for (int u = 1; u < cuttree::MAXN; ++u)
    for (int v = u + 1; v < cuttree::MAXN; ++v)
      require(tree_cut(cuttree::MAXN, u, v) == u, "GH array boundary");
}

struct CostArc { int u, v; ll cap, cost; };

static Wide run_circulation(int n, const vector<CostArc> &arcs) {
  circulation::mcmf.init(n);
  ll maximum = 0;
  vector<pair<int, int>> refs;
  for (auto e : arcs) {
    refs.push_back({e.u, SZ(circulation::mcmf.G[e.u])});
    circulation::mcmf.add_edge(e.u, e.v, e.cap, e.cost);
    maximum = max(maximum, e.cap);
  }
  int bit = maximum ? 63 - countl_zero((unsigned long long)maximum) : 0;
  circulation::mcmf.solve(bit);
  Wide cost = 0;
  vector<Wide> balance(n);
  for (int i = 0; i < SZ(arcs); ++i) {
    auto [u, id] = refs[i];
    const auto &e = circulation::mcmf.G[u][id];
    const auto &rev = circulation::mcmf.G[e.to][e.rev];
    require(e.cap == arcs[i].cap && 0 <= e.flow && e.flow <= e.cap,
            "circulation scaled capacity/flow");
    require(rev.to == u && rev.rev == id && rev.flow == -e.flow,
            "circulation reverse edges including self-loops");
    balance[u] += e.flow, balance[e.to] -= e.flow;
    cost += Wide(e.flow) * e.cost;
  }
  require(all_of(balance.begin(), balance.end(), [](Wide x) {
    return x == 0;
  }), "circulation conservation");
  return cost;
}

static void test_circulation() {
  require(run_circulation(0, {}) == 0, "circulation empty graph");
  require(run_circulation(64, {}) == 0, "circulation array boundary");
  require(run_circulation(1, {{0, 0, LLONG_MAX, -1}}) == -LLONG_MAX,
          "circulation bit 62 negative self-loop");
  require(run_circulation(2, {{0, 1, LLONG_MAX, -1},
                             {1, 0, LLONG_MAX, 0}}) == -LLONG_MAX,
          "circulation bit 62 negative cycle");
  require(run_circulation(1, {{0, 0, 1, -LLONG_MAX}}) == -LLONG_MAX,
          "circulation extreme signed cost");
  require(run_circulation(1, {{0, 0, LLONG_MAX, 1},
                             {0, 0, 0, -LLONG_MAX}}) == 0,
          "circulation positive/zero-capacity loops");
  mt19937 rng(50508);
  for (int tc = 0; tc < 400; ++tc) {
    int n = 1 + rng() % 5, m = rng() % 7;
    vector<CostArc> arcs;
    for (int i = 0; i < m; ++i)
      arcs.push_back({int(rng() % n), int(rng() % n),
                      ll(rng() % 5), ll(rng() % 11) - 5});
    Wide best = 0;
    vector<int> bal(n);
    function<void(int, Wide)> visit = [&](int at, Wide cost) {
      if (at == m) {
        if (all_of(bal.begin(), bal.end(), [](int x) { return x == 0; }))
          best = min(best, cost);
        return;
      }
      const auto &e = arcs[at];
      for (int flow = 0; flow <= e.cap; ++flow) {
        bal[e.u] += flow, bal[e.v] -= flow;
        visit(at + 1, cost + Wide(flow) * e.cost);
        bal[e.u] -= flow, bal[e.v] += flow;
      }
    };
    visit(0, 0);
    require(run_circulation(n, arcs) == best,
            "circulation wide exhaustive oracle");
  }
}

int main(int argc, char **argv) {
  vector<pair<string, void (*)()>> suites = {
      {"bipartite", test_bipartite}, {"KM", test_assignment},
      {"MCMF", test_costflow}, {"SW", test_globalcut},
      {"bounded", test_bounded}, {"GH", test_cuttree},
      {"circulation", test_circulation}};
  for (auto [name, test] : suites) if (argc == 1 || argv[1] == name) {
    test();
    cout << "PASS round5 " << name << '\n';
  }
}
