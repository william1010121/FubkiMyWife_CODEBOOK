#include "../../codebook/1_Basic/Default_code.cpp"

namespace maximal {
constexpr int N = 130;
#include "../../codebook/2_Graph/NumberofMaximalClique.cpp"
}
namespace maximum {
constexpr int N = 129;
#include "../../codebook/2_Graph/Maximum_Clique_Dyn.cpp"
}
namespace biconnected {
#include "../../codebook/2_Graph/BCC_Vertex.cpp"
}
namespace unary {
constexpr int len = 3, sigma = 1;
#include "../../codebook/5_String/Aho-Corasick_Automatan.cpp"
}
namespace wide_alphabet {
constexpr int len = 65, sigma = 63;
#include "../../codebook/5_String/Aho-Corasick_Automatan.cpp"
}
namespace witness {
ll mul(ll a, ll b, ll n) {
  assert(n > 0 && 0 <= a && a < n && 0 <= b && b < n);
  return (__int128)a * b % n;
}
#include "../../codebook/6_Math/Miller_Rabin.cpp"
}
namespace cuttree {
constexpr int MAXN = 16, INF = 7;
#include "../../codebook/4_Flow_Matching/Dinic.cpp"
#include "../../codebook/4_Flow_Matching/Gomory_Hu_tree.cpp"
}
namespace extreme_cut {
constexpr int MAXN = 2, INF = INT_MAX;
#include "../../codebook/4_Flow_Matching/Dinic.cpp"
#include "../../codebook/4_Flow_Matching/Gomory_Hu_tree.cpp"
}
namespace envelope {
constexpr ll INF = 1;
#include "../../codebook/3_Data_Structure/LiChaoST.cpp"
}
#include "../../codebook/9_Else/simulated_annealing.cpp"

void clique_tests() {
  maximal::BronKerbosch bk;
  maximum::MaxClique mc;
  mt19937 rng(11001);
  // Three-word bitsets; reuse after complete/empty/full-capacity graphs.
  for (int n : {129, 0, 1, 63, 64, 65, 127, 128, 129})
    for (int parts : {1, 2, 3, 129}) {
      vector<int> label(n); iota(ALL(label), 0);
      shuffle(ALL(label), rng);
      bk.init(n); mc.init(n);
      vector<int> size(parts);
      for (int v : label) ++size[v % parts];
      for (int u = 0; u < n; ++u)
        for (int v = u + 1; v < n; ++v)
          if (label[u] % parts != label[v] % parts) {
            bk.add_edge(u + 1, v + 1); mc.add_edge(u, v);
          }
      int count = 1, best = 0;
      for (int k : size) if (k) count *= k, ++best;
      for (int repeat = 0; repeat < 3; ++repeat) {
        assert(bk.solve() == count && mc.solve() == best);
        set<int> found;
        for (int i = 0; i < best; ++i) {
          int u = mc.sol[i];
          assert(0 <= u && u < n && found.insert(u).second);
          for (int j = 0; j < i; ++j)
            assert(mc.G[u][mc.sol[j]]);
        }
      }
    }
  bk.init(129); bk.S = INT_MAX - 1; bk.dfs({}, {});
  assert(bk.S == INT_MAX);
  bitset<maximal::N> excluded; excluded[129] = 1;
  bk.dfs({}, excluded); assert(bk.S == INT_MAX);
}

void bcc_tests() {
  // Many disconnected DFS roots leave stack remnants; each component
  // has two triangles, a duplicated bridge, and two singleton blocks.
  const int copies = 100, n = 8 * copies;
  vector<int> label(n); iota(ALL(label), 0);
  mt19937 rng(11002);
  for (int rep = 0; rep < 12; ++rep) {
    shuffle(ALL(label), rng);
    vector<pii> edges;
    set<vector<int>> expected;
    vector<int> ap(n);
    for (int c = 0; c < copies; ++c) {
      auto v = [&](int k) { return label[8 * c + k]; };
      for (auto [a, b] : vector<pii>{{0, 1}, {1, 2}, {2, 0},
             {2, 3}, {3, 4}, {4, 2}, {4, 5}, {4, 5}})
        edges.pb({v(a), v(b)});
      for (int k = 0; k < 8; ++k) edges.pb({v(k), v(k)});
      for (auto block : vector<vector<int>>{
             {v(0), v(1), v(2)}, {v(2), v(3), v(4)},
             {v(4), v(5)}, {v(6)}, {v(7)}}) {
        sort(ALL(block)); expected.insert(block);
      }
      ap[v(2)] = ap[v(4)] = 1;
    }
    shuffle(ALL(edges), rng);
    biconnected::BCC b(n);
    for (auto [u, v] : edges) b.add_edge(u, v);
    b.solve();
    set<vector<int>> got;
    for (auto block : b.bcc) sort(ALL(block)), got.insert(block);
    assert(got == expected && b.is_ap == ap);
    assert(b.nbcc == 5 * copies && b.dft == n);
    b.block_cut_tree();
    assert(b.nbcc == 7 * copies && SZ(b.nG) == b.nbcc);
    int arcs = 0;
    for (auto &row : b.nG) arcs += SZ(row);
    assert(arcs == 8 * copies);
  }
}

void ac_tests() {
  for (int rep = 0; rep < 4; ++rep) {
    auto &a = unary::ac; a.init();
    string empty, one = "A";
    assert(a.input(empty) == 1);
    int v = a.input(one); assert(a.input(one) == v);
    a.make_fl(); assert(a.top == unary::len);
    a.cnt[1] = 1;
    int state = 1;
    for (int i = 0; i < 10000; ++i)
      state = a.rnx[state][0], ++a.cnt[state];
    a.solve(); assert(a.cnt[v] == 10000 && a.cnt[1] == 10001);

    auto &b = wide_alphabet::ac; b.init();
    vector<int> endpoints;
    for (int k = 0; k < wide_alphabet::sigma; ++k) {
      string s(1, char('A' + k)); endpoints.pb(b.input(s));
    }
    b.make_fl(); assert(b.top == wide_alphabet::len);
    for (int u = 1; u < b.top; ++u)
      for (int k = 0; k < wide_alphabet::sigma; ++k)
        assert(b.rnx[u][k] == endpoints[k]);
    b.cnt[1] = 1; b.cnt[endpoints.back()] = INT_MAX - 1;
    b.solve(); assert(b.cnt[1] == INT_MAX);
    assert(b.cnt[endpoints.back()] == INT_MAX - 1);
  }
}

// Independent modular products use only unsigned addition/doubling.
// Since n <= LLONG_MAX, a+b and 2*a cannot overflow unsigned ll.
unsigned long long slow_mul(unsigned long long a,
                             unsigned long long b,
                             unsigned long long n) {
  unsigned long long x = 0;
  for (; b; b >>= 1, a = (a + a) % n)
    if (b & 1) x = (x + a) % n;
  return x;
}
ll slow_pow(ll a, ll e, ll n) {
  ll x = 1;
  for (int bit = 62; bit >= 0; --bit) {
    x = slow_mul(x, x, n);
    if (e >> bit & 1) x = slow_mul(x, a, n);
  }
  return x;
}
bool strong_oracle(ll a, ll n) {
  if (n < 2) return false;
  a %= n;
  if (!a) return true;
  if (!(n & 1)) return n == 2;
  ll d = n - 1;
  while (!(d & 1)) d /= 2;
  if (slow_pow(a, d, n) == 1) return true;
  // Recompute each required power rather than reusing test squarings.
  for (; d < n - 1; d *= 2)
    if (slow_pow(a, d, n) == n - 1) return true;
  return false;
}
bool prime(ll n) {
  for (ll a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022})
    if (!witness::Miller_Rabin(a, n)) return false;
  return true;
}
void mr_tests() {
  vector<ll> ns{LLONG_MIN, -1, 0, 1, 2, 4, LLONG_MAX,
                LLONG_MAX - 1, (1LL << 61) - 1};
  for (int k = 1; k <= 62; ++k) ns.pb((1LL << k) + 1);
  mt19937_64 rng(11003);
  for (int k = 0; k < 40; ++k) ns.pb(rng() & LLONG_MAX);
  for (ll n : ns) {
    vector<ll> as{0, 1, 2, 325, LLONG_MAX};
    if (n >= 2) as.pb(n - 1), as.pb(n);
    for (ll a : as)
      assert(witness::Miller_Rabin(a, n) == strong_oracle(a, n));
  }
  assert(prime((1LL << 61) - 1));
  for (ll p : {3LL, 97LL, 2147483647LL, 3037000499LL})
    for (ll q : {3LL, 97LL, 2147483647LL, 3037000499LL}) {
      __int128 n = (__int128)p * q;
      if (n <= LLONG_MAX) assert(!prime((ll)n));
    }
  assert(witness::Miller_Rabin(2, 2047) && !prime(2047));
}

void gh_tests() {
  using namespace cuttree;
  // Force many augmentations: the kernel INF is smaller than capacities.
  Dinic.init(0); GomoryHu(0);
  mt19937 rng(11004);
  for (int n : {1, 8, 2, 7, 0, 8}) {
    vector<vector<int>> w(n, vector<int>(n));
    Dinic.init(n);
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v) {
      int cap = rng() % 100;
      w[u][v] = w[v][u] = cap;
      Dinic.add_edge(u, v, cap / 2);
      Dinic.add_edge(v, u, cap / 2);
      Dinic.add_edge(u, v, cap - cap / 2);
      Dinic.add_edge(v, u, cap - cap / 2);
    }
    if (n > 1) Dinic.maxflow(n - 1, 0); // Reset nonzero residuals.
    for (int repeat = 0; repeat < 2; ++repeat) {
      GomoryHu(n);
      for (int s = 0; s < n; ++s) for (int t = s + 1; t < n; ++t) {
        int want = INT_MAX;
        for (int mask = 0; mask < (1 << n); ++mask)
          if ((mask >> s & 1) && !(mask >> t & 1)) {
            int cut = 0;
            for (int u = 0; u < n; ++u) if (mask >> u & 1)
              for (int v = 0; v < n; ++v)
                if (!(mask >> v & 1)) cut += w[u][v];
            want = min(want, cut);
          }
        vector<vector<pii>> tree(n);
        for (int u = 1; u < n; ++u) {
          tree[u].pb({g[u], gh_w[u]});
          tree[g[u]].pb({u, gh_w[u]});
        }
        vector<int> value(n, -1); value[s] = INT_MAX;
        queue<int> q; q.push(s);
        while (!q.empty()) {
          int u = q.front(); q.pop();
          for (auto [v, cap] : tree[u]) if (value[v] < 0)
            value[v] = min(value[u], cap), q.push(v);
        }
        assert(value[t] == want);
      }
    }
  }
  extreme_cut::Dinic.init(2);
  extreme_cut::Dinic.add_edge(0, 1, INT_MAX);
  extreme_cut::Dinic.add_edge(1, 0, INT_MAX);
  for (int rep = 0; rep < 3; ++rep) {
    extreme_cut::GomoryHu(2);
    assert(extreme_cut::g[1] == 0 && extreme_cut::gh_w[1] == INT_MAX);
  }
}

void lichao_tests() {
  using namespace envelope;
  // A minimal positive sentinel; actual values can equal either sentinel.
  for (int n = 1; n <= 5; ++n) {
    vector<L> lines;
    for (ll m = -2; m <= 2; ++m) for (ll k = -1; k <= 1; ++k)
      if (-INF <= m * (n - 1) + k && m * (n - 1) + k <= INF)
        lines.pb(L(m, k, LLONG_MIN));
    mt19937 rng(11005);
    for (int rep = 0; rep < 24; ++rep) {
      shuffle(ALL(lines), rng);
      LiChao tree(n); vector<ll> want(n, -INF);
      for (auto line : lines) {
        tree.insert(line);
        for (int x = 0; x < n; ++x) {
          want[x] = max(want[x], line.at(x));
          assert(tree.query(x) == want[x]);
        }
      }
    }
  }
}

void annealing_tests() {
  // Replay proposals and acceptance with a long-double probability oracle.
  for (double initial : {-3., 0., 1e-14, 1., 1e6})
    for (double cooling : {-1., 0., 0.5, 1., 1.1}) {
      double temp = initial;
      mt19937_64 rng(11006), oracle_rng = rng;
      vector<int> visits;
      auto next = [&](int x, auto &r) {
        visits.pb(x); return x + int(r() % 7) - 3;
      };
      auto score = [](int x) { return double(x * x); };
      int got = simulated_annealing(17, score, next, rng,
                                   200, temp, cooling);
      int current = 17, best = 17;
      uniform_real_distribution<double> unit(0, 1);
      for (int i = 0; i < 200; ++i) {
        assert(visits[i] == current);
        int candidate = current + int(oracle_rng() % 7) - 3;
        long double delta = score(current) - score(candidate);
        if (delta > 0 || expl(delta / max(temp, 1e-15))
            >= unit(oracle_rng)) current = candidate;
        if (score(current) < score(best)) best = current;
        temp *= cooling;
      }
      assert(got == best && rng == oracle_rng);
    }
  mt19937_64 rng(11), saved = rng;
  assert(simulated_annealing(0, [](int x) { return double(x); },
    [](int, auto &) -> int { abort(); }, rng, -1) == 0);
  assert(rng == saved);
}

int main() {
  clique_tests(); bcc_tests(); ac_tests(); mr_tests();
  gh_tests(); lichao_tests(); annealing_tests();
  cout << "PASS round11 g11: capacity, exact arithmetic, reuse, RNG\n";
}
