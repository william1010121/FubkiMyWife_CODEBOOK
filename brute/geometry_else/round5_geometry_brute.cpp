#include <bits/stdc++.h>
using namespace std;
const double eps = 1e-9, pi = acos(-1.);
int sign(double x) { return abs(x) < eps ? 0 : (x > 0 ? 1 : -1); }
double sq(double x) { return x * x; }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/sectorArea.cpp"
#include "../../codebook/8_Geometry/halfPlaneIntersect.cpp"
#include "../../codebook/8_Geometry/circle.cpp"
#include "../../codebook/8_Geometry/CircleTangent.cpp"
#include "../../codebook/8_Geometry/circleOrArea.cpp"
#include "../../codebook/8_Geometry/PointInConvex.cpp"
#include "../../codebook/8_Geometry/rotatingSweepLine.cpp"
struct Point {
  double x, y;
  Point(double x = 0, double y = 0) : x(x), y(y) {}
  Point operator+(Point p) const { return {x + p.x, y + p.y}; }
  Point operator-(Point p) const { return {x - p.x, y - p.y}; }
  Point operator*(double t) const { return {x * t, y * t}; }
  Point operator/(double t) const { return {x / t, y / t}; }
};
double Cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }
double len(Point p) { return hypot(p.x, p.y); }
#include "../../codebook/8_Geometry/triangleCenter.cpp"
#include "../../codebook/8_Geometry/center.cpp"
#include "../../codebook/8_Geometry/maxTriangleOfConvex.cpp"
#include "../../codebook/8_Geometry/minDistOfTwoConvex.cpp"

void require(bool ok, string message) {
  if (!ok) { cerr << "FAIL " << message << '\n'; exit(1); }
}
void near(double got, double want, double tol, string message) {
  require(isfinite(got) && abs(got - want) <= tol, message);
}
double area(const vector<P> &v) {
  double z = 0;
  for (int i = 1; i + 1 < (int)v.size(); ++i)
    z += (v[i] - v[0]) ^ (v[i + 1] - v[0]);
  return abs(z) / 2;
}
void test_audit() {
  // Exhaustive inclusive segment predicates, including point segments.
  vector<P> grid;
  for (int x = -1; x <= 1; ++x) for (int y = -1; y <= 1; ++y)
    grid.push_back({double(x), double(y)});
  auto on = [](P a, P b, P p) {
    return ((b - a) ^ (p - a)) == 0 &&
      min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
      min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
  };
  for (P a : grid) for (P b : grid) for (P c : grid) for (P d : grid) {
    double x = (b - a) ^ (c - a), y = (b - a) ^ (d - a),
      z = (d - c) ^ (a - c), w = (d - c) ^ (b - c);
    bool want = on(a, b, c) || on(a, b, d) || on(c, d, a) ||
      on(c, d, b) || (x * y < 0 && z * w < 0);
    require(SegmentIntersect(a, b, c, d) == want,
      "exhaustive basic segment predicate");
  }
  L l({2, 3}, {5, 7}); P p{8, -4}, q = l.project(p);
  near((q - l.pa) ^ (l.pb - l.pa), 0, 1e-12, "projection on line");
  near((p - q) * (l.pb - l.pa), 0, 1e-12, "projection normal");
  require((l.reflect(l.reflect(p)) - p).abs() < 1e-12,
    "reflection involution");
  require((Intersect(L({0, 0}, {1, 1}), L({0, 2}, {1, 1})) -
    P{1, 1}).abs() < 1e-12, "line intersection");
  for (double t : {-pi, -pi + 1e-8, -.4, 0., .4, pi - 1e-8, pi})
    near(SectorArea(P{1, 0}, P{cos(t), sin(t)}, 2), 2 * abs(t),
      1e-12, "sector angle wrapping and antipodal vectors");

  vector<L> empty, one{L({0, 0}, {1, 0})},
    parallel{L({0, 0}, {1, 0}), L({0, 1}, {1, 1})};
  require(HPI(empty).empty() && HPI(one).empty() &&
    HPI(parallel).empty(), "HPI insufficient directions");
  mt19937 rng(592073);
  for (int tc = 0; tc < 500; ++tc) {
    vector<L> box{L({0, 0}, {4, 0}), L({4, 0}, {4, 3}),
      L({4, 3}, {0, 3}), L({0, 3}, {0, 0})};
    box.emplace_back(P{0, -1}, P{16, -1});
    box.emplace_back(P{0, 0}, P{32, 0});
    box.emplace_back(P{4, 3}, P{4, 7});
    shuffle(box.begin(), box.end(), rng);
    auto v = HPI(box);
    near(area(v), 12, 1e-10, "HPI duplicate scaled constraints");
    for (P q : v) for (L l : box)
      require(((l.pb - l.pa) ^ (q - l.pa)) >= -eps,
        "HPI all vertices feasible");
  }

  require(rotatingSweepLineOrder({}).empty(), "empty rotating sweep");
  require(rotatingSweepLineOrder({{0, 0}}) == vector<int>{0},
    "singleton rotating sweep");
  for (auto ps : vector<vector<pair<int, int>>>{
    {{0, 0}, {0, 1}}, {{0, 0}, {2, 0}},
    {{0, 0}, {2, 0}, {2, 3}, {0, 3}},
    {{INT_MIN, INT_MIN}, {INT_MIN, INT_MAX},
      {INT_MAX, INT_MIN}, {INT_MAX, INT_MAX}}}) {
    sort(ps.begin(), ps.end());
    do {
      vector<int> want(ps.size()); iota(want.begin(), want.end(), 0);
      sort(want.begin(), want.end(), [&](int a, int b) {
        return ps[a] > ps[b];
      });
      require(rotatingSweepLineOrder(ps) == want,
        "sweep vertical and simultaneous parallel events");
    } while (next_permutation(ps.begin(), ps.end()));
  }

  vector<IPoint> square{{-1000000000000000000LL, -1000000000000000000LL},
    {1000000000000000000LL, -1000000000000000000LL},
    {1000000000000000000LL, 1000000000000000000LL},
    {-1000000000000000000LL, 1000000000000000000LL}};
  require(pointInConvex({}, {0, 0}) == 0 &&
    pointInConvex({{0, 0}}, {0, 0}) == 1 &&
    pointInConvex({{0, 0}}, {0, 1}) == 0 &&
    pointInConvex({{-2, 0}, {2, 0}}, {1, 0}) == 1 &&
    pointInConvex({{-2, 0}, {2, 0}}, {3, 0}) == 0,
    "convex membership empty/point/segment");
  require(pointInConvex(square, {0, 0}) == 2 &&
    pointInConvex(square, square[0]) == 1 &&
    pointInConvex(square, {0, square[0].y}) == 1,
    "convex membership coordinate limits");

  for (double t : {0., 1e9}) {
    Point a{t, t}, b{t + 6, t}, c{t, t + 8};
    auto cc = TriangleCircumCenter(a, b, c);
    near(cc.x - t, 3, 1e-7, "translated circumcenter x");
    near(cc.y - t, 4, 1e-7, "translated circumcenter y");
    auto ic = TriangleInnerCenter(a, b, c);
    near(ic.x - t, 2, 1e-7, "right triangle incenter");
    near(ic.y - t, 2, 1e-7, "right triangle incenter");
    auto hc = TriangleOrthoCenter(a, b, c);
    near(len(hc - a), 0, 1e-7, "right triangle orthocenter");
  }
  // Concavity, redundant vertices and the unused n parameter.
  vector<Point> concave{{0, 0}, {4, 0}, {4, 1}, {1, 1},
    {1, 4}, {0, 4}, {0, 0}, {0, 0}};
  for (int dir = 0; dir < 2; ++dir) {
    auto c = BaryCenter(concave, 0);
    near(c.x, 19. / 14, 1e-12, "concave centroid x");
    near(c.y, 19. / 14, 1e-12, "concave centroid y");
    reverse(concave.begin(), concave.end());
  }
  require(ConvexHullMaxTriangleArea(nullptr, nullptr, 0) == 0,
    "empty maximum triangle");
  Point same_points[] = {{1, 1}, {1, 1}, {1, 1}};
  int ids[] = {0, 1, 2};
  near(ConvexHullMaxTriangleArea(same_points, ids, 3), 0, 0,
    "maximum triangle all duplicates");
  for (int n = 1; n <= 2; ++n)
    near(ConvexHullMaxTriangleArea(same_points, ids, n), 0, 0,
      "maximum triangle too few vertices");
  Point seg[] = {{0, 0}, {4, 4}, {4, 4}, {0, 0}},
    pt[] = {{2, 2}}, off[] = {{2, 3}};
  near(TwoConvexHullMinDist(seg, pt, 4, 1), 0, 1e-12,
    "point on repeated closing segment");
  near(TwoConvexHullMinDist(seg, off, 4, 1), sqrt(.5), 1e-12,
    "point off diagonal segment");

  near(CircleUnionArea({}), 0, 0, "empty circle union");
  near(CircleUnionArea({C({0, 0}, 0)}), 0, 0, "zero-radius union");
  require(Intersect(C(), C({1, 0}, 1)).size() == 1,
    "zero-radius circle on boundary");
  require(CircleCrossLine({1, 0}, {1, 0}, {0, 0}, 1).size() == 1 &&
    CircleCrossLine({0, 0}, {0, 0}, {0, 0}, 1).empty(),
    "point segment on/off circle");
  near(AreaOfCircleTriangle(vector<P>{{1, 1}, {1, 1}, {1, 1}}, 2),
    0, 1e-12, "duplicate triangle vertices");
  near(AreaOfCircleTriangle(vector<P>{{-2, 0}, {2, 0}, {0, 3}}, 0),
    0, 0, "zero-radius triangle intersection");
  require(tangent(C({0, 0}, 2), P{0, 0}).empty(),
    "tangents from interior point");
  require(tangent(C({0, 0}, 2), C({0, 0}, 1)).empty(),
    "concentric common tangents");
  for (auto l : tangent(C({3, 4}, 5), P{6, 8}))
    require((l.pb - l.pa).abs() > 0,
      "point on circle has nonzero tangent");
  for (int tc = 0; tc < 5000; ++tc) {
    C a{{0, 0}, double(1 + rng() % 30)},
      b{{double(int(rng() % 81) - 40),
        double(int(rng() % 81) - 40)}, double(1 + rng() % 30)};
    if (a.r > b.r) swap(a, b);
    long double d = hypotl(a.c.x - b.c.x, a.c.y - b.c.y),
      r = a.r, s = b.r, want;
    if (d >= r + s) want = 0;
    else if (d + r <= s) want = r * r * acosl(-1.L);
    else {
      long double p = acosl((r * r + d * d - s * s) / (2 * r * d)),
        q = acosl((s * s + d * d - r * r) / (2 * s * d));
      want = p * r * r + q * s * s - r * d * sinl(p);
    }
    near(IntersectArea(a, b), want, 1e-8, "random lens area oracle");
  }
}

void test_segment() {
  for (double t : {1e4, 1e6, 1e8, 1e10, 1e12}) {
    auto v = CircleCrossLine({-t, -t + 1}, {t, t + 1}, {0, 0}, 1);
    require(v.size() == 2, "long tilted segment crossing count");
    require((v[0] - P{-1, 0}).abs() < 1e-10 &&
      (v[1] - P{0, 1}).abs() < 1e-10,
      "long tilted segment avoids cancellation in the offset");
  }
  for (double t : {1e4, 1e6, 1e8, 1e10, 1e12})
    for (double y : {0., .5, 1., 1.001, 2.}) {
      auto v = CircleCrossLine({-t, y}, {t, y}, {0, 0}, 1);
      int count = y < 1 ? 2 : y == 1 ? 1 : 0;
      require((int)v.size() == count, "long segment crossing count");
      if (count) {
        near(v.front().x, -sqrt(max(0., 1 - y * y)), 1e-10,
          "long segment first root");
        near(v.back().x, sqrt(max(0., 1 - y * y)), 1e-10,
          "long segment second root");
      }
      reverse(v.begin(), v.end());
      auto w = CircleCrossLine({t, y}, {-t, y}, {0, 0}, 1);
      require(w.size() == v.size(), "reversed segment count");
      for (int i = 0; i < (int)v.size(); ++i)
        require((v[i] - w[i]).abs() < 1e-10, "reversed root order");
    }
  for (double t : {1e8, 1e12}) {
    require(CircleCrossLine({2, 0}, {t, 0}, {0, 0}, 1).empty(),
      "parameter tolerance cannot admit remote out-of-segment roots");
    auto v = CircleCrossLine({1, 0}, {t, 0}, {0, 0}, 1);
    require(v.size() == 1 && (v[0] - P{1, 0}).abs() < 1e-10,
      "long segment endpoint root");
    double cap = acos(.5) - .5 * sqrt(.75);
    near(AreaOfCircleTriangle({-t, .5}, {t, .5}, 1),
      SectorArea({-t, .5}, {t, .5}, 1) - cap, 1e-10,
      "origin triangle with very long crossing edge");
    near(AreaOfCircleTriangle(vector<P>{{-t, .5}, {t, .5}, {0, t}}, 1),
      cap, 1e-10, "large triangle intersects circular cap");
  }
}

// Independent analytic lens oracle, evaluated at 100 decimal digits.
// Constants generated by the classical lens formula in Decimal arithmetic.
void test_lens() {
  vector<array<double, 3>> cases = {
    {1e4, -.5, 2.5273861528383834982},
    {1e4, 0., 1.5707629934615549526},
    {1e4, .5, .61416319948114474110},
    {1e6, -.5, 2.5274075877789826797},
    {1e6, 0., 1.570795993461563286},
    {1e6, .5, .61418463279810866651},
    {1e9, -.5, 2.5274078040689084647},
    {1e9, 0., 1.570796326461563286},
    {1e9, .5, .61418484908787207191}
  };
  for (auto [r, shift, want] : cases) {
    C a{{0, 0}, 1}, b{{r + shift, 0}, r};
    near(IntersectArea(a, b), want, 1e-9, "disparate circle lens");
    near(IntersectArea(b, a), want, 1e-9, "lens argument symmetry");
    auto v = Intersect(a, b);
    require(v.size() == 2, "disparate circle intersection count");
    long double d = r + shift;
    double x = (1 + (d - r) * (d + r)) / (2 * d);
    near(v[0].x, x, 1e-12, "factored common chord position");
    near(v[0].y, sqrt(1 - x * x), 1e-12, "common chord altitude");
  }
  auto almost_concentric = Intersect(C({0, 0}, 1), C({2 * eps, 0}, 1));
  require(almost_concentric.size() == 2, "nearly concentric count");
  near(almost_concentric[0].x, eps, 1e-12, "nearly concentric chord");
}
void test_union() {
  for (double t : {0., 1e8, 1e12, 1e16}) {
    near(CircleUnionArea({C({t, t}, 1)}), pi, 1e-12,
      "single-circle area invariant under translation");
    vector<C> v{C({t, t}, 2), C({t + 4, t}, 2)};
    near(CircleUnionArea(v), 8 * pi, 1e-10,
      "externally touching union invariant under translation");
  }
  for (double t : {0., 1e8, 1e12})
    near(CircleUnionArea({C({t, t}, 2), C({t + 1, t}, 2)}),
      8 * pi - IntersectArea(C({0, 0}, 2), C({1, 0}, 2)),
      1e-10, "overlapping union invariant under translation");
}
void test_tangent() {
  for (double r : {1e-12, 1e-10, 1., 1e6})
    require(tangent(C({0, 0}, r), P{0, 0}).empty(),
      "circle center is interior even for radius below eps");
}
void test_distance() {
  Point a[] = {{3e120, 4e120}}, b[] = {{0, 0}};
  near(TwoConvexHullMinDist(a, b, 1, 1) / 1e120, 5, 1e-12,
    "finite distances must not be capped by the answer sentinel");
}
void test_triangle() {
  vector<Point> p{{0, 0}, {1, 0}, {1, 1e-12}};
  vector<int> id{0, 1, 2};
  do {
    auto c = TriangleCircumCenter(p[id[0]], p[id[1]], p[id[2]]);
    near(c.x, .5, 1e-12, "skinny noncollinear circumcenter x");
    near(c.y, 5e-13, 1e-25, "skinny noncollinear circumcenter y");
    auto h = TriangleOrthoCenter(p[id[0]], p[id[1]], p[id[2]]);
    near(h.x, 1, 1e-12, "skinny right triangle orthocenter x");
    near(h.y, 0, 1e-25, "skinny right triangle orthocenter y");
  } while (next_permutation(id.begin(), id.end()));
}
int main(int argc, char **argv) {
  string mode = argc > 1 ? argv[1] : "all";
  if (mode == "all" || mode == "audit") test_audit();
  if (mode == "all" || mode == "segment") test_segment();
  if (mode == "all" || mode == "lens") test_lens();
  if (mode == "all" || mode == "union") test_union();
  if (mode == "all" || mode == "tangent") test_tangent();
  if (mode == "all" || mode == "distance") test_distance();
  if (mode == "all" || mode == "triangle") test_triangle();
  cout << "round5 " << mode << " PASS\n";
}
