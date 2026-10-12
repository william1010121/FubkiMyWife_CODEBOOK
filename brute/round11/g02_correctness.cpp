#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
#define X first
#define Y second
#define SZ(a) ((int)(a).size())
#define ALL(a) (a).begin(), (a).end()
#define pb push_back

namespace fft_test {
#include "../../codebook/7_Polynomial/FFT_chrislaiisme.cpp"
}
namespace centroid_test {
constexpr int N = 128;
#include "../../codebook/3_Data_Structure/Centroid_Decomposition.cpp"
}
namespace manacher_test {
constexpr int MAXN = 1025; // Exact capacity, including final gap.
#include "../../codebook/5_String/Manacher.cpp"
}
namespace steiner_one {
constexpr int N = 7, T = 4, INF = 1;
#include "../../codebook/2_Graph/MinimumSteinerTree.cpp"
}
namespace steiner_seven {
constexpr int N = 7, T = 4, INF = 7;
#include "../../codebook/2_Graph/MinimumSteinerTree.cpp"
}
namespace lcs_test {
#include "../../codebook/9_Else/BitsetLCS.cpp"
}
namespace xor_test {
#include "../../codebook/3_Data_Structure/XOR_Basis.cpp"
}
namespace treap_test {
#include "../../codebook/3_Data_Structure/Treap.cpp"
}
namespace factorial_test {
constexpr int MAXP = 8193;
#include "../../codebook/6_Math/fac_no_p.cpp"
}

void require(bool ok, const char *label) {
  if (!ok) { cerr << "FAIL " << label << '\n'; exit(1); }
}

void fft_audit() {
  // Large common terms cancel to small differences; end values fit int.
  for (int n : {1, 2, 31, 32, 33, 255, 256, 257, 4096, 4097}) {
    for (int value : {INT_MAX, INT_MIN}) {
      vector<int> a(n, value), want(n + 1);
      if (value == INT_MIN) ++a.back();
      want[0] = a[0], want[n] = -a.back();
      for (int i = 1; i < n; ++i) want[i] = a[i] - a[i - 1];
      require(fft_test::mult(a, {1, -1}) == want, "FFT cancellation");
      require(fft_test::mult({1, -1}, a) == want, "FFT commutation");
    }
  }
  for (int n : {1, 2, 31, 32, 33, 255, 256, 257}) {
    vector<int> a(n), want(2 * n - 1);
    for (int i = 0; i < n; ++i) a[i] = (i * 37) % 101 - 50;
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
      want[i + j] += a[i] * a[j];
    require(fft_test::mult(a, a) == want, "FFT same input object");
  }
  require(fft_test::mult({}, {INT_MIN}).empty(), "FFT empty left");
  require(fft_test::mult({INT_MAX}, {}).empty(), "FFT empty right");
}

void centroid_audit() {
  static centroid_test::Cent_Dec cd;
  mt19937 rng(1102);
  // Reuse across size changes; choose mixed full-width signed weights.
  for (int n : {127, 1, 64, 2, 127, 3, 65}) {
    cd.init(n);
    vector<vector<pii>> g(n + 1);
    vector<int> order(n);
    iota(ALL(order), 1); shuffle(ALL(order), rng);
    for (int i = 1; i < n; ++i) {
      int u = order[i], v = order[rng() % i];
      int w = i % 3 == 0 ? INT_MIN : i % 3 == 1 ? INT_MAX : 0;
      cd.add_edge(u, v, w);
      g[u].pb({v, w}), g[v].pb({u, w});
    }
    cd.build();
    vector<vector<ll>> d(n + 1, vector<ll>(n + 1));
    for (int source = 1; source <= n; ++source) {
      vector<pii> stack{{source, 0}};
      while (!stack.empty()) {
        auto [u, parent] = stack.back(); stack.pop_back();
        for (auto [v, w] : g[u]) if (v != parent) {
          d[source][v] = d[source][u] + w;
          stack.pb({v, u});
        }
      }
      require(cd.query(source) == 0, "centroid reset");
    }
    vector<ll> counts(n + 1);
    for (int q = 0; q < 2000; ++q) {
      int marked = q % 3 ? order[0] : 1 + rng() % n;
      cd.modify(marked), ++counts[marked];
      if (q % 37 == 0 || q == 1999)
        for (int u = 1; u <= n; ++u) {
          ll want = 0;
          for (int v = 1; v <= n; ++v) want += counts[v] * d[u][v];
          require(cd.query(u) == want, "centroid signed multiplicity");
        }
    }
  }
}

vector<int> kmp_result(const string &s, const string &t) {
  ostringstream output;
  auto *saved = cout.rdbuf(output.rdbuf());
#include "../../codebook/5_String/KMP.cpp"
  cout.rdbuf(saved);
  // Independently check the public pattern prefix table as well.
  for (int i = 0; i < SZ(t); ++i) {
    int want = 0;
    for (int k = 1; k <= i; ++k)
      if (t.substr(0, k) == t.substr(i + 1 - k, k)) want = k;
    require(nxt[i] == want, "KMP byte prefix table");
  }
  istringstream parsed(output.str());
  vector<int> answer;
  for (int x; parsed >> x;) answer.pb(x);
  return answer;
}

void string_audit() {
  // Bijectively relabel all 256 bytes: separators cannot be special.
  for (int shift = 0; shift < 256; ++shift) {
    string s;
    for (int i = 0; i < 256; ++i) s += char((i + shift) & 255);
    s += string(s.rbegin(), s.rend());
    for (const string &text : {s, s.substr(0, 257), string()}) {
      fill_n(manacher_test::z, manacher_test::MAXN, -1);
      manacher_test::Manacher(text);
      int length = 2 * SZ(text) + 1;
      vector<int> expanded(length, 256);
      for (int i = 0; i < SZ(text); ++i)
        expanded[2 * i + 1] = (unsigned char)text[i];
      for (int i = 0; i < length; ++i) {
        int radius = 1;
        while (i - radius >= 0 && i + radius < length &&
               expanded[i - radius] == expanded[i + radius]) ++radius;
        require(manacher_test::z[i] == radius, "Manacher byte mirror");
      }
      for (int i = length; i < manacher_test::MAXN; ++i)
        require(manacher_test::z[i] == -1, "Manacher untouched suffix");
    }
    for (int start : {0, 1, 127, 255, 256, 511})
      for (int length : {0, 1, 2, 17}) {
        string pattern = s.substr(start, length);
        vector<int> want;
        for (int i = 0; i + SZ(pattern) <= SZ(s); ++i)
          if (s.compare(i, pattern.size(), pattern) == 0)
            want.pb(i + 1);
        require(kmp_result(s, pattern) == want, "KMP byte permutation");
      }
  }
  require(kmp_result("", "") == vector<int>{1}, "KMP empty both");
  require(kmp_result("", string(1, '\0')).empty(), "KMP empty text");
}

struct Edge { int u, v, w; };
int steiner_oracle(int n, const vector<Edge> &edges,
                   const vector<int> &term, const vector<int> &vc,
                   int inf) {
  if (term.empty()) return 0;
  int answer = inf;
  for (int mask = 0; mask < (1 << SZ(edges)); ++mask) {
    vector<int> reach(n), used(n);
    for (int u = 0; u < n; ++u) reach[u] = 1 << u;
    for (int u : term) used[u] = 1;
    ll cost = 0;
    for (int e = 0; e < SZ(edges); ++e) if (mask >> e & 1) {
      auto [u, v, w] = edges[e];
      cost += w; used[u] = used[v] = 1; reach[u] |= 1 << v;
    }
    for (int u = 0; u < n; ++u) cost += used[u] * vc[u];
    for (int k = 0; k < n; ++k) for (int u = 0; u < n; ++u)
      if (reach[u] >> k & 1) reach[u] |= reach[k];
    int common = (1 << n) - 1;
    for (int u : term) common &= reach[u];
    if (common) answer = min<ll>(answer, cost);
  }
  return answer;
}

template<class Tree> void steiner_audit(int inf) {
  Tree st;
  auto check = [&](int n, vector<Edge> es, vector<int> term,
                   vector<int> vc) {
    st.init(n);
    for (int i = 0; i < n; ++i) st.vcst[i] = vc[i];
    for (auto [u, v, w] : es) st.add_edge(u, v, w);
    require(st.solve(term) == steiner_oracle(n, es, term, vc, inf),
            "Steiner small INF edge-subset oracle");
  };
  check(0, {}, {}, {});
  mt19937 rng(1103);
  for (int tc = 0; tc < 1500; ++tc) {
    int n = 1 + rng() % 7;
    vector<int> vc(n), term;
    vector<Edge> es;
    for (int &x : vc) x = rng() % inf;
    for (int i = 0; i < 8; ++i)
      es.pb({int(rng() % n), int(rng() % n), int(rng() % inf)});
    // More than T entries, but at most T distinct terminals.
    for (int k = 0; k < 4; ++k) if (rng() & 1) {
      int u = rng() % n;
      for (int i = 0; i < 5; ++i) term.pb(u);
    }
    shuffle(ALL(term), rng); check(n, es, term, vc);
  }
}

int lcs_oracle(const vector<int> &a, const vector<int> &b) {
  vector<int> dp(b.size() + 1);
  for (int x : a) {
    int diagonal = 0;
    for (int j = 0; j < SZ(b); ++j) {
      int old = dp[j + 1];
      dp[j + 1] = x == b[j] ? diagonal + 1 : max(dp[j], old);
      diagonal = old;
    }
  }
  return dp.back();
}
void lcs_audit() {
  for (int n : {0, 1, 63, 64, 65, 127, 128, 129, 191, 192, 193})
    for (int hole : {0, 1, 62, 63, 64, 65, 126, 127, 128, 192}) {
      vector<int> a(n, INT_MIN), b(n + 1, INT_MIN);
      if (hole < n) a[hole] = INT_MAX;
      if (hole < SZ(b)) b[hole] = INT_MAX;
      // Fill low words, then advance a match across word boundaries.
      for (int shift : {0, 1, 63, 64, 65}) {
        auto c = b;
        rotate(c.begin(), c.begin() + shift % SZ(c), c.end());
        for (int absent : {0, 1}) {
          auto d = a;
          if (absent) d.insert(d.begin(), 70, 17);
          int want = lcs_oracle(d, c);
          require(lcs_test::bitset_lcs(d, c) == want, "LCS carry/borrow");
          require(lcs_test::bitset_lcs(c, d) == want, "LCS symmetry");
        }
      }
    }
  require(lcs_test::bitset_lcs({INT_MAX}, {}) == 0, "LCS empty b");
}

void xor_audit() {
  mt19937_64 rng(1104);
  for (int tc = 0; tc < 30; ++tc) {
    vector<unsigned long long> rows, span{0};
    for (int i = 0; i < 12; ++i)
      rows.pb((1ULL << (63 - i)) | (rng() & ((1ULL << 51) - 1)));
    for (auto x : rows) {
      int size = SZ(span);
      for (int i = 0; i < size; ++i) span.pb(span[i] ^ x);
    }
    sort(ALL(span)); shuffle(ALL(rows), rng);
    fill_n(xor_test::basis, 64, 0ULL);
    xor_test::ins(0);
    for (auto x : rows) xor_test::ins(x);
    vector<unsigned long long> before(xor_test::basis,
                                      xor_test::basis + 64);
    for (auto x : span) {
      xor_test::ins(x);
      require(equal(ALL(before), xor_test::basis), "XOR sparse span");
    }
    unsigned long long outside;
    do { outside = rng(); } while (binary_search(ALL(span), outside));
    xor_test::ins(outside);
    require(count_if(xor_test::basis, xor_test::basis + 64,
                     [](auto x) { return x != 0; }) == 13,
            "XOR nonmember adds rank");
  }
}

void treap_audit() {
  using namespace treap_test;
  for (int shape = 0; shape < 3; ++shape) {
    node *root = nullptr;
    vector<int> want;
    // Equal priorities and both priority orders force tall valid trees.
    for (int i = 0; i < 127; ++i) {
      int key = i == 0 ? INT_MIN : i == 126 ? INT_MAX : i / 3;
      node *p = new node(key);
      p->priority = shape == 0 ? 0 : shape == 1 ? i : 126 - i;
      root = merge(root, p), want.pb(key);
    }
    auto check = [&](auto &&self, node *p, int &at) -> void {
      if (!p) return;
      require(p->sz == 1 + sz(p->l) + sz(p->r), "Treap chain size");
      require(!p->l || p->priority <= p->l->priority, "Treap left heap");
      require(!p->r || p->priority <= p->r->priority, "Treap right heap");
      self(self, p->l, at);
      require(at < SZ(want) && p->data == want[at++], "Treap order");
      self(self, p->r, at);
    };
    for (int k = 0; k <= 127; ++k) {
      node *other;
      split2(root, root, other, k);
      require(sz(root) == k && sz(other) == 127 - k, "Treap left alias");
      root = merge(root, other);
      split2(root, other, root, k);
      require(sz(other) == k && sz(root) == 127 - k, "Treap right alias");
      root = merge(other, root);
      interval(root, k + 1, 127 - k);
      int at = 0; check(check, root, at);
      require(at == 127, "Treap traversal count");
    }
    while (!want.empty()) {
      int i = SZ(want) / 2, key = want[i];
      require(kth(root, i + 1)->data == key, "Treap kth on chain");
      int rank = lower_bound(ALL(want), key) - want.begin();
      require(Rank(root, key) == rank, "Treap strict rank on chain");
      require(erase(root, key), "Treap chain erase");
      want.erase(want.begin() + rank);
      int at = 0; check(check, root, at);
      require(at == SZ(want), "Treap deletion count");
    }
    require(!root && !erase(root, INT_MIN), "Treap empty erase");
  }
}

void factorial_audit() {
  // Prefix table is observable; rebuild large/small moduli out of order.
  for (auto [p, pk] : vector<pll>{{2, 8192}, {3, 6561}, {2, 2},
                                 {5, 3125}, {2, 4}, {2, 8192}}) {
    for (ll n : {0LL, 1LL, pk - 1, pk, pk + 1, 2 * pk + 1}) {
      ll want = 1;
      for (ll i = 1; i <= n; ++i) {
        ll x = i;
        while (x % p == 0) x /= p;
        want = want * (x % pk) % pk;
      }
      fill_n(factorial_test::prod, pk + 1, -1LL);
      require(factorial_test::fac_no_p(n, p, pk) == want,
              "factorial stripped product");
      ll prefix = 1;
      for (int i = 0; i <= pk; ++i) {
        if (i && i % p) prefix = prefix * i % pk;
        require(factorial_test::prod[i] == prefix, "factorial full table");
      }
      // Unit factorial recurrence at ll limits, independently with pow.
      n = LLONG_MAX;
      ll result = factorial_test::fac_no_p(n, p, pk);
      ll power = 1, base = prefix, exponent = n / pk;
      for (; exponent; exponent >>= 1, base = base * base % pk)
        if (exponent & 1) power = power * base % pk;
      ll rhs = power * factorial_test::prod[n % pk] % pk;
      rhs = rhs * factorial_test::fac_no_p(n / p, p, pk) % pk;
      require(result == rhs, "factorial ll-limit recurrence");
    }
  }
}

int main() {
  fft_audit(); centroid_audit(); string_audit();
  steiner_audit<steiner_one::SteinerTree>(1);
  steiner_audit<steiner_seven::SteinerTree>(7);
  lcs_audit(); xor_audit(); treap_audit(); factorial_audit();
  cout << "PASS g02 round11 adversarial audit (nine templates)\n";
}
