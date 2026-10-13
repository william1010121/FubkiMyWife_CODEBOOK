#include "../../codebook/1_Basic/Default_code.cpp"
namespace geo {
const double eps = 1e-9, pi = acos(-1.);
int sign(double x) { return abs(x) < eps ? 0 : x > 0 ? 1 : -1; }
#include "../../codebook/8_Geometry/_basic.cpp"
struct C { P c; double r; };
#include "../../codebook/8_Geometry/CircleTangent.cpp"
}
namespace convex {
#include "../../codebook/8_Geometry/PointInConvex.cpp"
}
namespace km {
constexpr int N = 9;
constexpr ll INF = 1LL << 60;
#include "../../codebook/4_Flow_Matching/Kuhn_Munkres.cpp"
KM solver;
}
namespace cut {
#include "../../codebook/4_Flow_Matching/SW-mincut.cpp"
}
namespace clique {
constexpr int N = 132;
#include "../../codebook/2_Graph/NumberofMaximalClique.cpp"
}
namespace tree {
constexpr int N = 64;
int par[N], dfn[N], dep[N];
int LCA(int a, int b) {
  while (dep[a] > dep[b]) a = par[a];
  while (dep[b] > dep[a]) b = par[b];
  while (a != b) a = par[a], b = par[b];
  return a;
}
#include "../../codebook/2_Graph/Virtual_Tree.cpp"
}
namespace nt {
ll mul(ll a, ll b, ll n) { return (__int128)a * b % n; }
#include "../../codebook/6_Math/Miller_Rabin.cpp"
bool prime(ll n) {
  if (n < 2) return false;
  for (ll a : {2,325,9375,28178,450775,9780504,1795265022})
    if (!Miller_Rabin(a, n)) return false;
  return true;
}
#include "../../codebook/6_Math/Pollard_Rho.cpp"
}
namespace sweep {
vector<pii> ps;
vector<int> observed;
set<pii> seen;
pll last;
bool second;
// Check the full projection order between successive direction groups.
void projection(__int128 num, __int128 den) {
  for (int i = 1; i < SZ(observed); ++i) {
    auto a = ps[observed[i - 1]], b = ps[observed[i]];
    assert((__int128(a.Y) - b.Y) * den <=
           (__int128(a.X) - b.X) * num);
  }
}
void swap(int &a, int &b) {
  if (!second) {
    ll x = ll(ps[a].X) - ps[b].X;
    ll y = ll(ps[a].Y) - ps[b].Y;
    if (x < 0 || (!x && y < 0)) x = -x, y = -y;
    if (!seen.empty() && (__int128)last.Y * x <
                         (__int128)y * last.X) {
      if (x) projection((__int128)last.Y * x +
                        (__int128)y * last.X,
                        (__int128)2 * last.X * x);
      else projection(last.Y + last.X, last.X);
    }
    auto i = find(ALL(observed), a), j = find(ALL(observed), b);
    assert(abs(i - j) == 1);
    assert(seen.insert(minmax(a, b)).second);
    iter_swap(i, j); last = {x, y};
  }
  second = !second; std::swap(a, b);
}
#include "../../codebook/8_Geometry/rotatingSweepLine.cpp"
}
mt19937_64 rng(170006);

void tangent_tests() {
  using namespace geo;
  for (double scale : {0.125, 1., 1024., 1048576.})
    for (int turn = 0; turn < 4; ++turn)
      for (double distance : {1., 2., 3., 4., 5., 6., 9.}) {
        P u{3, 4};
        for (int i = 0; i < turn; ++i) u = {-u.y, u.x};
        C a{{17 * scale, -13 * scale}, 20 * scale};
        C b{a.c + u * (distance * scale), 5 * scale};
        int want = distance < 3 ? 0 : distance == 3 ? 1 :
                   distance < 5 ? 2 : distance == 5 ? 3 : 4;
        for (auto [x, y] : {pair{a, b}, pair{b, a}}) {
          auto lines = tangent(x, y);
          if (SZ(lines) != want) {
            cerr << setprecision(17) << "circle scale=" << scale
                 << " turn=" << turn << " distance=" << distance
                 << " measured=" << (x.c-y.c).abs()/scale
                 << " want=" << want << " got=" << SZ(lines) << '\n';
            abort();
          }
          for (auto l : lines) {
            P v = l.pb - l.pa; assert(v.abs() > 0);
            for (C c : {a, b})
              assert(abs(abs(v ^ (c.c - l.pa)) / v.abs() - c.r)
                     < 1e-6 * scale);
          }
        }
        P p = a.c + u * (distance * scale);
        auto lines = tangent(a, p);
        assert(SZ(lines) == (distance < 4 ? 0 : distance == 4 ? 1 : 2));
        for (auto l : lines) {
          P v = l.pb - l.pa;
          assert(v.abs() > 0);
          assert(abs(v ^ (p - l.pa)) < 1e-5 * scale * scale);
          assert(abs(abs(v ^ (a.c - l.pa)) / v.abs() - a.r)
                 < 1e-6 * scale);
        }
      }
  // Strict containment wins over epsilon contact, as in the old API.
  C a{{0, 0}, 4}, b{{3 - eps / 4, 0}, 1};
  assert(tangent(a, b).empty());
  // Exact contact and center conventions, including eps-sized radii.
  for (double r : {eps / 16, 0.125, 1., 1024.}) {
    C c{{0, 0}, r};
    assert(tangent(c, c).empty());
    assert(tangent(c, P{0, 0}).empty());
    assert(tangent(c, P{r, 0}).size() == 1);
  }
}

void convex_tests() {
  using namespace convex;
  // Independent half-plane oracle on a strict parallelogram, including
  // unimodular shears, thin edges, fan diagonals and translated bounds.
  for (ll scale : {1LL, 1000000000LL, 1000000000000000LL})
    for (ll offset : {-999000000000000000LL, 0LL,
                       999000000000000000LL}) {
      vector<IPoint> hull{{offset, offset},
        {offset + 7 * scale, offset + 3 * scale},
        {offset + 12 * scale, offset + 6 * scale},
        {offset + 5 * scale, offset + 3 * scale}};
      // The positive extreme uses the same safe translation for all
      // scales, leaving every polygon/query coordinate within 1e18.
      if (offset && scale == 1000000000000000LL) continue;
      for (int turn = 0; turn < 4; ++turn) {
        for (ll x = -2; x <= 14; ++x)
          for (ll y = -2; y <= 8; ++y) {
            IPoint p{offset + x * scale, offset + y * scale};
            // Invert the two edge vectors in small lattice coordinates.
            ll u = 3 * x - 5 * y, v = -3 * x + 7 * y;
            bool outside = u < 0 || u > 6 || v < 0 || v > 6;
            bool boundary = u == 0 || u == 6 || v == 0 || v == 6;
            assert(pointInConvex(hull, p) ==
                   (outside ? 0 : boundary ? 1 : 2));
          }
        rotate(hull.begin(), hull.begin() + 1, hull.end());
      }
    }
  assert(pointInConvex({}, {0, 0}) == 0);
  for (ll x : {-1000000000000000000LL, 0LL, 1000000000000000000LL}) {
    assert(pointInConvex({{x, x}}, {x, x}) == 1);
    assert(pointInConvex({{x, x}}, {x, x ? 0 : 1}) == 0);
  }
  vector<IPoint> segment{{-1000000000000000000LL, 0},
                         {1000000000000000000LL, 0}};
  assert(pointInConvex(segment, {0, 0}) == 1);
  assert(pointInConvex(segment, {0, 1}) == 0);
}

ll matching_oracle(const vector<vector<ll>> &w) {
  int n = SZ(w); vector<ll> dp(1 << n, LLONG_MIN); dp[0] = 0;
  for (int mask = 0; mask + 1 < (1 << n); ++mask) {
    if (dp[mask] == LLONG_MIN) continue;
    int row = popcount(unsigned(mask));
    for (int j = 0; j < n; ++j)
      if (!(mask >> j & 1) && w[row][j] != -km::INF)
        dp[mask | (1 << j)] = max(dp[mask | (1 << j)],
                                 dp[mask] + w[row][j]);
  }
  return dp.back();
}
void matching_tests() {
  for (int tc = 0; tc < 600; ++tc) {
    int n = tc % 9; km::solver.init(n);
    ll b = (km::INF - 1) / max(1, 2 * n);
    vector<vector<ll>> w(n, vector<ll>(n, -km::INF));
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
      if (i == j || rng() % 3) {
        w[i][j] = (ll(rng() % 3) - 1) * b;
        km::solver.add_edge(i, j, w[i][j]);
      }
    for (int step = 0; step < 12; ++step) {
      // Last-write updates retain a guaranteed real diagonal matching.
      if (n) {
        int i = rng() % n, j = rng() % n;
        w[i][j] = (ll(rng() % 3) - 1) * b;
        km::solver.add_edge(i, j, -w[i][j]);
        km::solver.add_edge(i, j, w[i][j]);
      }
      ll want = matching_oracle(w);
      for (int rep = 0; rep < 2; ++rep) {
        assert(km::solver.solve() == want);
        for (int i = 0; i < n; ++i)
          assert(km::solver.fr[km::solver.fl[i]] == i &&
                 w[i][km::solver.fl[i]] != -km::INF);
      }
    }
  }
}

void graph_tests() {
  clique::BronKerbosch bk;
  for (int tc = 0; tc < 1000; ++tc) {
    int n = tc % 11; vector<vector<int>> w(n, vector<int>(n));
    vector<int> adj(n); bk.init(131); cut::sw.init(n);
    vector<int> ids{1, 63, 64, 65, 127, 128, 129, 130, 131, 2};
    int want_count = 0; ll best = n <= 1 ? 0 : LLONG_MAX, largest = 0;
    for (int i = 0; i < n; ++i) for (int j = 0; j < i; ++j)
      if (rng() % 3) {
        w[i][j] = w[j][i] = 1 + rng() % 7;
        adj[i] |= 1 << j; adj[j] |= 1 << i;
        bk.add_edge(ids[i], ids[j]);
      }
    for (int mask = 0; mask < (1 << n); ++mask) {
      bool maximal = true; ll capacity = 0;
      for (int i = 0; i < n; ++i) {
        if (mask >> i & 1)
          maximal &= (adj[i] & mask) == (mask ^ (1 << i));
        else maximal &= (adj[i] & mask) != mask;
        for (int j = 0; j < i; ++j)
          if (((mask >> i) ^ (mask >> j)) & 1) capacity += w[i][j];
      }
      want_count += maximal;
      if (mask && mask + 1 < (1 << n))
        best = min(best, capacity), largest = max(largest, capacity);
    }
    vector<int> keys(ids.begin(), ids.begin() + n);
    bk.S = INT_MAX - want_count; bk.dfs_vec(keys, {});
    assert(bk.S == INT_MAX);
    // All 131 active vertices are included by solve; others are isolated.
    assert(bk.solve() == (n ? want_count + 131 - n : 131));
    ll scale = largest ? INT_MAX / largest : 1;
    for (int i = 0; i < n; ++i) {
      cut::sw.addEdge(i, i, INT_MAX);
      for (int j = 0; j < i; ++j) {
        int value = w[i][j] * scale;
        cut::sw.addEdge(i, j, value / 2);
        cut::sw.addEdge(j, i, value - value / 2);
      }
    }
    if (n) {
      int s, t; auto saved = cut::sw.edge[0][0];
      ll phase = cut::sw.search(s, t, n), degree = 0;
      for (int i = 0; i < n; ++i) degree += cut::sw.edge[t][i];
      assert(phase == degree && t >= 0 && (n == 1 || s >= 0));
      assert(cut::sw.edge[0][0] == saved);
    }
    assert(cut::sw.solve(n) == best * scale);
  }
  bk.init(0); assert(bk.solve() == 1);
}

void tree_tests() {
  using namespace tree;
  // All capacity-sized chains/stars/binary trees with rotated labels;
  // compare build against independently formed all-pairs LCA closure.
  for (int shape = 0; shape < 3; ++shape)
    for (int shift = 0; shift < N; ++shift) {
      vector<vector<int>> children(N); int root = shift;
      for (int i = 0; i < N; ++i) {
        int u = (i + shift) % N;
        int p = i ? shape == 0 ? i - 1 : shape == 1 ? 0 : (i - 1) / 2 : 0;
        par[u] = (p + shift) % N;
        if (i) children[par[u]].pb(u);
      }
      int timer = 0;
      auto visit = [&](auto &&self, int u) -> void {
        dfn[u] = timer++;
        for (int v : children[u]) dep[v] = dep[u] + 1, self(self, v);
      };
      dep[root] = 0; visit(visit, root);
      for (int pass = 0; pass < 6; ++pass) {
        vector<int> keys;
        for (int u = 0; u < N; ++u) if (pass == 1 || rng() % 3 == 0)
          for (int copies = 1 + rng() % 5; copies; --copies) keys.pb(u);
        if (!pass) keys.clear();
        shuffle(ALL(keys), rng); set<int> nodes(ALL(keys));
        auto saved = nodes;
        for (int u : saved) for (int v : saved) nodes.insert(LCA(u, v));
        set<pii> expected, actual; int expected_root = -1;
        for (int u : nodes) {
          int p = par[u];
          while (p != par[p] && !nodes.count(p)) p = par[p];
          if (p != u && nodes.count(p)) expected.insert({p, u});
          else expected_root = u;
        }
        int got = build(keys); assert(got == expected_root);
        assert(is_sorted(ALL(keys), [](int a, int b) {
          return dfn[a] < dfn[b];
        }));
        assert(adjacent_find(ALL(keys)) == keys.end());
        for (int u = 0; u < N; ++u) for (int v : vG[u])
          assert(actual.insert({u, v}).second);
        assert(actual == expected);
        if (got != -1) reset(got);
        for (auto &row : vG) assert(row.empty());
      }
    }
}

ll pow_oracle(ll a, ll d, ll n) {
  ll x = 1;
  for (int bit = 62; bit >= 0; --bit) {
    x = (__int128)x * x % n;
    if (d >> bit & 1) x = (__int128)x * a % n;
  }
  return x;
}
bool witness_oracle(ll a, ll n) {
  if (n < 2) return false;
  if (!(a %= n)) return true;
  if (!(n & 1)) return n == 2;
  ll d = n - 1; int s = 0;
  while (!(d & 1)) d /= 2, ++s;
  ll x = pow_oracle(a, d, n);
  if (x == 1 || x == n - 1) return true;
  for (int i = 1; i < s; ++i) {
    x = (__int128)x * x % n;
    if (x == n - 1) return true;
  }
  return false;
}
bool trial_prime(ll n) {
  if (n < 2) return false;
  for (ll d = 2; d <= n / d; ++d) if (!(n % d)) return false;
  return true;
}
void number_tests() {
  for (int s = 1; s <= 62; ++s)
    for (ll k : {1LL, 3LL, 5LL, 7LL}) {
      __int128 n = ((__int128)k << s) + 1;
      if (n > LLONG_MAX) continue;
      for (ll a : {0LL, 1LL, 2LL, 3LL, ll(n) - 1, ll(n), LLONG_MAX})
        assert(nt::Miller_Rabin(a, n) == witness_oracle(a, n));
    }
  for (int i = 0; i < 20000; ++i) {
    ll n = rng() & LLONG_MAX, a = rng() & LLONG_MAX;
    assert(nt::Miller_Rabin(a, n) == witness_oracle(a, n));
  }
  for (ll n : {LLONG_MIN, -1LL, 0LL, 1LL, 2LL, 4LL, LLONG_MAX})
    for (ll a : {0LL, 1LL, 2LL, LLONG_MAX})
      assert(nt::Miller_Rabin(a, n) == witness_oracle(a, n));
  vector<ll> primes{2, 3, 5, 7, 17, 31, 97, 257, 65537,
                    1000000007, 1000000009};
  for (ll p : primes) assert(trial_prime(p));
  nt::cnt.clear(); nt::PollardRho(1); assert(nt::cnt.empty());
  for (int tc = 0; tc < 300; ++tc) {
    ll n = 1; map<ll, int> want;
    for (int i = 0; i < 20; ++i) {
      ll p = primes[rng() % primes.size()];
      if (n <= LLONG_MAX / p) n *= p, ++want[p];
    }
    if (tc == 0) n = 1000000016000000063LL,
                 want = {{1000000007, 1}, {1000000009, 1}};
    nt::cnt.clear(); nt::PollardRho(n); assert(nt::cnt == want);
    nt::PollardRho(1); assert(nt::cnt == want);
    nt::PollardRho(n);
    for (auto &[p, e] : want) e *= 2;
    assert(nt::cnt == want);
  }
  // Genuine reuse: the pre-existing factor map survives all calls.
  nt::cnt.clear();
  for (ll p : primes) nt::PollardRho(p), nt::PollardRho(p);
  for (ll p : primes) assert(nt::cnt[p] == 2);
  assert(nt::cnt.size() == primes.size());
}

void sweep_tests() {
  using namespace sweep;
  vector<vector<pii>> inputs;
  // All grid subsets satisfying the stated no-three-collinear contract.
  vector<pii> grid;
  for (int x = -1; x <= 1; ++x) for (int y = -1; y <= 1; ++y)
    grid.pb({x, y});
  for (int mask = 0; mask < 512; ++mask) {
    vector<pii> a;
    for (int i = 0; i < 9; ++i) if (mask >> i & 1) a.pb(grid[i]);
    bool good = true;
    for (int i = 0; i < SZ(a); ++i) for (int j = 0; j < i; ++j)
      for (int k = 0; k < j; ++k)
        good &= ll(a[i].X - a[j].X) * (a[k].Y - a[j].Y) !=
                ll(a[i].Y - a[j].Y) * (a[k].X - a[j].X);
    if (good) inputs.pb(a);
  }
  inputs.pb({{INT_MIN, INT_MIN}, {INT_MAX, INT_MIN},
             {INT_MAX, INT_MAX}, {INT_MIN, INT_MAX}});
  for (int turn = 0; turn < 4; ++turn) {
    vector<pii> a;
    for (int i = -16; i <= 16; ++i) {
      int x = i * 30000000, y = i * i * 3000000;
      if (turn & 1) std::swap(x, y);
      if (turn & 2) x = -x;
      a.pb({x, y});
    }
    inputs.pb(a);
  }
  for (auto a : inputs) for (int rep = 0; rep < 3; ++rep) {
    shuffle(ALL(a), rng); ps = a;
    observed.resize(a.size()); iota(ALL(observed), 0);
    sort(ALL(observed), [&](int i, int j) { return ps[i] < ps[j]; });
    seen.clear(); second = false;
    auto got = rotatingSweepLineOrder(a);
    assert(!second && seen.size() == a.size() * (a.size() - 1) / 2);
    if (!seen.empty() && last.X) projection(last.Y + last.X, last.X);
    assert(got == observed);
    for (int i = 1; i < SZ(got); ++i) assert(a[got[i]] < a[got[i - 1]]);
    seen.clear(); second = false;
    iota(ALL(observed), 0);
    sort(ALL(observed), [&](int i, int j) { return ps[i] < ps[j]; });
    rotatingSweepLine(a); assert(a == ps);
  }
}
int main() {
  tangent_tests(); convex_tests(); matching_tests(); graph_tests();
  tree_tests(); number_tests(); sweep_tests();
  cout << "PASS round17 g06: nine templates, edge/reuse oracles\n";
}
