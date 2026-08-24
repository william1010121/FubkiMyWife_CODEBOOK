#include <bits/stdc++.h>
using namespace std;

static void require(bool condition, const string &message) {
  if (!condition) {
    cerr << "FAIL " << message << '\n';
    exit(1);
  }
}

namespace centers {
const double pi = acos(-1.0);
struct Point {
  double x = 0, y = 0;
  Point() = default;
  Point(double x_, double y_) : x(x_), y(y_) {}
  Point operator+(Point b) const { return {x + b.x, y + b.y}; }
  Point operator-(Point b) const { return {x - b.x, y - b.y}; }
  Point operator*(double k) const { return {x * k, y * k}; }
  Point operator/(double k) const { return {x / k, y / k}; }
  double operator*(Point b) const { return x * b.x + y * b.y; }
};
double len(Point p) { return hypot(p.x, p.y); }
#include "../../codebook/8_Geometry/circumCenter.cpp"
#include "../../codebook/8_Geometry/innerCenter.cpp"
#include "../../codebook/8_Geometry/massCenter.cpp"
#include "../../codebook/8_Geometry/orthoCenter.cpp"
}  // namespace centers

namespace circle_tangent {
struct Point {
  double x = 0, y = 0;
  Point() = default;
  Point(double x_, double y_) : x(x_), y(y_) {}
  Point operator+(Point b) const { return {x + b.x, y + b.y}; }
  Point operator-(Point b) const { return {x - b.x, y - b.y}; }
  Point operator*(double k) const { return {x * k, y * k}; }
  Point operator/(double k) const { return {x / k, y / k}; }
  double operator*(Point b) const { return x * b.x + y * b.y; }
};
#include "../../codebook/8_Geometry/tangentToCircleFromPoint.cpp"
}  // namespace circle_tangent

namespace convex_contains {
struct point {
  long long x = 0, y = 0;
  point() = default;
  point(long long x_, long long y_) : x(x_), y(y_) {}
  point operator-(point b) const { return {x - b.x, y - b.y}; }
  long long operator^(point b) const { return x * b.y - y * b.x; }
  long long operator*(point b) const { return x * b.x + y * b.y; }
  double len() const { return hypot((double)x, (double)y); }
};
#include "../../codebook/8_Geometry/PointInConvex.cpp"
}  // namespace convex_contains

namespace old_kdtree {
constexpr int maxn = 256;
struct point { int x = 0, y = 0; };
#include "../../codebook/8_Geometry/KDtree.cpp"
}  // namespace old_kdtree

namespace second_hpi {
struct point {
  double x = 0, y = 0;
  point() = default;
  point(double x_, double y_) : x(x_), y(y_) {}
  point operator-(point b) const { return {x - b.x, y - b.y}; }
  double operator^(point b) const { return x * b.y - y * b.x; }
};
#include "../../codebook/8_Geometry/halfPlaneIntersect2.cpp"
}  // namespace second_hpi

static double cross(centers::Point a, centers::Point b) {
  return a.x * b.y - a.y * b.x;
}

static void test_centers() {
  mt19937 rng(44321);
  for (int test = 0; test < 5000; ++test) {
    using centers::Point;
    Point a{(int)(rng() % 41) - 20.0, (int)(rng() % 41) - 20.0};
    Point b{(int)(rng() % 41) - 20.0, (int)(rng() % 41) - 20.0};
    Point c{(int)(rng() % 41) - 20.0, (int)(rng() % 41) - 20.0};
    if (abs(cross(b - a, c - a)) < 1e-8) continue;
    Point circum = centers::TriangleCircumCenter(a, b, c);
    double ra = centers::len(circum - a);
    require(abs(ra - centers::len(circum - b)) < 1e-7, "circumcenter AB");
    require(abs(ra - centers::len(circum - c)) < 1e-7, "circumcenter AC");

    Point mass = centers::TriangleMassCenter(a, b, c);
    require(abs(mass.x * 3 - a.x - b.x - c.x) < 1e-9, "mass center x");
    require(abs(mass.y * 3 - a.y - b.y - c.y) < 1e-9, "mass center y");

    Point inner = centers::TriangleInnerCenter(a, b, c);
    auto line_distance = [](Point p, Point x, Point y) {
      return abs(cross(y - x, p - x)) / centers::len(y - x);
    };
    double da = line_distance(inner, b, c);
    require(abs(da - line_distance(inner, a, c)) < 1e-7, "incenter distance 1");
    require(abs(da - line_distance(inner, a, b)) < 1e-7, "incenter distance 2");

    Point ortho = centers::TriangleOrthoCenter(a, b, c);
    require(abs((ortho - a) * (c - b)) < 1e-6, "orthocenter altitude A");
    require(abs((ortho - b) * (c - a)) < 1e-6, "orthocenter altitude B");
  }
}

static void test_tangents() {
  mt19937 rng(76543);
  for (int test = 0; test < 4000; ++test) {
    using circle_tangent::Point;
    Point o{(int)(rng() % 21) - 10.0, (int)(rng() % 21) - 10.0};
    double r = 1 + rng() % 10;
    double angle = (rng() % 10000) * acos(-1.0) / 5000;
    double distance = r + 0.1 + rng() % 30;
    Point p{o.x + cos(angle) * distance, o.y + sin(angle) * distance};
    auto tangents = circle_tangent::tangent(o, r, p);
    for (Point t : tangents) {
      Point radius = t - o, direction = p - t;
      require(abs(sqrt(radius * radius) - r) < 1e-7, "tangent on circle");
      require(abs(radius * direction) < 1e-6, "tangent perpendicular");
    }
  }
}

using IPoint = convex_contains::point;
static long long icross(IPoint a, IPoint b, IPoint c) {
  return (b - a) ^ (c - a);
}

static bool brute_inside(const vector<IPoint> &polygon, IPoint p) {
  for (int i = 0; i < (int)polygon.size(); ++i)
    if (convex_contains::on(polygon[i], polygon[(i + 1) % polygon.size()], p)) return true;
  bool inside = false;
  for (int i = 0, j = polygon.size() - 1; i < (int)polygon.size(); j = i++) {
    const auto &a = polygon[i], &b = polygon[j];
    if ((a.y > p.y) != (b.y > p.y) &&
        (long double)p.x < (long double)(b.x - a.x) * (p.y - a.y) / (b.y - a.y) + a.x)
      inside = !inside;
  }
  return inside;
}

static vector<IPoint> integer_hull(vector<IPoint> points) {
  sort(points.begin(), points.end(), [](IPoint a, IPoint b) {
    return tie(a.x, a.y) < tie(b.x, b.y);
  });
  points.erase(unique(points.begin(), points.end(), [](IPoint a, IPoint b) {
    return a.x == b.x && a.y == b.y;
  }), points.end());
  vector<IPoint> hull;
  for (IPoint p : points) {
    while (hull.size() >= 2 && icross(hull[hull.size() - 2], hull.back(), p) <= 0) hull.pop_back();
    hull.push_back(p);
  }
  size_t lower = hull.size();
  for (int i = points.size() - 2; i >= 0; --i) {
    while (hull.size() > lower && icross(hull[hull.size() - 2], hull.back(), points[i]) <= 0) hull.pop_back();
    hull.push_back(points[i]);
  }
  hull.pop_back();
  return hull;
}

static void test_convex_contains() {
  mt19937 rng(88776);
  for (int test = 0; test < 400; ++test) {
    vector<IPoint> points;
    for (int i = 0; i < 30; ++i) points.emplace_back((int)(rng() % 31) - 15, (int)(rng() % 31) - 15);
    vector<IPoint> hull = integer_hull(points);
    if (hull.size() < 3) continue;
    for (int x = -18; x <= 18; ++x) for (int y = -18; y <= 18; ++y) {
      IPoint p{x, y};
      bool want = brute_inside(hull, p);
      require(convex_contains::in(hull, p) == want, "linear convex containment");
      require(convex_contains::inside(hull, p) == want, "log convex containment");
    }
  }
}

static void test_kdtree() {
  mt19937 rng(123987);
  for (int test = 0; test < 500; ++test) {
    vector<old_kdtree::point> points;
    set<pair<int, int>> used;
    int n = 2 + rng() % 100;
    while ((int)points.size() < n) {
      pair<int, int> p{(int)(rng() % 101) - 50, (int)(rng() % 101) - 50};
      if (used.insert(p).second) points.push_back({p.first, p.second});
    }
    old_kdtree::kdt::init(points);
    for (int query = 0; query < 80; ++query) {
      old_kdtree::point q{(int)(rng() % 121) - 60, (int)(rng() % 121) - 60};
      long long want = (long long)1e18;
      for (auto p : points) {
        long long d = old_kdtree::kdt::dist(p, q);
        if (d) want = min(want, d);
      }
      require(old_kdtree::kdt::nearest(q) == want, "old KD-tree nearest");
    }
  }
}

static long double polygon_area(const vector<second_hpi::point> &polygon) {
  long double area = 0;
  for (int i = 0; i < (int)polygon.size(); ++i) {
    auto a = polygon[i], b = polygon[(i + 1) % polygon.size()];
    area += (long double)a.x * b.y - (long double)a.y * b.x;
  }
  return abs(area) / 2;
}

static void test_second_hpi() {
  using second_hpi::plane;
  using second_hpi::point;
  vector<plane> square = {
      plane({1, 0}, {0, 0}), plane({1, 1}, {1, 0}),
      plane({0, 1}, {1, 1}), plane({0, 0}, {0, 1}),
  };
  auto result = second_hpi::hpi(square);
  require(result.size() == 4, "second HPI square vertex count");
  require(abs(polygon_area(result) - 1) < 1e-8, "second HPI square area");
  square.emplace_back(point{0.75, 0}, point{0.75, 1});
  result = second_hpi::hpi(square);
  require(abs(polygon_area(result) - 0.25) < 1e-8, "second HPI clipped area");
}

int main() {
  test_centers();
  test_tangents();
  test_convex_contains();
  test_kdtree();
  test_second_hpi();
  cout << "geometry support PASS\n";
}
