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

void require(bool ok, const char *message) {
  if (!ok) throw runtime_error(message);
}
void cyclic_audit() {
  for (int n = 1; n <= 40; ++n) for (int pivot = 0; pivot < n; ++pivot) {
    vector<int> a{INT_MIN};
    for (int i = 1; i < n - 1; ++i) if (i <= pivot) a.pb(i);
    if (n > 1) a.pb(INT_MAX);
    for (int i = n - 2; i > pivot; --i) a.pb(i);
    for (int shift = 0; shift < n; ++shift) {
      auto less = [&](int i, int j) { return a.at(i) < a.at(j); };
      auto greater = [&](int i, int j) { return a.at(i) > a.at(j); };
      require(a[cyc_tsearch(n, less)] == INT_MIN, "cyclic minimum");
      require(a[cyc_tsearch(n, greater)] == (n == 1 ? INT_MIN : INT_MAX),
              "cyclic reversed comparator");
      rotate(a.begin(), a.begin() + 1, a.end());
    }
  }
}
void cdq_audit() {
  const int values[] = {INT_MIN, 0, INT_MAX};
  // Every ordered four-point input in this tied 2D coordinate grid.
  for (int code = 0; code < 6561; ++code) {
    vector<vector<int>> a(4, vector<int>(2));
    int z = code;
    for (auto &row : a) for (int &x : row) x = values[z % 3], z /= 3;
    auto got = dominance(a);
    for (int i = 0; i < 4; ++i) {
      int want = 0;
      for (int j = 0; j < 4; ++j)
        want += i != j && a[j][0] <= a[i][0] && a[j][1] <= a[i][1];
      require(got[i] == want, "CDQ exhaustive coordinate ties");
    }
  }
  for (int k : {0, 1, 2, 3, 10, 100}) {
    require(dominance({vector<int>(k, INT_MAX)}) == vector<ll>{0},
            "CDQ singleton dimension boundary");
    require(dominance(vector<vector<int>>(13, vector<int>(k, INT_MIN)))
            == vector<ll>(13, 12), "CDQ identical rows");
  }
  require(dominance({}).empty(), "CDQ empty");
}
void mo_audit() {
  for (int block : {1, 2, INT_MAX}) {
    vector<mo_update::Query> qs;
    // All intervals/times, twice: equal sort keys must retain both ids.
    for (int copy = 0; copy < 2; ++copy)
      for (int l = 0; l < 3; ++l) for (int r = l; r < 3; ++r)
        for (int t = 0; t <= 4; ++t)
          qs.emplace_back(l, r, t, block, SZ(qs));
    vector<int> values{INT_MIN, 0, INT_MAX}, active(3), seen(qs.size());
    int time = 0;
    auto change = [&](int t, int l, int r, int direction) {
      require(t == time + (direction > 0), "Mo update numbering");
      for (int i = 0; i < 3; ++i)
        require(active[i] == (l <= i && i <= r), "Mo update range");
      // Four repeated updates/no-ops at position 1.
      const int version[] = {0, INT_MIN, INT_MIN, INT_MAX, 0};
      time += direction; values[1] = version[time];
    };
    auto answer = [&](const mo_update::Query &q) {
      require(time == q.T && ++seen[q.id] == 1, "Mo duplicate query");
      const int version[] = {0, INT_MIN, INT_MIN, INT_MAX, 0};
      require(values[1] == version[q.T], "Mo undo repeated update");
      for (int i = 0; i < 3; ++i)
        require(active[i] == (q.L <= i && i <= q.R), "Mo interval");
    };
    mo_update::solve(qs,
      [&](int t, int l, int r) { change(t, l, r, 1); },
      [&](int t, int l, int r) { change(t, l, r, -1); },
      [&](int i) { require(!active.at(i), "Mo add"); active[i] = 1; },
      [&](int i) { require(active.at(i), "Mo sub"); active[i] = 0; },
      answer);
    require(count(ALL(seen), 1) == SZ(qs), "Mo answers lost");
    vector<int> in{0}, out{1}, ord{0, 0};
    vector<mo_tree::Query> paths;
    for (int id = 0; id < 100; ++id)
      paths.emplace_back(0, 0, block, in, out,
                         [](int, int) { return 0; }, id);
    int on = 0, answers = 0;
    mo_tree::solve(paths, ord, [&](int u) {
      require(u == 0, "tree Mo singleton vertex"); on ^= 1;
    }, [&](const auto &q) {
      require(on && q.lca == -1 && q.L == 0 && q.R == 0,
              "tree Mo repeated singleton path");
      ++answers;
    });
    require(answers == 100 && on, "tree Mo singleton answers");
  }
}
void hilbert_audit() {
  for (int k = 0; k <= 30; ++k) {
    int n = 1 << k; ll area = ll(n) * n;
    require(hilbert(n, 0, 0) == 0, "Hilbert start corner");
    require(hilbert(n, n - 1, 0) == area - 1, "Hilbert last corner");
    if (k) {
      require(hilbert(n, 0, n - 1) == (area - 1) / 3,
              "Hilbert upper left corner");
      require(hilbert(n, n - 1, n - 1) == 2 * ((area - 1) / 3),
              "Hilbert upper right corner");
    }
  }
}
void hull_audit() {
  vector<pair<ll, ll>> lines;
  for (ll a = -2; a <= 2; ++a) for (ll b : {-2LL, 0LL, 2LL})
    lines.pb({a, b});
  // All 4-line subsets and insertion orders; exact/negative crossings.
  for (int a = 0; a < SZ(lines); ++a)
    for (int b = a + 1; b < SZ(lines); ++b)
      for (int c = b + 1; c < SZ(lines); ++c)
        for (int d = c + 1; d < SZ(lines); ++d) {
          vector<int> order{a, b, c, d};
          do {
            DynamicHull hull; vector<pair<ll, ll>> added;
            for (int i : order) {
              added.pb(lines[i]); hull.addline(lines[i].first,
                                              lines[i].second);
              for (ll x = -4; x <= 4; ++x) {
                ll want = LLONG_MIN;
                for (auto [s, t] : added) want = max(want, s * x + t);
                require(hull.query(x) == want, "hull insertion order");
              }
            }
          } while (next_permutation(ALL(order)));
        }
  DynamicHull hull;
  for (int i = 0; i < 10000; ++i) hull.addline(0, -i);
  require(hull.query(-DynamicHull::kInf + 1) == 0 &&
          hull.query(DynamicHull::kInf) == 0,
          "hull many inferior equal slopes");
}
set<string> subsequences(const string &s) {
  set<string> ret;
  for (int mask = 0; mask < (1 << SZ(s)); ++mask) {
    string t;
    for (int i = 0; i < SZ(s); ++i) if (mask >> i & 1) t += s[i];
    ret.insert(t);
  }
  return ret;
}
void lcs_audit() {
  vector<string> strings{""};
  const char alphabet[] = {0, char(128), char(255)};
  for (int len = 1, limit = 3; len <= 4; ++len, limit *= 3)
    for (int code = 0; code < limit; ++code) {
      string s(len, ' '); int z = code;
      for (char &c : s) c = alphabet[z % 3], z /= 3;
      strings.pb(s);
    }
  vector<set<string>> sets;
  for (const auto &s : strings) sets.pb(subsequences(s));
  for (int i = 0; i < SZ(strings); ++i)
    for (int j = 0; j < SZ(strings); ++j) {
      vector<string> common, want;
      set_intersection(ALL(sets[i]), ALL(sets[j]), back_inserter(common));
      size_t len = 0;
      for (const auto &s : common) len = max(len, s.size());
      for (const auto &s : common) if (s.size() == len) want.pb(s);
      require(all_lcs(strings[i], strings[j]) == want,
              "all LCS exhaustive byte alphabet");
      vector<int> a(ALL(strings[i])), b(ALL(strings[j]));
      require(bitset_lcs(a, b) == int(len), "bitset exhaustive oracle");
    }
  for (int n : {63, 64, 65, 127, 128, 129, 4096, 4097}) {
    vector<int> a(n); iota(ALL(a), INT_MIN);
    vector<int> b = a; reverse(ALL(b));
    require(bitset_lcs(a, b) == 1, "reversed distinct bitset symbols");
    require(bitset_lcs(a, a) == n, "bitset all word bits set");
  }
}
void simpson_audit() {
  // Cubics need no refinement in exact arithmetic; allow roundoff.
  for (double a : {-5.0, 0.0, 7.0})
    for (double b : {-2.0, 0.0, 3.0}) {
      auto f = make_simpson([=](double x) { return a*x*x*x + b*x + 1; });
      for (auto [l, r] : vector<pair<double, double>>{
            {-2, 3}, {3, -2}, {0, 0}, {-1e-5, 1e-5}}) {
        auto primitive = [=](double x) { return a*x*x*x*x/4+b*x*x/2+x; };
        double want = primitive(r) - primitive(l);
        for (int k : {1, 2, 3, 997})
          require(abs(f.eval2(l, r, 1e-10, k) - want) < 1e-9,
                  "Simpson signed/zero/cubic split intervals");
      }
    }
  int calls = 0;
  auto f = make_simpson([&](double x) { ++calls; return x*x; });
  auto mixed = f.mix({0, 0}, {2, 4}, 1);
  require(calls == 0 && mixed.first == 1 &&
          abs(mixed.second - 8.0/3) < 1e-15, "Simpson supplied midpoint");
  require(abs(f.eval({0, 0}, {2, 4}, 1, 1e-12) - 8.0/3) < 1e-15 &&
          calls == 2, "Simpson supplied pair overload sample reuse");
}
void annealing_audit() {
  struct CopyOnly {
    int x;
    explicit CopyOnly(int value) : x(value) {}
    CopyOnly(const CopyOnly &s) : x(s.x) {}
    CopyOnly &operator=(const CopyOnly &s) { x = s.x; return *this; }
  };
  mt19937_64 rng(8011), untouched = rng;
  auto next = [](const CopyOnly &s, auto &) { return CopyOnly(s.x-1); };
  auto score = [](const CopyOnly &s) { return double(s.x); };
  auto best = simulated_annealing(CopyOnly(100), score, next,
                                 rng, 100, 0, 0);
  require(best.x == 0 && rng == untouched,
          "copy-only annealing downhill skips acceptance RNG");
  int proposals = 0;
  best = simulated_annealing(CopyOnly(0), score,
    [&](const CopyOnly &s, auto &) {
      ++proposals; require(s.x == 0, "zero-temp uphill accepted");
      return CopyOnly(1);
    }, rng, 100, 0, 1);
  require(best.x == 0 && proposals == 100, "annealing rejects uphill");
}
void tree_audit() {
  for (int n : {1, 2, 3, 127, 2047}) {
    vector<vector<int>> g(n);
    for (int i = 1; i < n; ++i) g[(i-1)/2].pb(i), g[i].pb((i-1)/2);
    for (auto seed : {0ULL, 1ULL, 1ULL << 63, ULLONG_MAX}) {
      auto value = seed ? seed : tree_hash_default_seed;
      vector<tree_hash_u64> expected(n, value);
      // Independent bottom-up oracle also checks the public subtree API.
      for (int i = n - 1; i > 0; --i) {
        auto x = expected[i]; x ^= x << 13; x ^= x >> 7; x ^= x << 17;
        expected[(i-1)/2] += x;
      }
      for (int i = 0; i < n; ++i)
        require(tree_hash_dfs(g, i, i ? (i-1)/2 : -1, seed) == expected[i],
                "tree hash subtree/unsigned wrapping");
      require(rooted_tree_hash(g, 0, seed) == expected[0], "rooted hash");
    }
  }
}
bool fraction_less(Q a, Q b) {
  if (!a.q || !b.q) return b.q == 0 && a.q != 0;
  return (__int128)a.p * b.q < (__int128)b.p * a.q;
}
void fraction_audit() {
  for (int n = 1; n <= 30; ++n) {
    vector<Q> possible;
    for (int q = 1; q <= n; ++q) for (int p = 0; p <= n; ++p)
      possible.pb({p, q});
    sort(ALL(possible), fraction_less);
    possible.erase(unique(ALL(possible), [](Q a, Q b) {
      return a.p*b.q == b.p*a.q;
    }), possible.end());
    possible.pb({1, 0});
    auto check = [&](Q target, bool strict) {
      auto pred = [&](Q x) {
        return strict ? fraction_less(target, x) || !x.q
                      : !fraction_less(x, target);
      };
      auto it = find_if(ALL(possible), pred);
      Q got = frac_bs(n, pred);
      require(pred(got) && 0 <= got.p && got.p <= n &&
              0 <= got.q && got.q <= n &&
              !fraction_less(got, *it) && !fraction_less(*it, got),
              "fraction adjacent/exact bounded thresholds");
    };
    for (int i = 0; i < SZ(possible); ++i) {
      Q x = possible[i]; check(x, false); check(x, true);
      if (i && x.q) {
        Q y = possible[i-1];
        check({x.p*y.q + y.p*x.q, 2*x.q*y.q}, false);
      }
    }
  }
}
void convolution_audit() {
  for (int n = 1; n <= 6; ++n) {
    vector<ll> a(n, 4000000000000000000LL);
    auto generate = [&](auto &&self, int i, int previous) -> void {
      if (i < n) {
        for (int slope = previous; slope <= 2; ++slope) {
          a[i] = a[i-1] + slope; self(self, i+1, slope);
        }
        return;
      }
      for (int m = 1; m <= 5; ++m)
        for (int mask = 0; mask < (1<<m); ++mask) {
        vector<ll> b(m), want(n+m-1, LLONG_MAX);
        for (int j = 0; j < m; ++j) b[j] = mask>>j & 1 ? -2 : 2;
        for (int x = 0; x < n; ++x) for (int y = 0; y < m; ++y)
          want[x+y] = min(want[x+y], a[x]+b[y]);
        require(min_plus_convolution(a, b) == want,
                "min-plus exhaustive weak convexity/ties");
      }
    };
    generate(generate, 1, -2);
  }
  require(min_plus_convolution(vector<ll>{}, vector<ll>{1}).empty(),
          "min-plus empty");
}
void queens_audit() {
  vector<int> q;
  for (int n = 0; n <= 200; ++n) {
    auto size = q.size(); auto saved = q;
    queens::solve(q, n);
    require(equal(ALL(saved), q.begin()), "queen repeated append prefix");
    int added = n == 2 || n == 3 ? 0 : n;
    require(q.size() == size + added, "queen impossible/empty size");
    set<int> columns, up, down;
    for (int i = 0; i < added; ++i) {
      int c = q[size+i];
      require(1 <= c && c <= n && columns.insert(c).second &&
              up.insert(c+i).second && down.insert(c-i).second,
              "queen repeated append diagonals");
    }
  }
}
int main() {
  cyclic_audit(); cdq_audit(); mo_audit(); hilbert_audit(); hull_audit();
  lcs_audit(); simpson_audit(); annealing_audit(); tree_audit();
  fraction_audit(); convolution_audit(); queens_audit();
  cout << "g11 round8 audit PASS\n";
}
