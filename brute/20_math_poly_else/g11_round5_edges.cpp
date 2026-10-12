#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define SZ(a) ((int)a.size())
#define ALL(v) v.begin(), v.end()
#define pb push_back
#include "../../codebook/9_Else/cyc_tsearch.cpp"
#include "../../codebook/9_Else/CDQ_Dominance.cpp"
namespace mo_update {
#include "../../codebook/9_Else/Mos_Algorithm_With_modification.cpp"
}
namespace mo_tree {
#include "../../codebook/9_Else/Mos_Algorithm_On_Tree.cpp"
}
#include "../../codebook/9_Else/HilbertCurve.cpp"
#include "../../codebook/9_Else/DynamicConvexTrick.cpp"
#include "../../codebook/9_Else/All_LCS.cpp"
#include "../../codebook/9_Else/AdaptiveSimpson.cpp"
#include "../../codebook/9_Else/simulated_annealing.cpp"
#include "../../codebook/9_Else/tree_hash.cpp"
#include "../../codebook/9_Else/BinarySearchOnFraction.cpp"
#include "../../codebook/9_Else/min_plus_convolution.cpp"
#include "../../codebook/9_Else/BitsetLCS.cpp"
namespace queens {
#include "../../codebook/9_Else/NQueens.cpp"
}

void require(bool ok, const char *msg) {
  if (!ok) throw runtime_error(msg);
}
mt19937 rng(5011);
void cyclic() {
  require(cyc_tsearch(1, [](int, int) {
    throw runtime_error("singleton cyclic search called pred");
    return false;
  }) == 0, "singleton cyclic search");
  for (int tc = 0; tc < 400; ++tc) {
    int n = 2 + rng() % 100, shift = rng() % n;
    vector<int> a = {INT_MIN}, left, right;
    for (int i = 1; i < n - 1; ++i)
      (rng() & 1 ? left : right).pb(i);
    a.insert(a.end(), ALL(left)); a.pb(INT_MAX);
    a.insert(a.end(), right.rbegin(), right.rend());
    rotate(a.begin(), a.begin() + shift, a.end());
    auto pred = [&](int i, int j) { return a.at(i) < a.at(j); };
    require(a[cyc_tsearch(n, pred)] == INT_MIN, "extreme cyclic U");
  }
}
void cdq() {
  for (int tc = 0; tc < 1200; ++tc) {
    int n = rng() % 22, k = rng() % 9;
    vector<vector<int>> a(n, vector<int>(k));
    const int values[] = {INT_MIN, -1, 0, 1, INT_MAX};
    for (auto &row : a) for (int &x : row) x = values[rng() % 5];
    auto got = dominance(a);
    for (int i = 0; i < n; ++i) {
      ll want = 0;
      for (int j = 0; j < n; ++j) if (i != j) {
        bool ok = true;
        for (int d = 0; d < k; ++d) ok &= a[j][d] <= a[i][d];
        want += ok;
      }
      require(got[i] == want, "CDQ extremes/duplicates/zero dims");
    }
  }
}
void mo_updates() {
  auto bad_time = [](int, int, int) { require(false, "empty Mo time"); };
  auto bad_pos = [](int) { require(false, "empty Mo position"); };
  auto bad_ans = [](auto &) { require(false, "empty Mo answer"); };
  mo_update::solve({}, bad_time, bad_time, bad_pos, bad_pos, bad_ans);
  for (int tc = 0; tc < 200; ++tc) {
    int n = 1 + rng() % 25, block = 1 + rng() % (n + 3);
    vector<ll> initial(n), cur;
    for (ll &x : initial) x = ll(rng() % 2000000001) - 1000000000;
    struct Update { int pos; ll before, after; };
    vector<Update> updates(1);
    vector<vector<ll>> versions = {initial};
    for (int t = 1; t <= 40; ++t) {
      auto next = versions.back();
      int pos = rng() % n;
      ll value = t % 3 ? ll(rng() % 7) - 3 : next[pos];
      updates.pb({pos, next[pos], value}); next[pos] = value;
      versions.pb(next);
    }
    vector<mo_update::Query> qs;
    for (int id = 0; id < 100; ++id) {
      int l = rng() % n, r = rng() % n, t = rng() % 41;
      if (l > r) swap(l, r);
      qs.emplace_back(l, r, t, block, id);
    }
    shuffle(ALL(qs), rng); cur = initial;
    vector<bool> active(n), answered(100);
    ll sum = 0; int time = 0;
    auto add = [&](int i) {
      require(0 <= i && i < n && !active[i], "Mo duplicate add");
      active[i] = true; sum += cur[i];
    };
    auto sub = [&](int i) {
      require(0 <= i && i < n && active[i], "Mo inactive remove");
      active[i] = false; sum -= cur[i];
    };
    auto change = [&](int t, int l, int r, bool forward) {
      require(t == (forward ? time + 1 : time), "Mo time index");
      for (int i = 0; i < n; ++i)
        require(active[i] == (l <= i && i <= r), "Mo time range");
      auto u = updates.at(t);
      ll old = forward ? u.before : u.after;
      ll value = forward ? u.after : u.before;
      require(cur[u.pos] == old, "Mo repeated update rollback");
      if (active[u.pos]) sum += value - old;
      cur[u.pos] = value; time += forward ? 1 : -1;
    };
    auto answer = [&](const mo_update::Query &q) {
      require(!answered[q.id] && time == q.T, "Mo id/time");
      answered[q.id] = true;
      ll want = 0;
      for (int i = 0; i < n; ++i) {
        require(cur[i] == versions[q.T][i], "Mo array version");
        require(active[i] == (q.L <= i && i <= q.R), "Mo range");
        if (active[i]) want += cur[i];
      }
      require(sum == want, "Mo update answer");
    };
    mo_update::solve(qs,
      [&](int t, int l, int r) { change(t, l, r, true); },
      [&](int t, int l, int r) { change(t, l, r, false); },
      add, sub, answer);
    require(count(ALL(answered), true) == 100, "Mo missing answer");
  }
}
void mo_trees() {
  auto bad = [](int) { require(false, "empty tree Mo flip"); };
  mo_tree::solve({}, {}, bad, [](auto &) {
    require(false, "empty tree Mo answer");
  });
  for (int tc = 0; tc < 120; ++tc) {
    int n = 1 + rng() % 30;
    vector<vector<int>> g(n);
    vector<int> par(n, -1), dep(n), in(n), out(n), ord;
    for (int u = 1; u < n; ++u) {
      int p = tc % 3 == 0 ? 0 : tc % 3 == 1 ? u - 1 : rng() % u;
      par[u] = p; dep[u] = dep[p] + 1;
      g[p].pb(u); g[u].pb(p);
    }
    auto dfs = [&](auto &&self, int u) -> void {
      in[u] = SZ(ord); ord.pb(u);
      for (int v : g[u]) if (v != par[u]) self(self, v);
      out[u] = SZ(ord); ord.pb(u);
    };
    dfs(dfs, 0);
    auto lca = [&](int u, int v) {
      while (dep[u] > dep[v]) u = par[u];
      while (dep[v] > dep[u]) v = par[v];
      while (u != v) u = par[u], v = par[v];
      return u;
    };
    int block = 1 + rng() % (2 * n + 2);
    vector<mo_tree::Query> qs;
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
      qs.emplace_back(u, v, block, in, out, lca, u * n + v);
    shuffle(ALL(qs), rng);
    vector<bool> active(n), answered(n * n);
    int last_l = 0, last_r = -1;
    auto answer = [&](const mo_tree::Query &q) {
      int u = q.id / n, v = q.id % n, c = lca(u, v);
      vector<bool> want(n);
      for (; u != c; u = par[u]) want[u] = true;
      for (; v != c; v = par[v]) want[v] = true;
      want[c] = true;
      require(active == want, "tree Mo exact path membership");
      require(!answered[q.id], "tree Mo duplicate answer");
      answered[q.id] = true; last_l = q.L; last_r = q.R;
    };
    mo_tree::solve(qs, ord,
      [&](int u) { active.at(u) = !active.at(u); }, answer);
    vector<bool> want(n);
    for (int i = last_l; i <= last_r; ++i) want[ord[i]].flip();
    require(active == want, "tree Mo extra LCA left active");
    require(count(ALL(answered), true) == n * n, "tree Mo coverage");
  }
}
ll hilbert_oracle(int n, int x, int y) {
  if (n == 1) return 0;
  int h = n / 2, rx = x >= h, ry = y >= h;
  int quadrant = (3 * rx) ^ ry;
  x %= h; y %= h;
  if (!ry) {
    if (rx) x = h - 1 - x, y = h - 1 - y;
    swap(x, y);
  }
  return ll(quadrant) * h * h + hilbert_oracle(h, x, y);
}
void hilbert_edges() {
  for (int n = 1; n <= 64; n *= 2) {
    vector<pair<int, int>> points(n * n, {-1, -1});
    for (int x = 0; x < n; ++x) for (int y = 0; y < n; ++y) {
      ll z = hilbert(n, x, y);
      require(z == hilbert_oracle(n, x, y), "Hilbert orientation");
      require(points.at(z).first == -1, "Hilbert duplicate index");
      points[z] = {x, y};
    }
    for (int i = 1; i < n * n; ++i)
      require(abs(points[i].first - points[i - 1].first) +
              abs(points[i].second - points[i - 1].second) == 1,
              "Hilbert nonadjacent consecutive cells");
  }
  const int n = 1 << 30;
  for (int tc = 0; tc < 2000; ++tc) {
    int x = rng() % n, y = rng() % n;
    require(hilbert(n, x, y) == hilbert_oracle(n, x, y),
            "Hilbert largest int power of two");
  }
}
void hull_edges() {
  for (ll a : {LLONG_MIN, -8LL, -1LL, 0LL, 1LL, 8LL, LLONG_MAX})
    for (ll b : {-7LL, -1LL, 1LL, 7LL}) {
      if (a == LLONG_MIN && b == -1) continue;
      __int128 q = (__int128)a / b, r = (__int128)a % b;
      if (r && ((a < 0) != (b < 0))) --q;
      require(DynamicHull().Div(a, b) == q, "hull floor division");
    }
  DynamicHull flat;
  flat.addline(0, LLONG_MIN); flat.addline(0, LLONG_MAX);
  require(flat.query(0) == LLONG_MAX, "hull extreme equal slopes");
  const ll big = 4000000000000000000LL, inf = DynamicHull::kInf;
  for (int tc = 0; tc < 100; ++tc) {
    DynamicHull h; vector<pair<ll, ll>> lines;
    for (int i = 0; i < 60; ++i) {
      ll a = ll(rng() % 7) - 3;
      ll b = i % 3 == 0 ? big : i % 3 == 1 ? -big : ll(rng() % 31)-15;
      lines.pb({a, b}); h.addline(a, b);
      for (ll x : {-inf + 1, -100LL, -1LL, 0LL, 1LL, 100LL, inf}) {
        ll want = LLONG_MIN;
        for (auto [s, t] : lines) want = max(want, s * x + t);
        require(h.query(x) == want, "hull far intersections/ties");
      }
    }
  }
}
vector<string> subsequences(const string &s) {
  set<string> ret;
  for (int mask = 0; mask < (1 << SZ(s)); ++mask) {
    string t;
    for (int i = 0; i < SZ(s); ++i) if (mask >> i & 1) t += s[i];
    ret.insert(t);
  }
  return {ALL(ret)};
}
void all_lcs_edges() {
  for (int tc = 0; tc < 500; ++tc) {
    string a(rng() % 10, ' '), b(rng() % 10, ' ');
    const unsigned char values[] = {0, 1, 127, 128, 255};
    for (char &c : a) c = values[rng() % 5];
    for (char &c : b) c = values[rng() % 5];
    auto sa = subsequences(a), sb = subsequences(b);
    vector<string> common, want;
    set_intersection(ALL(sa), ALL(sb), back_inserter(common));
    size_t length = 0;
    for (const auto &s : common) length = max(length, s.size());
    for (const auto &s : common) if (s.size() == length) want.pb(s);
    require(all_lcs(a, b) == want, "all LCS embedded zero/high bytes");
  }
  require(all_lcs("", "aaa") == vector<string>{""}, "empty all LCS");
  require(all_lcs(string(300, 'a'), string(400, 'a')) ==
          vector<string>{string(300, 'a')}, "all LCS duplicate collapse");
}
void simpson_edges() {
  auto square = make_simpson([](double x) { return x * x; });
  for (auto [l, r] : vector<pair<double, double>>{
        {0, 0}, {-2, 3}, {3, -2}, {1, 1 + 1e-8}}) {
    double want = (r - l) * (l * l + l * r + r * r) / 3;
    require(abs(square.eval(l, r, 1e-12) - want) < 1e-10,
            "Simpson zero/reversed/tiny intervals");
    require(abs(square.eval2(l, r, 1e-12, 7) - want) < 1e-10,
            "Simpson split reversed interval");
  }
  for (double l : {1.0, 1e16, 8e307}) {
    double r = nextafter(l, INFINITY), value = l > 1e300 ? 1e-308 : 1;
    auto f = make_simpson([&](double x) {
      require(isfinite(x) && l <= x && x <= r, "Simpson sample range");
      return value;
    });
    double want = (r - l) * value;
    require(abs(f.eval(l, r, 1e-300) - want) <= abs(want) * 1e-14,
            "Simpson adjacent floating endpoints");
  }
  double l = 8e307, r = 1e308;
  auto f = make_simpson([&](double x) {
    require(isfinite(x) && l <= x && x <= r, "Simpson midpoint overflow");
    return (x * 1e-308) * 1e-308;
  });
  require(abs(f.eval(l, r, 1e-12) - 0.18) < 1e-12,
          "Simpson finite large-offset linear integral");
  require(abs(f.eval(r, l, 1e-12) + 0.18) < 1e-12,
          "Simpson large-offset reversed integral");
  auto ld = [](long double x) { return x * x * x; };
  Simpson<decltype(ld), long double> cubic{ld};
  require(abs(cubic.eval(0.L, 1.L, 1e-15L) - 0.25L) < 1e-15L,
          "Simpson long double numeric type");
}
void simpson_split_edges() {
  auto f = make_simpson([](double) { return 1.0; });
  for (int k : {1, 2, 7, 997}) {
    require(f.eval2(1e16, 1e16 + 2, 1e-12, k) == 2,
            "Simpson split lost narrow large-offset interval");
    require(f.eval2(1e16 + 2, 1e16, 1e-12, k) == -2,
            "Simpson split lost reversed large-offset interval");
  }
}
void annealing_edges() {
  struct State { int x; explicit State(int value) : x(value) {} };
  auto score = [](const State &s) { return s.x == 1 ? 0.0 : 1.0; };
  mt19937_64 r(71), saved = r;
  for (int iterations : {-1, 0}) {
    auto best = simulated_annealing(State(0), score,
      [](const State &, auto &) -> State {
        throw runtime_error("zero-iteration annealing proposed state");
      }, r, iterations);
    require(best.x == 0 && r == saved, "annealing zero iterations");
  }
  auto next = [](const State &s, auto &) { return State(s.x + 1); };
  auto best = simulated_annealing(State(0), score, next, r, 30, 1e300, 1);
  require(best.x == 1, "annealing retains best after accepting worse");
  auto downhill = [](int x, auto &) { return x - 1; };
  require(simulated_annealing(10, [](int x) { return double(x); },
          downhill, r, 10, 0, 0) == 0, "annealing temperature floor");
}
void hash_edges() {
  vector<vector<int>> leaf(1);
  for (auto seed : {0ULL, 1ULL, ULLONG_MAX})
    require(rooted_tree_hash(leaf, 0, seed) ==
            (seed ? seed : tree_hash_default_seed), "tree hash singleton");
  for (int tc = 0; tc < 100; ++tc) {
    int n = 1 + rng() % 100;
    vector<vector<int>> g(n), h(n);
    vector<int> perm(n); iota(ALL(perm), 0); shuffle(ALL(perm), rng);
    for (int u = 1; u < n; ++u) {
      int p = tc & 1 ? 0 : u - 1;
      g[u].pb(p); g[p].pb(u);
    }
    for (int u = 0; u < n; ++u) for (int v : g[u])
      h[perm[u]].pb(perm[v]);
    for (auto &row : h) shuffle(ALL(row), rng);
    int root = rng() % n;
    require(rooted_tree_hash(g, root, ULLONG_MAX) ==
            rooted_tree_hash(h, perm[root], ULLONG_MAX),
            "tree hash overflow wrap/relabel/order");
    require(rooted_tree_hash(g, root, 0) == rooted_tree_hash(g, root),
            "tree hash zero seed fallback");
  }
}
void fraction_edges() {
  const ll n = 1000000000000LL;
  for (Q target : {Q{0, 1}, Q{1, n}, Q{n, 1}, Q{n - 1, n}, Q{1, 0}}) {
    int calls = 0;
    auto pred = [&](Q x) {
      require(++calls < 10000, "fraction search lost acceleration");
      return !x.q || (__int128)x.p * target.q >= (__int128)target.p*x.q;
    };
    auto got = frac_bs(n, pred);
    require((__int128)got.p * target.q == (__int128)target.p * got.q,
            "fraction large zero/tiny/huge/infinity threshold");
  }
  for (auto [p, q, wp, wq] : vector<array<ll, 4>>{
        {0, 1, 1, n}, {1, 1, n, n - 1}, {1, 2, n / 2, n - 1}}) {
    auto got = frac_bs(n, [&](Q x) {
      return !x.q || (__int128)x.p * q > (__int128)p * x.q;
    });
    require((__int128)got.p * wq == (__int128)wp * got.q,
            "fraction large strict threshold");
  }
}
void convolution_edges() {
  for (int tc = 0; tc < 1500; ++tc) {
    int n = rng() % 45, m = rng() % 45;
    vector<ll> a(n), b(m), want(n && m ? n + m - 1 : 0, LLONG_MAX);
    ll slope = -20;
    if (n) a[0] = tc % 2 ? 4000000000000000000LL : -4000000000000000000LL;
    for (int i = 1; i < n; ++i) {
      if (tc % 3) slope += rng() % 4;
      a[i] = a[i - 1] + slope;
    }
    for (ll &x : b) x = ll(rng() % 2001) - 1000;
    for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j)
      want[i + j] = min(want[i + j], a[i] + b[j]);
    require(min_plus_convolution(a, b) == want, "min-plus extreme/ties");
  }
  for (ll x : {LLONG_MIN, LLONG_MAX})
    require(min_plus_convolution(vector<ll>{x}, vector<ll>{0}) ==
            vector<ll>{x}, "min-plus singleton ll extremes");
}
int lcs_length(const vector<int> &a, const vector<int> &b) {
  vector<int> dp(b.size() + 1);
  for (int x : a) {
    int diagonal = 0;
    for (int j = 0; j < SZ(b); ++j) {
      int old = dp[j + 1];
      dp[j + 1] = x == b[j] ? diagonal + 1 : max(dp[j], dp[j + 1]);
      diagonal = old;
    }
  }
  return dp.back();
}
void bitset_edges() {
  for (int m : {0, 1, 63, 64, 65, 127, 128, 129, 1023, 1024, 2049})
    for (int mode = 0; mode < 5; ++mode) {
      vector<int> a(m + 7), b(m);
      const int values[] = {INT_MIN, -1, 0, 1, INT_MAX};
      for (int i = 0; i < SZ(a); ++i)
        a[i] = values[mode ? rng() % 5 : 0];
      for (int i = 0; i < m; ++i)
        b[i] = values[mode ? rng() % 5 : 0];
      require(bitset_lcs(a, b) == lcs_length(a, b),
              "bitset LCS carries/borrows/high-word padding");
      require(bitset_lcs(b, a) == lcs_length(a, b), "bitset symmetry");
    }
}
void queen_edges() {
  for (int n = 0; n <= 130; ++n) {
    vector<int> q(rng() % 100, INT_MIN), saved = q;
    queens::solve(q, n);
    require(equal(ALL(saved), q.begin()), "queens appended prefix");
    int added = n == 2 || n == 3 ? 0 : n;
    require(q.size() == saved.size() + added, "queens empty/impossible");
    set<int> col, up, down;
    for (int i = 0; i < added; ++i) {
      int c = q[saved.size() + i];
      require(1 <= c && c <= n && col.insert(c).second &&
              up.insert(i + c).second && down.insert(i - c).second,
              "queens randomized prefix/diagonals");
    }
  }
}
int main(int argc, char **argv) {
  string test = argc > 1 ? argv[1] : "all";
  vector<pair<string, void (*)()>> tests = {
    {"cyclic", cyclic}, {"cdq", cdq}, {"mo_updates", mo_updates},
    {"mo_trees", mo_trees}, {"hilbert", hilbert_edges},
    {"hull", hull_edges}, {"all_lcs", all_lcs_edges},
    {"simpson", simpson_edges}, {"simpson_split", simpson_split_edges},
    {"annealing", annealing_edges},
    {"hash", hash_edges}, {"fractions", fraction_edges},
    {"convolution", convolution_edges}, {"bitset", bitset_edges},
    {"queens", queen_edges}
  };
  try {
    bool matched = false;
    for (auto [name, run] : tests) if (test == "all" || test == name) {
      matched = true; run(); cout << name << " PASS\n";
    }
    require(matched, "unknown test name");
  } catch (const exception &e) { cerr << e.what() << '\n'; return 1; }
}
