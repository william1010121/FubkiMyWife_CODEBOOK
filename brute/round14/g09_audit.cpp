#include "../../codebook/1_Basic/Default_code.cpp"
#include <unistd.h>

namespace nt {
#include "../../codebook/7_Polynomial/NTT_chrislaiisme.cpp"
}
#undef int
namespace walsh {
#include "../../codebook/7_Polynomial/Fast_Walsh_Transform.cpp"
}
namespace eq {
constexpr int MAXN = 8;
#include "../../codebook/6_Math/Fraction.cpp"
#include "../../codebook/6_Math/Simultaneous_Equations.cpp"
}
namespace gauss {
#include "../../codebook/6_Math/Gaussian_gcd.cpp"
}
namespace cut {
#include "../../codebook/4_Flow_Matching/SW-mincut.cpp"
}
namespace clique {
constexpr int N = 131;
#include "../../codebook/2_Graph/NumberofMaximalClique.cpp"
}
namespace matching {
#include "../../codebook/4_Flow_Matching/Maximum_Weight_Matching.cpp"
}
namespace tree {
constexpr int N = 64;
int par[N], dep[N], dfn[N];
int LCA(int a, int b) {
  while (dep[a] > dep[b]) a = par[a];
  while (dep[b] > dep[a]) b = par[b];
  while (a != b) a = par[a], b = par[b];
  return a;
}
#include "../../codebook/2_Graph/Virtual_Tree.cpp"
}
namespace io {
#include "../../codebook/1_Basic/bigIntIO.cpp"
}

void require(bool ok, const char *why) {
  if (!ok) { cerr << "FAIL round14 g09: " << why << '\n'; exit(1); }
}
mt19937_64 rng(0x140009);

ll power(ll a, ll e) { // Independent left-to-right modular exponent.
  ll r = 1;
  for (int bit = 62; bit >= 0; --bit) {
    r = r * r % nt::mod;
    if (e >> bit & 1) r = r * a % nt::mod;
  }
  return r;
}
void ntt_tests() {
  for (ll a : {0LL, 1LL, 3LL, nt::mod - 1})
    for (ll e : {0LL, 1LL, 2LL, LLONG_MAX})
      for (ll ret : {0LL, 1LL, nt::mod - 1})
        require(nt::pwr(a, e, ret) == power(a, e) * ret % nt::mod,
                "pwr accumulator and largest exponent");
  for (int n : {1, 2, 4, 8, 32, 64}) for (int pos = 0; pos < n; ++pos)
    for (bool inverse : {false, true}) {
      vector<ll> a(n); a[pos] = nt::mod - 1;
      nt::ntt(a, inverse);
      ll root = power(3, (nt::mod - 1) / n);
      if (inverse) root = power(root, nt::mod - 2);
      for (int j = 0; j < n; ++j) {
        ll want = (nt::mod - 1) * power(root, pos * j) % nt::mod;
        if (inverse) want = want * power(n, nt::mod - 2) % nt::mod;
        require(a[j] == want, "every signed impulse forward/inverse DFT");
      }
    }
  // Output lengths exactly below/at/above each padding threshold.
  for (int k = 1; k <= 10; ++k) for (int delta : {-1, 0, 1}) {
    int n = (1 << k) + delta;
    vector<ll> a(max(1, n - 1), nt::mod - 1);
    vector<ll> b(min(2, n), nt::mod - 1);
    auto c = nt::mult(a, b);
    require(SZ(c) == n, "convolution length at padding threshold");
    for (int i = 0; i < n; ++i)
      require(c[i] == (i == 0 || i == n - 1 ? 1 : 2),
              "convolution coefficients at padding threshold");
  }
}
void walsh_tests() {
  for (int L : {8, 0, 5, 1, 7, 0}) {
    int n = 1 << L; vector<int> storage(3 * n + 4, -777);
    int *a = storage.data() + 1, *b = storage.data() + n / 2 + 1;
    for (int i = 0; i < n + n / 2; ++i) a[i] = int(rng() % 7) - 3;
    vector<int> aa(a, a + n), bb(b, b + n), want(n);
    for (int s = 0; s < n; ++s)
      for (int t = s;; t = (t - 1) & s) {
        want[s] += aa[t] * bb[s ^ t]; if (!t) break;
      }
    // Both inputs overlap, and output partially overlaps both inputs.
    int *c = a + 1;
    walsh::subset_convolution(a, b, c, L);
    require(vector<int>(c, c + n) == want, "overlapping subset buffers");
    require(storage.front() == -777 && storage.back() == -777,
            "subset buffer guards");
    vector<int> z(n), transformed(n);
    for (int &v : z) v = int(rng() % 2000001) - 1000000;
    for (int s = 0; s < n; ++s)
      for (int t = s;; t = (t - 1) & s) {
        transformed[s] += z[t]; if (!t) break;
      }
    auto saved = z; walsh::fwt(z.data(), n, 1);
    require(z == transformed, "OR transform vs direct subset sum");
    walsh::fwt(z.data(), n, -1);
    require(z == saved, "OR inverse after cancellation");
  }
}

ll determinant(const vector<vector<ll>> &a) {
  int n = SZ(a); if (!n) return 1;
  vector<int> p(n); iota(ALL(p), 0); ll sum = 0;
  do {
    ll term = 1; int inversions = 0;
    for (int i = 0; i < n; ++i) {
      term *= a[i][p[i]];
      for (int j = 0; j < i; ++j) inversions += p[j] > p[i];
    }
    sum += inversions % 2 ? -term : term;
  } while (next_permutation(ALL(p)));
  return sum;
}
int minor_rank(const vector<vector<ll>> &a, int m) {
  int n = SZ(a), rank = 0;
  for (int rows = 1; rows < (1 << n); ++rows)
    for (int cols = 1; cols < (1 << m); ++cols) {
      int k = popcount(unsigned(rows));
      if (k <= rank || k != popcount(unsigned(cols))) continue;
      vector<vector<ll>> minor;
      for (int i = 0; i < n; ++i) if (rows >> i & 1) {
        minor.pb({});
        for (int j = 0; j < m; ++j)
          if (cols >> j & 1) minor.back().pb(a[i][j]);
      }
      if (determinant(minor)) rank = k;
    }
  return rank;
}
void equation_tests() {
  for (int tc = 0; tc < 2000; ++tc) {
    eq::matrix s; s.n = tc % 5, s.m = (tc / 5) % 5;
    vector<vector<ll>> a(s.n, vector<ll>(s.m + 1));
    for (int i = 0; i < s.n; ++i) for (int j = 0; j <= s.m; ++j) {
      int num = int(rng() % 5) - 2, den = 1 + rng() % 3;
      if (rng() & 1) num = -num, den = -den;
      s.M[i][j] = eq::fraction(num, den);
      a[i][j] = num * (6 / den); // Clear each individual denominator.
    }
    int rank = minor_rank(a, s.m);
    int want = rank == minor_rank(a, s.m + 1) ? rank : -1;
    fill_n(s.sol, eq::MAXN, eq::fraction(37, 5));
    for (int repeat = 0; repeat < 2; ++repeat) {
      require(s.solve() == want, "rational system rank via exact minors");
      if (want >= 0) {
        for (int i = 0; i < s.n; ++i) {
          eq::fraction total;
          for (int j = 0; j < s.m; ++j)
            total = total + eq::fraction(a[i][j]) * s.sol[j];
          require(total.n == a[i][s.m] * total.d,
                  "rational solution satisfies original rows");
        }
        int piv = 0;
        for (int j = 0; j < s.m; ++j) {
          int next = minor_rank(a, j + 1);
          if (next == piv) require(s.sol[j].n == 0, "free variable zero");
          piv = next;
        }
      }
      for (int j = want < 0 ? 0 : s.m; j < eq::MAXN; ++j)
        require(s.sol[j].n == 37 && s.sol[j].d == 5,
                "inconsistency/unused solution preservation");
    }
  }
}

using wide = __int128_t;
using Z = complex<wide>;
wide norm2(Z z) { return z.real() * z.real() + z.imag() * z.imag(); }
Z gaussian_oracle(Z a, Z b) {
  while (b != Z{}) {
    wide norm = norm2(b); Z num = a * conj(b);
    auto nearest = [&](wide x) {
      wide q = x / norm, rem = x % norm;
      if (2 * (rem < 0 ? -rem : rem) >= norm)
        q += rem < 0 ? -1 : 1;
      return q;
    };
    Z rem = a - Z(nearest(num.real()), nearest(num.imag())) * b;
    a = b, b = rem;
  }
  return a;
}
void gaussian_tests() {
  for (ll scale : {1LL, 1000000LL, 500000000LL})
    for (int x = -5; x <= 5; x += 2)
      for (int y = -3; y <= 3; y += 2)
        for (int rot = 0; rot < 4; ++rot) {
          complex<ll> a(x * scale, y * scale), b(2 * scale, 0);
          for (int i = 0; i < rot; ++i) a *= complex<ll>(0, 1);
          auto got = gauss::gaussian_gcd(a, b);
          Z az(a.real(), a.imag()), bz(b.real(), b.imag());
          Z gz(got.real(), got.imag());
          require(norm2(gz) == norm2(gaussian_oracle(az, bz)),
                  "large half-integer Gaussian quotients");
          for (Z z : {az, bz}) {
            Z num = z * conj(gz);
            require(num.real() % norm2(gz) == 0 &&
                    num.imag() % norm2(gz) == 0, "Gaussian divisibility");
          }
        }
}

pair<ll, ll> cut_oracle(const vector<vector<int>> &w) {
  int n = SZ(w); if (n <= 1) return {0, 0};
  ll best = LLONG_MAX, largest = 0;
  for (int mask = 1; mask < (1 << n) - 1; ++mask) {
    ll value = 0;
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      if (((mask >> u) ^ (mask >> v)) & 1) value += w[u][v];
    best = min(best, value), largest = max(largest, value);
  }
  return {best, largest};
}
void cut_tests() {
  for (int tc = 0; tc < 800; ++tc) {
    int n = tc % 10; vector<vector<int>> w(n, vector<int>(n));
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      w[u][v] = w[v][u] = rng() % 5;
    ll largest = cut_oracle(w).second;
    int scale = largest ? INT_MAX / largest : 1;
    cut::sw.init(n);
    for (int u = 0; u < n; ++u) {
      cut::sw.addEdge(u, u, INT_MAX); // Ignored, cannot overflow.
      for (int v = u + 1; v < n; ++v) {
        int value = w[u][v] * scale, first = value / 3;
        w[u][v] = w[v][u] = value;
        cut::sw.addEdge(u, v, first);
        cut::sw.addEdge(v, u, value - first);
      }
    }
    require(cut::sw.solve(n) == cut_oracle(w).first,
            "mincut with largest cut near INT_MAX and parallel edges");
  }
}

void clique_tests() {
  clique::BronKerbosch bk;
  const int label[] = {1, 63, 64, 65, 127, 128, 130};
  for (int tc = 0; tc < 1200; ++tc) {
    int n = tc % 8; bk.init(130);
    vector<int> adj(n), P, X; int pm = 0, xm = 0;
    for (int u = 0; u < n; ++u) {
      int type = rng() % 3;
      if (type == 1) pm |= 1 << u, P.pb(label[u]);
      if (type == 2) xm |= 1 << u, X.pb(label[u]);
      for (int v = 0; v < u; ++v) if (rng() & 1) {
        adj[u] |= 1 << v, adj[v] |= 1 << u;
        bk.add_edge(label[u], label[v]);
      }
    }
    int want = 0;
    for (int mask = 0; mask < (1 << n); ++mask) {
      if (mask & ~pm) continue;
      bool ok = true;
      for (int u = 0; u < n; ++u) {
        if (mask >> u & 1) ok &= (adj[u] & mask) == (mask ^ (1 << u));
        else if ((pm | xm) >> u & 1) ok &= (adj[u] & mask) != mask;
      }
      want += ok;
    }
    bk.S = INT_MAX - want; bk.dfs_vec(P, X);
    require(bk.S == INT_MAX, "scattered P/X count at INT_MAX");
  }
}

ll matching_oracle(const vector<vector<int>> &w) {
  int n = SZ(w); vector<ll> dp(1 << n);
  for (int mask = 1; mask < (1 << n); ++mask) {
    int u = countr_zero(unsigned(mask)), rest = mask ^ (1 << u);
    dp[mask] = dp[rest];
    for (int v = u + 1; v < n; ++v) if (rest >> v & 1)
      dp[mask] = max(dp[mask], dp[rest ^ (1 << v)] + w[u][v]);
  }
  return dp.back();
}
void matching_tests() {
  const int vals[] = {-10000000, 0, 1, 9999999, 10000000};
  for (int tc = 0; tc < 160; ++tc) {
    int n = tc % 13; matching::WeightGraph g(n);
    vector<vector<int>> w(n, vector<int>(n));
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v) {
      int x = vals[rng() % 5];
      w[u][v] = w[v][u] = x; g.add_edge(u + 1, v + 1, x);
    }
    // Preserve all other edges/old contracted snapshots between solves.
    for (int stage = 0; stage < 40; ++stage) {
      if (n > 1) {
        int u = rng() % n, v = rng() % (n - 1); v += v >= u;
        int x = vals[rng() % 5];
        w[u][v] = w[v][u] = x; g.add_edge(v + 1, u + 1, x);
      }
      ll want = matching_oracle(w);
      for (int repeat = 0; repeat < 2; ++repeat) {
        auto got = g.solve(); ll total = 0; int pairs = 0;
        for (int u = 1; u <= n; ++u) if (int v = g.match[u]) {
          require(v != u && 1 <= v && v <= n && g.match[v] == u &&
                  w[u - 1][v - 1] > 0, "valid symmetric matching");
          if (u < v) total += w[u - 1][v - 1], ++pairs;
        }
        require(got.first == want && got.first == total &&
                got.second == pairs, "single-edge mutations vs subset DP");
      }
    }
  }
}

void tree_tests() {
  using namespace tree;
  for (int tc = 0; tc < 500; ++tc) {
    int n = 1 + rng() % N;
    vector<int> label(n); iota(ALL(label), 0); shuffle(ALL(label), rng);
    vector<vector<int>> children(n);
    for (int i = 1; i < n; ++i)
      children[label[rng() % i]].pb(label[i]);
    int timer = 0;
    auto dfs = [&](auto &&self, int u, int p) -> void {
      par[u] = p; dep[u] = u == p ? 0 : dep[p] + 1; dfn[u] = timer++;
      shuffle(ALL(children[u]), rng);
      for (int v : children[u]) self(self, v, u);
    };
    dfs(dfs, label[0], label[0]);
    for (int pass = 0; pass < 5; ++pass) {
      vector<int> keys;
      for (int i = 0; i < n; ++i) if (rng() % 5 == 0)
        for (int copies = 1 + rng() % 20; copies; --copies) keys.pb(i);
      if (!pass) keys.clear();
      shuffle(ALL(keys), rng); set<int> nodes(ALL(keys));
      vector<int> unique_nodes(ALL(nodes));
      for (int a : unique_nodes) for (int b : unique_nodes)
        nodes.insert(LCA(a, b));
      set<pii> want, got; int expected_root = -1;
      for (int u : nodes) {
        int p = par[u];
        while (p != par[p] && !nodes.count(p)) p = par[p];
        if (u != p && nodes.count(p)) want.insert({p, u});
        else expected_root = u;
      }
      int root = build(keys);
      require(root == expected_root, "virtual root with permuted IDs");
      require(is_sorted(ALL(keys), [](int a, int b) {
        return dfn[a] < dfn[b];
      }) && adjacent_find(ALL(keys)) == keys.end(), "dedup/order keys");
      for (int u = 0; u < N; ++u) for (int v : vG[u])
        require(got.insert({u, v}).second, "no duplicate virtual edges");
      require(got == want, "nearest included ancestor edges");
      if (root != -1) reset(root);
      for (auto &row : vG)
        require(row.empty(), "reset before root change");
    }
  }
}

void io_tests() {
  FILE *f = tmpfile(); require(f, "tmpfile");
  string input = string(1000000, '0') + "-42 " +
    "170141183460469231731687303715884105728" +
    string(1000000, '0') + " -7 -- -- ";
  require(fwrite(input.data(), 1, input.size(), f) == input.size(),
          "write long scanner runs");
  fflush(f), rewind(f);
  int original = dup(STDIN_FILENO);
  require(original >= 0 && dup2(fileno(f), STDIN_FILENO) >= 0,
          "redirect stdin"); clearerr(stdin);
  __int128 x = 99;
  require(io::read(x) && x == 0, "million leading zeros");
  require(io::read(x) && x == 42, "terminating minus consumed");
  require(!io::read(x) && x == 42, "sticky overflow across million zeros");
  require(io::read(x) && x == -7, "recover after entire overflow run");
  for (int i = 0; i < 3; ++i)
    require(!io::read(x) && x == -7, "junk-only tail and repeated EOF");
  require(dup2(original, STDIN_FILENO) >= 0, "restore stdin");
  clearerr(stdin); close(original); fclose(f);
}

int main() {
  ntt_tests(); walsh_tests(); equation_tests(); gaussian_tests();
  cut_tests(); clique_tests(); matching_tests(); tree_tests(); io_tests();
  cerr << "PASS round14 g09: all nine assigned templates\n";
}
