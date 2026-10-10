#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define SZ(a) ((int)(a).size())
#define ALL(a) (a).begin(), (a).end()
#define pb push_back
namespace lcs {
#include "../../codebook/9_Else/All_LCS.cpp"
}
namespace suffix {
#include "../../codebook/5_String/SAIS-C++20.cpp"
}
namespace blossom {
#include "../../codebook/4_Flow_Matching/Maximum_Simple_Graph_Matching.cpp"
}
namespace weighted {
const int N = 10;
const ll INF = 1LL << 60;
#include "../../codebook/4_Flow_Matching/Kuhn_Munkres.cpp"
}
namespace prime_count {
#include "../../codebook/6_Math/PiCount.cpp"
}
struct F2 {
  int x;
  F2(int x = 0) : x(x & 1) {}
  F2 operator-() const { return *this; }
  F2 &operator+=(F2 b) { x ^= b.x; return *this; }
  friend F2 operator*(F2 a, F2 b) { return a.x & b.x; }
  friend F2 operator/(F2 a, F2 b) {
    if (!b.x) abort();
    return a;
  }
  friend bool operator==(F2 a, F2 b) { return a.x == b.x; }
};
namespace recurrence {
#include "../../codebook/6_Math/Berlekamp-Massey.cpp"
}
namespace geometry {
const double eps = 1e-9, pi = acos(-1.);
int sign(double x) { return abs(x) < eps ? 0 : x > 0 ? 1 : -1; }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/sectorArea.cpp"
using pt = P;
double norm2(pt p) { return p * p; }
struct circle { pt c; double r; };
#include "../../codebook/8_Geometry/MinimumEnclosingCircle.cpp"
using Point = P;
double Cross(P a, P b) { return a ^ b; }
#include "../../codebook/8_Geometry/center.cpp"
}
void require(bool ok, const char *what) {
  if (!ok) throw runtime_error(what);
}
bool close(double a, double b, double tol = 1e-11) {
  return isfinite(a) && abs(a - b) <= tol * max(1., abs(b));
}
set<string> subsequences(const string &s) {
  set<string> a;
  for (int mask = 0; mask < (1 << SZ(s)); ++mask) {
    string t;
    for (int i = 0; i < SZ(s); ++i) if (mask >> i & 1) t += s[i];
    a.insert(t);
  }
  return a;
}
void check_lcs(const string &s, const string &t) {
  auto a = subsequences(s), b = subsequences(t);
  vector<string> want;
  size_t best = 0;
  for (const string &u : a) if (b.count(u)) best = max(best, u.size());
  for (const string &u : a)
    if (u.size() == best && b.count(u)) want.push_back(u);
  require(lcs::all_lcs(s, t) == want, "LCS byte-set oracle");
}
void audit_lcs() {
  vector<string> words;
  for (int n = 0, total = 1; n <= 4; ++n, total *= 3)
    for (int code = 0; code < total; ++code) {
      string s(n, 0); int z = code;
      for (char &c : s) c = char(z % 3 == 0 ? 0 :
                               z % 3 == 1 ? 128 : 255), z /= 3;
      words.push_back(s);
    }
  for (const auto &s : words) for (const auto &t : words) check_lcs(s, t);
  mt19937 rng(1107);
  for (int tc = 0; tc < 300; ++tc) {
    string s(rng() % 13, 0), t(rng() % 13, 0);
    for (char &c : s) c = char(rng() % 256);
    for (char &c : t) c = char(rng() % 256);
    check_lcs(s, t);
  }
  require(lcs::all_lcs(string(3000, 'x'), "xy") == vector<string>{"x"},
          "LCS asymmetric lengths");
  cout << "PASS LCS empty/arbitrary-byte/exhaustive/asymmetric\n";
}
void audit_suffix() {
  for (int n = 0; n <= 14; ++n) for (int mask = 0; mask < (1 << n);
       ++mask) {
    vector<int> s(n), want(n);
    for (int i = 0; i < n; ++i) s[i] = 1 + (mask >> i & 1);
    iota(ALL(want), 0);
    sort(ALL(want), [&](int a, int b) {
      return lexicographical_compare(s.begin() + a, s.end(),
                                     s.begin() + b, s.end());
    });
    suffix::Suffix got(s.data(), n);
    require(got.sa == want, "SAIS exhaustive binary order");
    for (int i = 0; i < n; ++i) {
      int h = 0;
      if (i) while (max(want[i], want[i - 1]) + h < n &&
                    s[want[i] + h] == s[want[i - 1] + h]) ++h;
      require(got.ra[want[i]] == i && got.hi[i] == h,
              "SAIS inverse/LCP");
    }
    s.push_back(0); want.insert(want.begin(), n);
    require(suffix::sais(span<const int>(s)) == want,
            "SAIS read-only random-access span");
  }
  cout << "PASS SAIS all binary words through length 14, span API\n";
}
int matching_oracle(const vector<vector<int>> &a, int mask,
                    vector<int> &memo) {
  if (!mask) return 0;
  int &r = memo[mask]; if (r >= 0) return r;
  int u = countr_zero((unsigned)mask), rest = mask ^ (1 << u);
  r = matching_oracle(a, rest, memo);
  for (int v = u + 1; v < SZ(a); ++v)
    if ((rest >> v & 1) && a[u][v])
      r = max(r, 1 + matching_oracle(a, rest ^ (1 << v), memo));
  return r;
}
void audit_blossom() {
  mt19937 rng(110711);
  for (int mask = 0; mask < (1 << 15); ++mask) {
    blossom::Matching m(6);
    vector<vector<int>> a(6, vector<int>(6));
    vector<pair<int, int>> edges;
    for (int i = 0, bit = 0; i < 6; ++i)
      for (int j = i + 1; j < 6; ++j, ++bit) if (mask >> bit & 1)
        edges.push_back({i, j}), a[i][j] = a[j][i] = 1;
    shuffle(ALL(edges), rng);
    for (auto [i, j] : edges) {
      m.add_edge(i, j); m.add_edge(i, j);
    }
    vector<int> memo(64, -1);
    int want = matching_oracle(a, 63, memo);
    require(m.solve() == want && m.solve() == 0,
            "Blossom every six-vertex graph/duplicates/repeat");
    for (int i = 0; i < 6; ++i) if (m.match[i] != 6)
      require(a[i][m.match[i]] && m.match[m.match[i]] == i,
              "Blossom reciprocal matching");
  }
  cout << "PASS blossom all 32768 six-vertex graphs with duplicates\n";
}
void audit_km() {
  mt19937 rng(11071107);
  weighted::KM m;
  for (int tc = 0; tc < 1200; ++tc) {
    int n = tc % 8;
    ll bound = n ? (weighted::INF - 1) / (2 * n) : 0;
    vector<vector<ll>> w(n, vector<ll>(n));
    vector<int> perm(n); iota(ALL(perm), 0); shuffle(ALL(perm), rng);
    m.init(n);
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
      w[i][j] = j == perm[i] || rng() % 3 ?
        (rng() % 2 ? bound : -bound) : -weighted::INF;
      if (w[i][j] != -weighted::INF) {
        m.add_edge(i, j, 17); // Last write wins.
        m.add_edge(i, j, w[i][j]);
      }
    }
    iota(ALL(perm), 0);
    ll best = LLONG_MIN;
    do {
      bool valid = true; ll sum = 0;
      for (int i = 0; i < n; ++i) {
        valid &= w[i][perm[i]] != -weighted::INF;
        if (w[i][perm[i]] != -weighted::INF) sum += w[i][perm[i]];
      }
      if (valid) best = max(best, sum);
    } while (next_permutation(ALL(perm)));
    for (int repeat = 0; repeat < 2; ++repeat) {
      require(m.solve() == best, "KM near sentinel bound/reset/repeat");
      vector<int> seen(n);
      for (int i = 0; i < n; ++i) {
        int j = m.fl[i];
        require(j >= 0 && j < n && !seen[j]++ && m.fr[j] == i &&
                w[i][j] != -weighted::INF, "KM actual perfect matching");
      }
    }
  }
  cout << "PASS KM sparse +/-floor((INF-1)/(2n)), reinit/repeated solve\n";
}
void audit_bm() {
  for (int n = 0; n <= 12; ++n) for (int mask = 0; mask < (1 << n);
       ++mask) {
    vector<F2> s(n);
    for (int i = 0; i < n; ++i) s[i] = mask >> i & 1;
    auto c = recurrence::BerlekampMassey(s);
    auto valid = [&](int k, int coef) {
      for (int i = k; i < n; ++i) {
        int value = 0;
        for (int j = 0; j < k; ++j)
          value ^= (coef >> j & 1) & s[i - j - 1].x;
        if (value != s[i].x) return false;
      }
      return true;
    };
    int best = n;
    for (int k = 0; k < n && best == n; ++k)
      for (int coef = 0; coef < (1 << k); ++coef)
        if (valid(k, coef)) { best = k; break; }
    int coef = 0;
    for (int i = 0; i < SZ(c); ++i) coef |= c[i].x << i;
    require(SZ(c) == best && valid(SZ(c), coef),
            "BM exhaustive F2 minimal order and coefficients");
  }
  cout << "PASS BM every F2 sequence through length 12\n";
}
void audit_prime_count() {
  const int limit = 300000;
  vector<int> pi(limit + 1); vector<bool> composite(limit + 1);
  for (int i = 2; i * i <= limit; ++i) if (!composite[i])
    for (int j = i * i; j <= limit; j += i) composite[j] = true;
  for (int i = 1; i <= limit; ++i)
    pi[i] = pi[i - 1] + (i >= 2 && !composite[i]);
  for (int i = 0; i <= limit; ++i)
    require(prime_count::PrimeCount(i) == pi[i],
            "PrimeCount dense square/fourth-power/odd-even boundaries");
  require(prime_count::PrimeCount(LLONG_MIN) == 0,
          "PrimeCount negative extreme");
  cout << "PASS PrimeCount all 0..300000, LLONG_MIN\n";
}
void audit_geometry() {
  using namespace geometry;
  vector<P> grid;
  for (int x = -1; x <= 1; ++x) for (int y = -1; y <= 1; ++y)
    grid.emplace_back(x, y);
  auto orient = [](P a, P b, P c) {
    double z = (b - a) ^ (c - a); return z > 0 ? 1 : z < 0 ? -1 : 0;
  };
  auto on = [&](P a, P b, P c) {
    return !orient(a, b, c) && min(a.x, b.x) <= c.x &&
      c.x <= max(a.x, b.x) && min(a.y, b.y) <= c.y &&
      c.y <= max(a.y, b.y);
  };
  for (P a : grid) for (P b : grid) {
    L l(a, b);
    for (P c : grid) {
      require(l.inside(c) == on(a, b, c), "Basic segment membership");
      if ((a - b).abs()) {
        P q = l.project(c), r = l.reflect(c);
        require(close((q - c) * (b - a), 0) &&
                close((q - a) ^ (b - a), 0), "Basic projection");
        require(close(l.reflect(r).x, c.x) &&
                close(l.reflect(r).y, c.y), "Basic reflection twice");
        require(close(l.get_ratio(q), l.get_ratio(c)), "Basic ratio");
      }
      for (P d : grid) {
        bool want = on(a, b, c) || on(a, b, d) || on(c, d, a) ||
          on(c, d, b) || (orient(a, b, c) * orient(a, b, d) < 0 &&
                         orient(c, d, a) * orient(c, d, b) < 0);
        require(SegmentIntersect(a, b, c, d) == want,
                "Basic point/endpoint/overlap/proper intersections");
        L k(c, d);
        if (((b - a) ^ (d - c)) != 0) {
          P q = Intersect(l, k);
          require(close((q - a) ^ (b - a), 0) &&
                  close((q - c) ^ (d - c), 0), "Basic line intersection");
        }
      }
    }
  }
  const vector<P> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1},
                          {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
  for (P a : dirs) for (P b : dirs) {
    double theta = acos(clamp(a * b / (a.abs() * b.abs()), -1., 1.));
    // Identical directions avoid acos's amplification of rounded cosine.
    if (!(a ^ b)) theta = a * b > 0 ? 0 : pi;
    for (int e : {-500, 0, 500}) for (int f : {-500, 0, 500})
      for (double r : {0., 1., 1e100})
        require(close(SectorArea(a * ldexp(1., e), b * ldexp(1., f), r),
                      r * r * theta / 2), "Sector exponent/branch cut");
  }
  const vector<P> ring = {{5, 0}, {-5, 0}, {0, 5}, {0, -5},
                          {3, 4}, {-3, 4}, {3, -4}, {-3, -4}};
  vector<P> empty;
  auto zero = min_enclosing(empty);
  require(zero.r == 0 && zero.c.x == 0 && zero.c.y == 0, "MEC empty");
  for (int mask = 0; mask < 64; ++mask) for (int e : {-200, 0, 200})
    for (int shift : {-1000000, 0, 1000000}) {
      double scale = ldexp(1., e); P origin(shift * scale, -shift * scale);
      vector<P> p = {origin + ring[0] * scale, origin + ring[1] * scale};
      for (int i = 0; i < 6; ++i) if (mask >> i & 1)
        p.push_back(origin + ring[i + 2] * scale);
      P first = p[0], second = p[1];
      p.push_back(first); p.push_back(second);
      auto c = min_enclosing(p);
      require(close(c.r / scale, 5) &&
              close((c.c.x - origin.x) / scale, 0, 1e-8) &&
              close((c.c.y - origin.y) / scale, 0, 1e-8),
              "MEC exact diametric support/subsets/duplicates/scales");
    }
  // A trapezoid is the union of a rectangle and a right triangle.
  // Collinear vertices and a closing duplicate do not change its centroid.
  for (double a : {1., 7., 1000000.}) for (double b : {1., 3., 20.})
    for (double h : {1., 4., 500.}) for (double shift : {0., 1e9}) {
      double area = (2 * a + b) * h / 2;
      P want(shift + (a * h * a / 2 + b * h / 2 * (a + b / 3)) / area,
             shift + (a * h * h / 2 + b * h / 2 * h / 3) / area);
      vector<P> p = {{0, 0}, {a + b, 0}, {a + b / 2, h / 2},
                     {a, h}, {0, h}, {0, h / 2}, {0, 0}};
      for (P &v : p) v = v + P(shift, shift);
      for (int reverse_order = 0; reverse_order < 2; ++reverse_order) {
        for (int start = 0; start < SZ(p); ++start) {
          P c = BaryCenter(p, -1);
          require(abs(c.x - want.x) <= 1e-6 && abs(c.y - want.y) <= 1e-6,
                  "Centroid trapezoid/collinear/closing point oracle");
          rotate(p.begin(), p.begin() + 1, p.end());
        }
        reverse(ALL(p));
      }
    }
  cout << "PASS basic/sector/MEC/centroid degenerate and scale cases\n";
}
int main() {
  try {
    audit_lcs(); audit_suffix(); audit_blossom(); audit_km();
    audit_bm(); audit_prime_count(); audit_geometry();
  } catch (const exception &e) {
    cerr << "FAIL g07 round11: " << e.what() << '\n'; return 1;
  }
}
