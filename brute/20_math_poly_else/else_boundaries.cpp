#include <bits/stdc++.h>
using namespace std;

namespace simpson_test {
#include "../../codebook/9_Else/AdaptiveSimpson.cpp"
}
namespace all_lcs_test {
#include "../../codebook/9_Else/All_LCS.cpp"
}
namespace bitset_lcs_test {
#include "../../codebook/9_Else/BitsetLCS.cpp"
}
namespace digit_test {
#include "../../codebook/9_Else/digit_dp.cpp"
}
namespace tree_test {
#include "../../codebook/9_Else/tree_hash.cpp"
}

static vector<string> all_lcs_oracle(const string &a, const string &b) {
  set<string> sa, sb;
  for (int mask = 0; mask < (1 << a.size()); ++mask) {
    string x;
    for (int i = 0; i < (int)a.size(); ++i) if (mask >> i & 1) x += a[i];
    sa.insert(x);
  }
  for (int mask = 0; mask < (1 << b.size()); ++mask) {
    string x;
    for (int i = 0; i < (int)b.size(); ++i) if (mask >> i & 1) x += b[i];
    sb.insert(x);
  }
  vector<string> ret;
  for (const string &x : sa) if (sb.count(x)) ret.push_back(x);
  size_t best = 0;
  for (const string &x : ret) best = max(best, x.size());
  ret.erase(remove_if(ret.begin(), ret.end(), [&](const string &x) {
    return x.size() != best;
  }), ret.end());
  return ret;
}

static int lcs_oracle(const vector<int> &a, const vector<int> &b) {
  vector<vector<int>> dp(a.size() + 1, vector<int>(b.size() + 1));
  for (int i = (int)a.size() - 1; i >= 0; --i)
    for (int j = (int)b.size() - 1; j >= 0; --j)
      dp[i][j] = a[i] == b[j] ? 1 + dp[i + 1][j + 1]
        : max(dp[i + 1][j], dp[i][j + 1]);
  return dp[0][0];
}

static bool no_adjacent_one(int x) { return x >= 0 && !(x & (x >> 1)); }

static long long digit_oracle(int x) {
  function<long long(int, bool, bool)> go = [&](int bit, bool tight, bool prev) {
    if (bit < 0) return 1LL;
    int lim = tight ? ((x >> bit) & 1) : 1;
    long long ret = 0;
    for (int cur = 0; cur <= lim; ++cur) {
      if (prev && cur) continue;
      ret += go(bit - 1, tight && cur == lim, cur);
    }
    return ret;
  };
  return x < 0 ? 0 : go(30, true, false);
}

static string canonical_tree(const vector<vector<int>> &g, int u, int p) {
  vector<string> child;
  for (int v : g[u]) if (v != p) child.push_back(canonical_tree(g, v, u));
  sort(child.begin(), child.end());
  string ret = "(";
  for (const string &x : child) ret += x;
  return ret + ")";
}

static vector<vector<int>> make_tree(const vector<int> &parent) {
  vector<vector<int>> g(parent.size());
  for (int i = 1; i < (int)parent.size(); ++i)
    g[i].push_back(parent[i]), g[parent[i]].push_back(i);
  return g;
}

static void check_tree_family(const vector<int> &parent, mt19937 &rng) {
  auto g = make_tree(parent);
  int n = g.size();
  vector<int> order(n);
  iota(order.begin(), order.end(), 0);
  for (int root = 0; root < n; ++root) {
    string shape = canonical_tree(g, root, -1);
    for (uint64_t seed : {0ULL, 1ULL, 0x9e3779b97f4a7c15ULL,
                          0x123456789abcdef0ULL}) {
      auto want = tree_test::rooted_tree_hash(g, root, seed);
      static map<pair<uint64_t, uint64_t>, string> seen;
      auto key = make_pair(seed, want);
      auto it = seen.find(key);
      if (it != seen.end() && it->second != shape) {
        cerr << "tree hash collision for distinct canonical rooted trees\n";
        exit(1);
      }
      seen[key] = shape;
      vector<int> perm = order;
      shuffle(perm.begin() + 1, perm.end(), rng);
      vector<vector<int>> h(n);
      for (int u = 0; u < n; ++u) for (int v : g[u]) h[perm[u]].push_back(perm[v]);
      for (auto &adj : h) shuffle(adj.begin(), adj.end(), rng);
      auto got = tree_test::rooted_tree_hash(h, perm[root], seed);
      if (got != want) {
        cerr << "tree hash changed under relabel/order root=" << root << '\n';
        exit(1);
      }
    }
  }
}

int main() {
  {
    auto constant = simpson_test::make_simpson([](double) { return 3.0; });
    auto linear = simpson_test::make_simpson([](double x) { return x; });
    auto square = simpson_test::make_simpson([](double x) { return x * x; });
    auto wave = simpson_test::make_simpson([](double x) { return sin(x); });
    double c = constant.eval(-2.0, 5.0, 1e-12);
    double l = linear.eval(-2.0, 3.0, 1e-12);
    double q = square.eval(0.0, 1.0, 1e-12);
    double w = wave.eval2(0.0, acos(-1.0), 1e-11, 13);
    double tiny = constant.eval(2.0, 2.0 + 1e-12, 1e-20);
    if (abs(c - 21.0) > 1e-10 || abs(l - 2.5) > 1e-10 ||
        abs(q - 1.0 / 3) > 1e-10 || abs(w - 2.0) > 1e-9 ||
        abs(tiny - 3e-12) > 1e-15) {
      cerr << setprecision(17) << "Simpson values " << c << ' ' << l << ' '
           << q << ' ' << w << ' ' << tiny << '\n';
      cerr << "Adaptive Simpson exact-function boundary mismatch\n";
      return 1;
    }
  }
  vector<string> strings;
  for (int len = 0; len <= 5; ++len) {
    int lim = 1 << len;
    for (int mask = 0; mask < lim; ++mask) {
      string s(len, 'a');
      for (int i = 0; i < len; ++i) if (mask >> i & 1) s[i] = 'b';
      strings.push_back(s);
    }
  }
  for (const string &a : strings) for (const string &b : strings)
    if (all_lcs_test::all_lcs(a, b) != all_lcs_oracle(a, b)) {
      cerr << "All_LCS exhaustive mismatch a=" << a << " b=" << b << '\n';
      return 1;
    }
  mt19937 rng(0xe15);
  for (int tc = 0; tc < 180; ++tc) {
    string a(1 + rng() % 8, 'a'), b(1 + rng() % 8, 'a');
    for (char &c : a) c = 'a' + rng() % 3;
    for (char &c : b) c = 'a' + rng() % 3;
    if (all_lcs_test::all_lcs(a, b) != all_lcs_oracle(a, b)) {
      cerr << "All_LCS random mismatch\n"; return 1;
    }
  }

  for (int na : {0, 1, 2, 3, 62, 63, 64, 65, 127, 128, 129, 180})
    for (int nb : {0, 1, 2, 63, 64, 65, 127, 128, 131, 193}) {
      vector<int> a(na), b(nb);
      for (int i = 0; i < na; ++i) a[i] = (i * 17) % 9 - 4;
      for (int i = 0; i < nb; ++i) b[i] = (i * 11) % 7 - 3;
      if (bitset_lcs_test::bitset_lcs(a, b) != lcs_oracle(a, b)) {
        cerr << "Bitset LCS boundary mismatch na=" << na << " nb=" << nb << '\n';
        return 1;
      }
    }
  for (int tc = 0; tc < 300; ++tc) {
    int na = rng() % 180, nb = rng() % 190;
    vector<int> a(na), b(nb);
    for (int &x : a) x = (int)(rng() % 17) - 8;
    for (int &x : b) x = (int)(rng() % 17) - 8;
    if (bitset_lcs_test::bitset_lcs(a, b) != lcs_oracle(a, b)) {
      cerr << "Bitset LCS random mismatch\n"; return 1;
    }
  }

  for (int x = -5; x <= 100000; ++x) {
    int want = 0;
    for (int i = 0; i <= x; ++i) want += no_adjacent_one(i);
    if (digit_test::calc(x) != want) {
      cerr << "digit DP exhaustive mismatch x=" << x << '\n'; return 1;
    }
  }
  for (int x : {1 << 29, (1 << 29) - 1, (1 << 30) - 1, INT_MAX}) {
    if (digit_test::calc(x) != digit_oracle(x)) {
      cerr << "digit DP boundary mismatch x=" << x << '\n'; return 1;
    }
  }

  for (int n = 1; n <= 8; ++n) {
    vector<int> parent(n);
    function<void(int)> go = [&](int at) {
      if (at == n) { check_tree_family(parent, rng); return; }
      for (int p = 0; p < at; ++p) parent[at] = p, go(at + 1);
    };
    go(1);
  }
  cout << "else_boundaries: PASS Simpson/All_LCS/BitsetLCS/digit_dp/tree_hash\n";
}
