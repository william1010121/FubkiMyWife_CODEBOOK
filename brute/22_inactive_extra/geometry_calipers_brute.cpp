#include <bits/stdc++.h>
using namespace std;

namespace rotating_template {
constexpr int maxn = 80;
int n;
#include "../../codebook/8_Geometry/RotatingCaliper.cpp"
}  // namespace rotating_template

namespace diameter_template {
struct Point {
  double x, y;
  Point operator-(const Point &other) const { return {x - other.x, y - other.y}; }
};
double Cross(const Point &a, const Point &b) { return a.x * b.y - a.y * b.x; }
double dis(const Point &a, const Point &b) { return hypot(a.x - b.x, a.y - b.y); }
#include "../../codebook/8_Geometry/diameterOfConvex.cpp"
}  // namespace diameter_template

static void require(bool condition, const string &message) {
  if (!condition) {
    cerr << "FAIL " << message << '\n';
    exit(1);
  }
}

using IPoint = pair<int, int>;

static long long cross(const IPoint &o, const IPoint &a, const IPoint &b) {
  return 1LL * (a.first - o.first) * (b.second - o.second) -
         1LL * (a.second - o.second) * (b.first - o.first);
}

static vector<IPoint> brute_hull(vector<IPoint> points) {
  sort(points.begin(), points.end());
  points.erase(unique(points.begin(), points.end()), points.end());
  if (points.size() <= 1) return points;
  vector<IPoint> hull;
  for (const auto &point : points) {
    while (hull.size() >= 2 &&
           cross(hull[hull.size() - 2], hull.back(), point) <= 0) {
      hull.pop_back();
    }
    hull.push_back(point);
  }
  size_t lower_size = hull.size();
  for (int i = static_cast<int>(points.size()) - 2; i >= 0; --i) {
    while (hull.size() > lower_size &&
           cross(hull[hull.size() - 2], hull.back(), points[i]) <= 0) {
      hull.pop_back();
    }
    hull.push_back(points[i]);
  }
  hull.pop_back();
  return hull;
}

static vector<IPoint> template_hull(const vector<IPoint> &points) {
  using namespace rotating_template;
  n = static_cast<int>(points.size());
  for (int i = 0; i < n; ++i) p[i] = pnt(points[i].first, points[i].second);
  RotatingCaliper();
  vector<IPoint> result;
  for (int i = 0; i < tbz; ++i) result.emplace_back(p[tb[i]].x, p[tb[i]].y);
  return result;
}

static void test_rotating_hull() {
  vector<vector<IPoint>> fixed = {
      {{0, 0}},
      {{0, 0}, {3, 4}},
      {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
      {{0, 0}, {1, 0}, {2, 0}, {2, 2}, {1, 1}, {0, 2}},
  };
  for (const auto &points : fixed) {
    const auto want = brute_hull(points);
    require(template_hull(points) == want, "rotating hull fixed case");
    require(template_hull(points) == want, "rotating hull repeated call");
  }

  mt19937 rng(712367);
  for (int test = 0; test < 3000; ++test) {
    set<IPoint> unique_points;
    int count = 1 + rng() % 30;
    while (static_cast<int>(unique_points.size()) < count) {
      unique_points.emplace(static_cast<int>(rng() % 21) - 10,
                            static_cast<int>(rng() % 21) - 10);
    }
    vector<IPoint> points(unique_points.begin(), unique_points.end());
    shuffle(points.begin(), points.end(), rng);
    require(template_hull(points) == brute_hull(points), "rotating hull random");
  }
}

static vector<diameter_template::Point> convex_hull_for_diameter(vector<IPoint> points) {
  vector<IPoint> hull = brute_hull(std::move(points));
  vector<diameter_template::Point> result;
  for (auto [x, y] : hull) result.push_back({static_cast<double>(x), static_cast<double>(y)});
  return result;
}

static void test_diameter() {
  mt19937 rng(998244353);
  for (int test = 0; test < 3000; ++test) {
    set<IPoint> unique_points;
    int count = 3 + rng() % 30;
    while (static_cast<int>(unique_points.size()) < count) {
      unique_points.emplace(static_cast<int>(rng() % 61) - 30,
                            static_cast<int>(rng() % 61) - 30);
    }
    auto points = convex_hull_for_diameter(
        vector<IPoint>(unique_points.begin(), unique_points.end()));
    if (points.size() < 3) continue;

    int n = static_cast<int>(points.size());
    vector<int> hull_indices(n + 1);
    iota(hull_indices.begin(), hull_indices.begin() + n, 0);
    vector<array<int, 2>> opposite(2 * n + 5);
    int opposite_count = 0;
    double got = diameter_template::Diameter(
        points.data(), hull_indices.data(), n,
        reinterpret_cast<int (*)[2]>(opposite.data()), opposite_count);
    double want = 0;
    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
        want = max(want, diameter_template::dis(points[i], points[j]));
      }
    }
    require(abs(got - want) < 1e-9, "convex diameter random");
    require(opposite_count == n, "one antipodal record per hull edge");
  }
}

int main() {
  test_rotating_hull();
  test_diameter();
  cout << "geometry calipers PASS\n";
}
