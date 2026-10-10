#include "../../codebook/1_Basic/Default_code.cpp"
#include <unistd.h>
#define BLACK_MAGIC_NO_MAIN
#include "../../codebook/1_Basic/black_magic.cpp"
#undef BLACK_MAGIC_NO_MAIN
namespace io {
#include "../../codebook/1_Basic/bigIntIO.cpp"
}
namespace eq {
constexpr int MAXN = 8;
#include "../../codebook/6_Math/Fraction.cpp"
#include "../../codebook/6_Math/Simultaneous_Equations.cpp"
}
namespace congruence {
#include "../../codebook/6_Math/ax+by=gcd.cpp"
#include "../../codebook/6_Math/Linear_Congruence.cpp"
}
namespace bipartite {
constexpr int N = 32;
#include "../../codebook/4_Flow_Matching/Bipartite_Matching.cpp"
}
namespace blossom {
#include "../../codebook/4_Flow_Matching/Maximum_Simple_Graph_Matching.cpp"
}
struct F5 {
  int v;
  F5(int x = 0) : v((x % 5 + 5) % 5) {}
  F5 operator-() const { return -v; }
  F5 &operator+=(F5 b) { v = (v + b.v) % 5; return *this; }
  friend F5 operator*(F5 a, F5 b) { return a.v * b.v; }
  friend F5 operator/(F5 a, F5 b) {
    assert(b.v); return a * F5(b.v == 2 ? 3 : b.v == 3 ? 2 : b.v);
  }
  friend bool operator==(F5 a, F5 b) { return a.v == b.v; }
};
#include "../../codebook/6_Math/Berlekamp-Massey.cpp"
namespace values {
struct mint {
  int v;
  mint(int x = 0) : v((x % 7 + 7) % 7) {}
  mint &operator+=(mint b) { v = (v + b.v) % 7; return *this; }
  friend mint operator+(mint a, mint b) { return a += b; }
  friend mint operator-(mint a, mint b) { return a.v - b.v; }
  friend mint operator*(mint a, mint b) { return a.v * b.v; }
  mint &operator*=(mint b) { return *this = *this * b; }
  friend bool operator==(mint a, mint b) { return a.v == b.v; }
};
vector<mint> ifac, inegfac;
#include "../../codebook/7_Polynomial/Value_Poly.cpp"
void tables(int n) {
  ifac.assign(n, 1); inegfac.assign(n, 1); mint fac = 1;
  for (int i = 0; i < n; ++i) {
    if (i) fac *= i;
    int inv = 1; while ((fac * inv).v != 1) ++inv;
    ifac[i] = inv; inegfac[i] = i & 1 ? -inv : inv;
  }
}
void audit() {
  // Every impulse basis over F7; tables have exactly the required length.
  for (int n = 1; n <= 7; ++n) for (int b = 0; b < 7; ++b)
    for (int pulse = 0; pulse < n; ++pulse) {
      Poly f(b); f.poly.assign(n, 0); f.poly[pulse] = 1; tables(n);
      vector<mint> want(7);
      for (int t = 0; t < 7; ++t) {
        mint term = 1;
        for (int j = 0; j < n; ++j) if (j != pulse) {
          mint d = pulse - j; int inv = 1;
          while ((d * inv).v != 1) ++inv;
          term *= mint(t - j) * inv;
        }
        want[t] = term;
      }
      for (;;) {
        for (int t = 0; t < 7; ++t) assert(f.get_val(b + t) == want[t]);
        if (SZ(f.poly) == 7) break;
        f.raise(); tables(SZ(f.poly));
        for (int t = 1; t < 7; ++t) want[t] += want[t - 1];
      }
    }
}
}
namespace circles {
const double eps = 1e-9, pi = acos(-1.);
double sq(double x) { return x * x; }
int sign(double x) { return x < -eps ? -1 : x > eps; }
#include "../../codebook/8_Geometry/_basic.cpp"
struct C { P c; double r; };
#include "../../codebook/8_Geometry/circleOrArea.cpp"
void audit() {
  assert(CircleUnionArea({}) == 0);
  mt19937 rng(1703);
  // Only adjacent disks overlap; triple intersections have zero area.
  for (int n = 1; n <= 8; ++n)
    for (double d : {1., 1.25, 1.5, 2., 2.25}) {
      double lens = d >= 2 ? 0 : 2 * acos(d / 2) - d * sqrt(4-d*d) / 2;
      double want = n * pi - (n - 1) * lens;
      for (int k = 0; k < 12; ++k) {
        vector<C> c;
        for (int i = 0; i < n; ++i) {
          P p = P(i * d, 0).rot(k * pi / 6) + P(1e5, -1e5);
          c.pb({p, 1}); c.pb({p, .25}); c.pb({p, 0});
        }
        c.pb({{2e5, 2e5}, 0}); shuffle(ALL(c), rng);
        assert(abs(CircleUnionArea(c) - want) < 1e-8 * max(1., want));
      }
    }
}
}

void io_audit() {
  vector<pair<bool, __int128>> cases; string input;
  auto digits = [](__uint128_t x) {
    string s; do { s += char('0' + x % 10); x /= 10; } while (x);
    reverse(ALL(s)); return s;
  };
  for (int bit = 0; bit <= 127; ++bit) for (int delta : {-1, 0, 1}) {
    __uint128_t x = (__uint128_t(1) << bit) + delta;
    for (bool negative : {false, true}) {
      input += (negative ? "-" : "+") + string(50, '0') + digits(x) + " ";
      bool ok = x <= (__uint128_t(1) << 127) - !negative;
      __int128 v = 0;
      if (ok) v = negative ? -(__int128)(x ? x - 1 : 0) - bool(x) : x;
      cases.pb({ok, v});
    }
  }
  // A failed token must consume its whole digit run, then recover at EOF.
  input += string(400, '9') + ";-42"; cases.pb({false, 0});
  cases.pb({true, -42});
  FILE *f = tmpfile(); assert(f);
  assert(fwrite(input.data(), 1, input.size(), f) == input.size());
  rewind(f); int original = dup(STDIN_FILENO);
  assert(original >= 0 && dup2(fileno(f), STDIN_FILENO) >= 0);
  clearerr(stdin);
  for (auto [ok, expected] : cases) {
    __int128 x = 917; assert(io::read(x) == ok);
    assert(x == (ok ? expected : 917));
  }
  __int128 x = 917; assert(!io::read(x) && x == 917);
  assert(dup2(original, STDIN_FILENO) >= 0); clearerr(stdin);
  close(original); fclose(f);
}
void black_audit() {
  // Binary ropes, empty edits and mutation of both persistent branches.
  vector<rope<char>> snapshots(1); vector<string> want(1);
  mt19937 rng(1704);
  for (int t = 0; t < 500; ++t) {
    int source = rng() % SZ(snapshots);
    snapshots.pb(snapshots[source]); want.pb(want[source]);
    auto &r = snapshots.back(); auto &s = want.back();
    int pos = rng() % (s.size() + 1);
    if (t & 1) {
      string add{char(0), char(255), char(rng() % 256)};
      r.insert(pos, add.data(), add.size()); s.insert(pos, add);
    } else {
      int len = rng() % (s.size() - pos + 1);
      r.erase(pos, len); s.erase(pos, len);
    }
    for (int i = 0; i < SZ(want); ++i)
      assert(string(snapshots[i].begin(), snapshots[i].end()) == want[i]);
  }
  tree_set s; for (int x : {INT_MIN, INT_MAX, INT_MIN}) s.insert(x);
  assert(s.size() == 2 && s.order_of_key(INT_MIN) == 0);
  assert(s.order_of_key(INT_MAX) == 1 && s.find_by_order(2) == s.end());
  heap a, b;
  for (int i = 0; i < 1000; ++i) {
    b.push(i & 1 ? INT_MIN : INT_MAX); a.join(b); assert(b.empty());
    swap(a, b); b.join(a); assert(a.empty()); swap(a, b);
  }
  for (int i = 0; i < 1000; ++i) {
    assert(a.top() == (i < 500 ? INT_MAX : INT_MIN)); a.pop();
  }
  assert(a.empty());
}
void equation_audit() {
  eq::matrix a;
  for (int n = 0; n <= eq::MAXN; ++n)
    for (int m = 0; m <= eq::MAXN; ++m)
      for (bool denominator : {false, true}) {
      a.n = n; a.m = m; int rank = min(n, m);
      fill_n(a.sol, eq::MAXN, eq::fraction(37));
      for (int i = 0; i < n; ++i) fill_n(a.M[i], m + 1, eq::fraction());
      // Reverse pivot rows; individual arithmetic intermediates fit ll.
      for (int i = 0; i < rank; ++i) {
        auto f = denominator ? eq::fraction(1, LLONG_MAX - i)
                             : eq::fraction(LLONG_MAX - i);
        a.M[rank - i - 1][i] = a.M[rank - i - 1][m] = f;
      }
      assert(a.solve() == rank && a.solve() == rank);
      for (int j = 0; j < eq::MAXN; ++j)
        assert(a.sol[j].n == (j < rank ? 1 : j < m ? 0 : 37));
      if (rank < n) {
        a.M[n - 1][m] = eq::fraction(1, LLONG_MAX);
        vector<eq::fraction> saved(a.sol, a.sol + eq::MAXN);
        assert(a.solve() == -1);
        for (int j = 0; j < eq::MAXN; ++j)
          assert(a.sol[j].n == saved[j].n && a.sol[j].d == saved[j].d);
      }
    }
}
void congruence_audit() {
  // Large moduli with multiple solutions whose final element is n-1.
  for (ll g = 1; g <= 256; ++g) {
    ll n = LLONG_MAX / g * g, step = n / g;
    auto a = congruence::mod_leq(-g, g, n);
    assert(a.size() == size_t(g));
    for (ll i = 0; i < g; ++i) assert(a[i] == step - 1 + i * step);
    auto b = congruence::mod_leq(g, -g, n); assert(a == b);
    if (g > 1) assert(congruence::mod_leq(-g, 1, n).empty());
  }
}
int cover_bound(const vector<unsigned> &g, int r) {
  int best = SZ(g);
  for (unsigned s = 0; s < (1u << SZ(g)); ++s) {
    unsigned neighbors = 0;
    for (int u = 0; u < SZ(g); ++u) if (s >> u & 1) neighbors |= g[u];
    best = min(best, SZ(g) - popcount(s) + popcount(neighbors));
  }
  assert(best <= r); return best;
}
void bipartite_audit() {
  bipartite::Bipartite_Matching b; mt19937 rng(1705);
  for (int tc = 0; tc < 1000; ++tc) {
    int small = rng() % 10, large = rng() % 33;
    vector<unsigned> g(small);
    for (auto &mask : g) for (int v = 0; v < large; ++v)
      if (rng() % 3 == 0) mask |= 1u << v;
    int want = cover_bound(g, large);
    for (bool transpose : {false, true}) {
      int l = transpose ? large : small, r = transpose ? small : large;
      b.init(l, r);
      for (int u = small - 1; u >= 0; --u) for (int v = 0; v < large; ++v)
        if (g[u] >> v & 1) {
          int x = transpose ? v : u, y = transpose ? u : v;
          b.add_edge(x, y); if (tc & 1) b.add_edge(x, y);
        }
      for (int repeat = 0; repeat < 2; ++repeat) {
        assert(b.matching() == want); int count = 0;
        for (int u = 0; u < l; ++u) if (b.mp[u] != -1) {
          int v = b.mp[u]; assert(v >= 0 && v < r && b.mq[v] == u);
          assert(transpose ? (g[v] >> u & 1) : (g[u] >> v & 1)); ++count;
        }
        assert(count == want);
        for (int v = 0; v < r; ++v)
          assert(b.mq[v] == l || b.mp[b.mq[v]] == v);
      }
    }
  }
}
void blossom_audit() {
  // All 32768 labeled six-vertex graphs, starting from a valid seed.
  for (int code = 0; code < (1 << 15); ++code) {
    blossom::Matching m(6); int adj[6] = {}, bit = 0, seeded = 0;
    for (int u = 0; u < 6; ++u) for (int v = u + 1; v < 6; ++v, ++bit)
      if (code >> bit & 1) {
        adj[u] |= 1 << v; adj[v] |= 1 << u; m.add_edge(u, v);
        if ((u + v + code) % 3 == 0) m.add_edge(v, u);
        if (m.match[u] == 6 && m.match[v] == 6 && (bit + code) % 2) {
          m.match[u] = v; m.match[v] = u; ++seeded;
        }
      }
    int dp[64] = {};
    for (int mask = 1; mask < 64; ++mask) {
      int u = countr_zero(unsigned(mask)), rest = mask ^ (1 << u);
      dp[mask] = dp[rest];
      for (int v = u + 1; v < 6; ++v)
        if ((rest >> v & 1) && (adj[u] >> v & 1))
          dp[mask] = max(dp[mask], 1 + dp[rest ^ (1 << v)]);
    }
    assert(m.solve() == dp[63] - seeded && m.solve() == 0);
    int pairs = 0;
    for (int u = 0; u < 6; ++u) if (int v = m.match[u]; v != 6) {
      assert(v != u && (adj[u] >> v & 1) && m.match[v] == u);
      pairs += u < v;
    }
    assert(pairs == dp[63]);
  }
}
bool recurrence_possible(const vector<F5> &a, int k) {
  vector<vector<F5>> m(SZ(a) - k, vector<F5>(k + 1));
  for (int i = k; i < SZ(a); ++i) {
    for (int j = 0; j < k; ++j) m[i-k][j] = a[i-j-1];
    m[i-k][k] = a[i];
  }
  int rank = 0;
  for (int col = 0; col < k && rank < SZ(m); ++col) {
    int row = rank; while (row < SZ(m) && m[row][col] == 0) ++row;
    if (row == SZ(m)) continue;
    swap(m[row], m[rank]); F5 inv = F5(1) / m[rank][col];
    for (int j = col; j <= k; ++j) m[rank][j] = m[rank][j] * inv;
    for (int i = rank + 1; i < SZ(m); ++i) {
      F5 factor = -m[i][col];
      for (int j = col; j <= k; ++j) m[i][j] += factor * m[rank][j];
    }
    ++rank;
  }
  for (int i = rank; i < SZ(m); ++i) if (!(m[i][k] == 0)) return false;
  return true;
}
void bm_audit() {
  for (int n = 0, total = 1; n <= 7; ++n, total *= 5)
    for (int code = 0; code < total; ++code) {
      vector<F5> a(n); int x = code;
      for (auto &v : a) v = x % 5, x /= 5;
      int order = 0; while (!recurrence_possible(a, order)) ++order;
      auto c = BerlekampMassey(a); assert(SZ(c) == order);
      for (int i = order; i < n; ++i) {
        F5 sum;
        for (int j = 0; j < order; ++j) sum += c[j] * a[i-j-1];
        assert(sum == a[i]);
      }
    }
}
int main() {
  io_audit(); black_audit(); values::audit(); equation_audit();
  congruence_audit(); bipartite_audit(); blossom_audit(); bm_audit();
  circles::audit(); cout << "PASS round17 g03: nine templates\n";
}
