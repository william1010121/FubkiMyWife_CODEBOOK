#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
#define X first
#define Y second
#define SZ(a) int((a).size())
#define ALL(a) (a).begin(), (a).end()
#define pb push_back
namespace bip {
constexpr int N = 32;
#include "../../codebook/4_Flow_Matching/Bipartite_Matching.cpp"
}
namespace blossom {
#include "../../codebook/4_Flow_Matching/Maximum_Simple_Graph_Matching.cpp"
}
namespace cent {
constexpr int N = 33;
#include "../../codebook/3_Data_Structure/Centroid_Decomposition.cpp"
}
#include "../../codebook/5_String/PalTree.cpp"
namespace poly {
struct mint {
  static constexpr int P = 17;
  int v;
  mint(ll x = 0) : v((x % P + P) % P) {}
  mint &operator+=(mint b) { v = (v + b.v) % P; return *this; }
  friend mint operator+(mint a, mint b) { return a += b; }
  friend mint operator-(mint a, mint b) { return a.v - b.v; }
  friend mint operator*(mint a, mint b) { return a.v * b.v; }
  mint &operator*=(mint b) { return *this = *this * b; }
  friend bool operator==(mint a, mint b) { return a.v == b.v; }
};
vector<mint> ifac, inegfac;
#include "../../codebook/7_Polynomial/Value_Poly.cpp"
}
namespace geo {
constexpr double eps = 1e-9;
const double pi = acos(-1.);
int sign(double x) { return (x > eps) - (x < -eps); }
#include "../../codebook/8_Geometry/_basic.cpp"
struct C { P c; double r; };
#include "../../codebook/8_Geometry/CircleTangent.cpp"
struct Point { double x, y; };
#include "../../codebook/8_Geometry/minDistOfTwoConvex.cpp"
}
void require(bool ok, const char *s) {
  if (!ok) { cerr << "FAIL " << s << '\n'; exit(1); }
}
void near(double x, double y, double tol = 1e-8) {
  require(isfinite(x) && abs(x - y) <= tol, "geometry value");
}
int matching_oracle(const vector<int> &g) {
  int n = SZ(g); vector<int> dp(1 << n);
  for (int mask = 1; mask < (1 << n); ++mask) {
    int u = countr_zero(unsigned(mask)), rest = mask ^ (1 << u);
    dp[mask] = dp[rest];
    for (int vs = g[u] & rest; vs; vs &= vs - 1) {
      int v = countr_zero(unsigned(vs));
      dp[mask] = max(dp[mask], 1 + dp[rest ^ (1 << v)]);
    }
  }
  return dp.back();
}
void matching_audit() {
  mt19937 rng(1401); bip::Bipartite_Matching b;
  // Seed an arbitrary valid matching, then insert edges in small batches.
  // Each batch must augment it, including after unsuccessful searches.
  for (int tc = 0; tc < 1200; ++tc) {
    int n = tc % 13; blossom::Matching m(n); vector<int> g(n);
    vector<pii> edges;
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      if (rng() % 5 < 1 + tc % 5) edges.pb({u, v});
    shuffle(ALL(edges), rng);
    int total = 0;
    for (auto [u, v] : edges) {
      m.add_edge(u, v); g[u] |= 1 << v; g[v] |= 1 << u;
      if (m.match[u] == n && m.match[v] == n && rng() % 2)
        m.match[u] = v, m.match[v] = u, ++total;
    }
    int want = matching_oracle(g);
    total += m.solve(); require(total == want, "seeded blossom");
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v) {
      m.add_edge(u, v); m.add_edge(v, u);
      g[u] |= 1 << v; g[v] |= 1 << u;
      total += m.solve(); want = matching_oracle(g);
      require(total == want && m.solve() == 0, "incremental blossom");
      int pairs = 0;
      for (int x = 0; x < n; ++x) if (m.match[x] != n) {
        int y = m.match[x];
        require(y >= 0 && y < n && m.match[y] == x &&
                (g[x] >> y & 1), "blossom reciprocal edge");
        pairs += x < y;
      }
      require(pairs == total, "blossom pair count");
    }
  }
  // Hall-deficient rectangular components at exact array capacity.
  for (int l : {0, 1, 2, 31, 32}) for (int r : {0, 1, 2, 31, 32})
    for (int k = 0; k <= r; ++k) {
      b.init(l, r);
      for (int u = 0; u < l; ++u) for (int v = 0; v < k; ++v)
        b.add_edge(u, v), b.add_edge(u, v);
      for (int repeat = 0; repeat < 2; ++repeat) {
        require(b.matching() == min(l, k), "Hall deficiency");
        for (int u = 0; u < l; ++u) if (b.mp[u] >= 0)
          require(b.mq[b.mp[u]] == u, "bipartite reciprocal edge");
        for (int v = 0; v < r; ++v) if (b.mq[v] != l)
          require(b.mp[b.mq[v]] == v, "bipartite inverse right");
      }
    }
}
void centroid_audit() {
  cent::Cent_Dec c;
  // All labeled trees through n=6, with full-width signed edge weights.
  for (int n = 1; n <= 6; ++n) {
    int total = 1;
    for (int i = 0; i < n - 2; ++i) total *= n;
    for (int code = 0; code < total; ++code) {
      c.init(n); vector<int> seq(max(0, n - 2)), degree(n + 1, 1);
      int z = code;
      for (int &v : seq) v = z % n + 1, z /= n, ++degree[v];
      vector<pii> es;
      for (int v : seq) {
        int u = 1; while (degree[u] != 1) ++u;
        es.pb({u, v}); --degree[u]; --degree[v];
      }
      if (n > 1) {
        vector<int> leaves;
        for (int u = 1; u <= n; ++u) if (degree[u] == 1) leaves.pb(u);
        es.pb({leaves[0], leaves[1]});
      }
      vector<vector<pii>> g(n + 1);
      for (int i = 0; i < SZ(es); ++i) {
        auto [u, v] = es[i];
        int w = (i + code) % 3 == 0 ? INT_MIN :
                (i + code) % 3 == 1 ? INT_MAX : 0;
        c.add_edge(u, v, w); g[u].pb({v, w}); g[v].pb({u, w});
      }
      c.build(); vector<vector<ll>> d(n + 1, vector<ll>(n + 1));
      for (int s = 1; s <= n; ++s) {
        auto walk = [&](auto &&self, int u, int p) -> void {
          for (auto [v, w] : g[u]) if (v != p)
            d[s][v] = d[s][u] + w, self(self, v, u);
        };
        walk(walk, s, 0);
        require(c.query(s) == 0 && !c.done[s], "centroid clean state");
      }
      vector<int> copies(n + 1);
      for (int mark = 1; mark <= 2 * n; ++mark) {
        int v = 1 + (mark + code) % n; c.modify(v); ++copies[v];
        for (int u = 1; u <= n; ++u) {
          ll want = 0;
          for (int x = 1; x <= n; ++x) want += copies[x] * d[u][x];
          require(c.query(u) == want, "Pruefer signed distance sums");
        }
      }
    }
  }
}
void pal_audit() {
  palindromic_tree p;
  // Exhaust binary words through length 14; reuse after every count().
  for (int n = 0; n <= 14; ++n) for (int mask = 0; mask < (1 << n);
       ++mask) {
    string s(n, 'a'); p.clear(); map<string, int> want;
    vector<string> label(2);
    for (int i = 0; i < n; ++i) {
      s[i] = (mask >> i & 1) ? 'z' : 'a'; p.add(s[i]);
      int num = 0, longest = 0;
      for (int j = 0; j <= i; ++j) {
        bool ok = true;
        for (int l = j, r = i; l < r; ++l, --r) ok &= s[l] == s[r];
        if (ok) ++want[s.substr(j, i - j + 1)], ++num,
                longest = max(longest, i - j + 1);
      }
      label.resize(p.St.size());
      label[p.last] = s.substr(i + 1 - longest, longest);
      require(p.St[p.last].num == num && p.St[p.last].len == longest,
              "eertree prefix suffixes");
    }
    p.count(); require(p.size() == SZ(want), "eertree distinct");
    for (int v = 2; v < SZ(p.St); ++v)
      require(p.St[v].cnt == want.at(label[v]), "eertree occurrences");
    require(p.St[0].cnt == n && p.St[1].cnt == n, "eertree roots");
  }
}
void arithmetic_audit() {
  for (int mod : {2, 3, 17, 257, 65537, 2147483647}) {
    int N = min(mod - 1, 131072); vector<int> inv(N + 2, -123);
#include "../../codebook/6_Math/Mod_Inverse.cpp"
    for (int i = 1; i <= N; ++i)
      require(inv[i] > 0 && inv[i] < mod && ll(inv[i]) * i % mod == 1,
              "inverse full-width product");
    require(inv[0] == -123 && inv[N + 1] == -123, "inverse bounds");
  }
  using namespace poly;
  auto inverse = [](int a) {
    for (int b = 1; b < mint::P; ++b) if (a * b % mint::P == 1) return b;
    abort();
  };
  auto tables = [&](int n) {
    ifac.assign(n, 1); inegfac.assign(n, 1); int fac = 1;
    for (int i = 0; i < n; ++i) {
      if (i) fac = fac * i % mint::P;
      ifac[i] = inverse(fac); inegfac[i] = (i & 1) ? -ifac[i].v : ifac[i];
    }
  };
  auto eval = [&](const Poly &p, mint x) {
    mint ans = 0;
    for (int i = 0; i < SZ(p.poly); ++i) {
      mint term = p.poly[i];
      for (int j = 0; j < SZ(p.poly); ++j) if (i != j)
        term *= (x - (p.base + j)) * inverse(mint(i - j).v);
      ans += term;
    }
    return ans;
  };
  mt19937 rng(1402);
  for (int tc = 0; tc < 500; ++tc) {
    Poly p(tc % mint::P); p.poly.resize(1 + tc % mint::P);
    for (auto &x : p.poly) x = rng() % mint::P;
    for (int step = 0; step < 4; ++step) {
      tables(SZ(p.poly));
      for (int x = 0; x < mint::P; ++x)
        require(p.get_val(x) == eval(p, x), "mutable polynomial samples");
      if (SZ(p.poly) == mint::P) break;
      vector<mint> want(mint::P);
      for (int t = 0; t < mint::P; ++t)
        want[t] = (t ? want[t - 1] : mint(0)) + eval(p, p.base + t);
      p.raise(); tables(SZ(p.poly));
      for (int t = 0; t < mint::P; ++t)
        require(p.get_val(p.base + t) == want[t], "wrapped lift");
      p.base = rng() % mint::P;
      p.poly[rng() % p.poly.size()] = rng() % mint::P;
    }
  }
}
void geometry_audit() {
  using namespace geo;
  // Dyadic points give exact orientation signs, including point segments.
  vector<P> grid;
  for (int x = -1; x <= 1; ++x) for (int y = -1; y <= 1; ++y)
    grid.pb({x / 2., y / 2.});
  auto on = [](P a, P b, P p) {
    return ((b - a) ^ (p - a)) == 0 && (p - a) * (p - b) <= 0;
  };
  for (P a : grid) for (P b : grid) for (P c : grid) for (P d : grid) {
    double x = (b - a) ^ (c - a), y = (b - a) ^ (d - a);
    double z = (d - c) ^ (a - c), w = (d - c) ^ (b - c);
    bool want = on(a, b, c) || on(a, b, d) || on(c, d, a) ||
                on(c, d, b) || (x * y < 0 && z * w < 0);
    require(SegmentIntersect(a, b, c, d) == want, "dyadic segments");
    require(L(a, b).inside(c) == on(a, b, c), "line membership");
    if (((b - a) ^ (d - c)) != 0) {
      P p = Intersect(L(a, b), L(c, d));
      near((b - a) ^ (p - a), 0); near((d - c) ^ (p - c), 0);
    }
    Point u{a.x, a.y}, v{b.x, b.y}, s{c.x, c.y}, t{d.x, d.y};
    require(_segments_intersect(u, v, s, t) == want, "convex helper");
  }
  for (P a : grid) for (P b : grid) if ((b - a).abs())
    for (P p : grid) {
      L l(a, b); P q = l.project(p), r = l.reflect(l.reflect(p));
      near((q - p) * (b - a), 0); near(r.x, p.x); near(r.y, p.y);
      near((a + (b - a) * l.get_ratio(p) - q).abs(), 0);
    }
  // Contacts are exactly represented; proximity cases use existing eps
  // policy. Tiny positive circles retain the external-contact priority.
  for (double radius : {eps / 16, 0.125, 1., 1024.}) {
    C a{{0, 0}, radius};
    require(tangent(a, a).empty(), "coincident circle convention");
    require(tangent(a, P{0, 0}).empty(), "point at circle center");
    require(tangent(a, P{radius, 0}).size() == 1, "point contact");
    if (radius < eps) continue;
    for (int ratio : {1, 2, 3}) {
      C b{{radius * (ratio + 1), 0}, ratio * radius};
      for (auto pair : {std::pair{a, b}, std::pair{b, a}}) {
        auto lines = tangent(pair.first, pair.second);
        require(lines.size() == 3, "external circle contact");
        for (L l : lines) {
          P u = l.pb - l.pa; require(u.abs() > 0, "tangent direction");
          near(abs(u ^ (a.c - l.pa)) / u.abs(), a.r);
          near(abs(u ^ (b.c - l.pa)) / u.abs(), b.r);
        }
      }
    }
  }
  // Thin rectangles, arbitrary repeated start/end vertices and parallel
  // segment inputs. Exact dyadic scales remain well above tolerance.
  for (double h : {0.125, 1., 64.}) for (double gap : {0., 0.125, 4.})
    for (int repeat : {1, 2, 17}) {
      vector<Point> a, b;
      for (Point p : vector<Point>{{0, 0}, {64, 0}, {64, h}, {0, h}})
        for (int k = 0; k < repeat; ++k) a.pb(p);
      b = a; for (auto &p : b) p.y += h + gap;
      a.pb(a[0]); b.pb(b[0]);
      for (int k = 0; k < 4; ++k) {
        near(TwoConvexHullMinDist(a.data(), b.data(), SZ(a), SZ(b)), gap);
        rotate(a.begin(), a.begin() + 1, a.end());
        reverse(ALL(b));
      }
      vector<Point> segment{{0, 0}, {64, 0}, {64, 0}, {0, 0}};
      near(TwoConvexHullMinDist(segment.data(), b.data(), 4, SZ(b)),
           h + gap);
    }
}
int main() {
  matching_audit(); cout << "PASS matching\n" << flush;
  centroid_audit(); cout << "PASS centroid\n" << flush;
  pal_audit(); cout << "PASS eertree\n" << flush;
  arithmetic_audit(); cout << "PASS arithmetic\n" << flush;
  geometry_audit(); cout << "PASS geometry\n" << flush;
  cout << "PASS round14 g01: ten templates, independent edge oracles\n";
}
