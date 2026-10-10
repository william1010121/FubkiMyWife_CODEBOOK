#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
#define X first
#define Y second
const double eps = 1e-9, pi = acos(-1.);
int sign(double x) { return abs(x) < eps ? 0 : (x > 0 ? 1 : -1); }
double sq(double x) { return x * x; }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/sectorArea.cpp"
#include "../../codebook/8_Geometry/circle.cpp"
#include "../../codebook/8_Geometry/CircleTangent.cpp"
#include "../../codebook/8_Geometry/circleOrArea.cpp"
#include "../../codebook/8_Geometry/rotatingSweepLine.cpp"
using Point = P;
#include "../../codebook/8_Geometry/minDistOfTwoConvex.cpp"

void require(bool ok, string msg) {
  if (!ok) { cerr << "FAIL " << msg << '\n'; exit(1); }
}
void near(double a, double b, double tol, string msg) {
  require(isfinite(a) && abs(a - b) <= tol * max(1., abs(b)), msg);
}
// Original union loop: all pair intervals, no reused buffer/early exit.
double reference_union(vector<C> c) {
  if (!c.empty()) { P o = c[0].c; for (auto &v : c) v.c = v.c - o; }
  double ans = 0;
  for (int i = 0; i < (int)c.size(); ++i) {
    vector<pair<double, double>> s{{2 * pi, 2 * pi}};
    for (int j = 0; j < (int)c.size(); ++j) if (i != j)
      for (auto v : CoverSegment(c[i], c[j])) s.push_back(v);
    sort(s.begin(), s.end()); double w = 0;
    auto F = [&](double t) { return c[i].r * (c[i].r * t +
      c[i].c.x * sin(t) - c[i].c.y * cos(t)); };
    for (auto [l, r] : s) {
      if (l > w) ans += F(l) - F(w);
      w = max(w, r);
    }
  } return ans / 2;
}
void test_union() {
  mt19937 rng(60009);
  for (int tc = 0; tc < 3000; ++tc) {
    vector<C> cs;
    int n = 1 + rng() % 14;
    for (int i = 0; i < n; ++i) {
      P p{double(int(rng() % 2001) - 1000) / 100.,
        double(int(rng() % 2001) - 1000) / 100.};
      if (tc % 3 == 0) p = P{}; // Full containment, varied order.
      cs.emplace_back(p, .1 + rng() % 1000 / 100. + i * 1e-5);
    }
    if (tc % 5 == 0) cs[0].r = 0;
    if (tc % 7 == 0) for (auto &v : cs) v.c = v.c + P{1e8, -1e8};
    shuffle(cs.begin(), cs.end(), rng);
    near(CircleUnionArea(cs), reference_union(cs), 1e-12,
      "union all-pairs interval oracle");
  }
  vector<C> nested, dense, disjoint;
  const int n = 2000;
  for (int i = 0; i < n; ++i) {
    nested.emplace_back(P{}, 1 + i * .01);
    double t = 2 * pi * i / n;
    dense.emplace_back(P{cos(t), sin(t)}, 1.2);
    disjoint.emplace_back(P{double(i % 50) * 4, double(i / 50) * 4}, 1);
  }
  near(CircleUnionArea(nested), pi * sq(nested.back().r), 1e-12,
    "2000 nested circles");
  near(CircleUnionArea(disjoint), pi * n, 1e-10,
    "2000 disjoint circles");
  double r = cos(pi / n) + sqrt(1.44 - sq(sin(pi / n))),
    a = CircleUnionArea(dense);
  require(a >= pi * r * r - 1e-10 && a <= pi * sq(2.2) + 1e-10,
    "2000 dense circles: inscribed/circumscribed disk bounds");
}
void test_tangent() {
  for (double r : {1e-6, 1., 1e6, 1e9})
    for (double d : {1.000000001, 1.00001, 1.1, 2., 1e8}) {
      C c({3, -4}, r); P p = c.c + P{.6, .8} * (r * d);
      auto ls = tangent(c, p);
      if (same((p - c.c).abs(), r)) continue; // Epsilon contact.
      require(ls.size() == 2, "proper point tangent count");
      for (L l : ls) {
        near((l.pa - c.c).abs(), r, 2e-12, "tangent radius");
        P u = (l.pa - c.c) / r, v = (p - l.pa).unit();
        require(abs(u * v) < 2e-5, "near/far tangent orthogonality");
      }
      require(((p - c.c) ^ (ls[0].pa - c.c)) > 0,
        "point tangent plus-angle first");
    }
}
void test_sweep() {
  vector<pair<int, int>> ps;
  for (int i = 0; i < 1800; ++i) ps.emplace_back(i, i * i);
  mt19937 rng(60010); shuffle(ps.begin(), ps.end(), rng);
  auto got = rotatingSweepLineOrder(ps);
  vector<int> want(ps.size()); iota(want.begin(), want.end(), 0);
  sort(want.begin(), want.end(), [&](int a, int b) {
    return ps[a] > ps[b];
  });
  require(got == want, "1800 parabola sweep, parallel disjoint events");
}
void test_distance() {
  const int n = 200000; vector<Point> p, q;
  for (int i = 0; i < n; ++i) {
    double t = 2 * pi * i / n;
    p.push_back({1e4 * cos(t), 1e4 * sin(t)});
    q.push_back(p.back() + P{20002, 0});
  }
  near(TwoConvexHullMinDist(p.data(), q.data(), n, n), 2, 1e-8,
    "200000 vertices per polygon");
  reverse(q.begin(), q.end());
  near(TwoConvexHullMinDist(p.data(), q.data(), n, n), 2, 1e-8,
    "200000 opposite orientation");
}
int main() {
  test_union(); test_tangent(); test_sweep(); test_distance();
  cout << "round6 PASS: union differential and contest-scale stress\n";
}
