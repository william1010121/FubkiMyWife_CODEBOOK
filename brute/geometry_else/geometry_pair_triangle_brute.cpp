#include <bits/stdc++.h>
using namespace std;

struct Point {
  double x, y;
  Point operator-(Point q) const { return {x - q.x, y - q.y}; }
};
vector<Point> p;
long long distance_calls = 0, cross_calls = 0;
double dist(Point a, Point b) {
  ++distance_calls;
  return hypot(a.x - b.x, a.y - b.y);
}
double Cross(Point a, Point b) {
  ++cross_calls;
  return a.x * b.y - a.y * b.x;
}
#include "../../codebook/8_Geometry/ClosestPair.cpp"
#include "../../codebook/8_Geometry/maxTriangleOfConvex.cpp"

static void require(bool ok, const string &message) {
  if (!ok) { cerr << "FAIL " << message << '\n'; exit(1); }
}
static bool near(double got, long double want) {
  return fabsl(got - want) <= 2e-12L * max(1.0L, fabsl(want));
}
static long double cross(Point o, Point a, Point b) {
  return ((long double)a.x - o.x) * ((long double)b.y - o.y)
    - ((long double)a.y - o.y) * ((long double)b.x - o.x);
}
static void check_pair(vector<Point> points, int l = 0, int r = -2) {
  sort(points.begin(), points.end(), [](Point a, Point b) {
    return tie(a.x, a.y) < tie(b.x, b.y);
  });
  if (r == -2) r = (int)points.size() - 1;
  p = points;
  long double want = l >= r ? 1e9L : numeric_limits<long double>::infinity();
  for (int i = l; i <= r; ++i) for (int j = i + 1; j <= r; ++j)
    want = min(want, hypotl((long double)p[i].x - p[j].x,
                           (long double)p[i].y - p[j].y));
  require(near(closest_pair(l, r), want), "closest pair brute oracle");
  for (size_t i = 0; i < p.size(); ++i)
    require(p[i].x == points[i].x && p[i].y == points[i].y,
            "closest pair preserves all input points");
  require(near(closest_pair(l, r), want), "closest pair repeated call");
}
static vector<Point> hull(vector<Point> points) {
  sort(points.begin(), points.end(), [](Point a, Point b) {
    return tie(a.x, a.y) < tie(b.x, b.y);
  });
  points.erase(unique(points.begin(), points.end(), [](Point a, Point b) {
    return a.x == b.x && a.y == b.y;
  }), points.end());
  if (points.size() < 2) return points;
  vector<Point> result;
  for (Point q : points) {
    while (result.size() >= 2 &&
      cross(result[result.size() - 2], result.back(), q) <= 0) result.pop_back();
    result.push_back(q);
  }
  int lower = result.size();
  for (int i = (int)points.size() - 2; i >= 0; --i) {
    while ((int)result.size() > lower &&
      cross(result[result.size() - 2], result.back(), points[i]) <= 0)
      result.pop_back();
    result.push_back(points[i]);
  }
  result.pop_back();
  return result;
}
static void check_triangle(vector<Point> poly) {
  int n = poly.size();
  long double want = 0;
  for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
    for (int k = j + 1; k < n; ++k)
      want = max(want, fabsl(cross(poly[i], poly[j], poly[k])) / 2);
  vector<int> indices(n);
  iota(indices.begin(), indices.end(), 0);
  for (int direction = 0; direction < 2; ++direction) {
    for (int shift = 0; shift < min(n, 3); ++shift) {
      require(near(ConvexHullMaxTriangleArea(poly.data(), indices.data(), n), want),
              "maximum triangle CW/CCW and cyclic shifts");
      rotate(indices.begin(), indices.begin() + 1, indices.end());
    }
    reverse(indices.begin(), indices.end());
  }
  if (!n) require(ConvexHullMaxTriangleArea(nullptr, nullptr, 0) == 0,
                  "empty maximum triangle");
}
int main() {
  for (auto points : vector<vector<Point>>{
    {}, {{2, 3}}, {{0, 0}, {3, 4}}, {{0, 0}, {0, 0}, {8, 9}},
    {{0, 0}, {2e12, 0}, {4e12, 0}},
    {{1e12, -1e12}, {1e12 + 1, -1e12 + 1}, {-1e12, 1e12}},
    {{3, -5}, {3, 10}, {3, 10.25}, {3, 100}},
    {{-4, 0}, {-2, 0}, {0, 0}, {2, 0}, {4, 0}}}) check_pair(points);
  for (auto poly : vector<vector<Point>>{
    {}, {{1, 2}}, {{0, 0}, {3, 4}}, {{0, 0}, {1, 1}, {2, 2}, {3, 3}},
    {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}, {1, 2}, {0, 2}, {0, 1}},
    {{0, 0}, {0, 0}, {10, 0}, {5, 8}, {0, 1}, {0, 0}},
    {{-1e12, -1e12}, {1e12, -1e12}, {1e12, 1e12}, {-1e12, 1e12}},
    // Counterexample to the old, incorrect linear Dobkin-Snyder algorithm.
    hull({{4752,4262},{3383,413},{759,2927},{4745,4322},
          {1213,691},{2506,4423},{3040,4460},{1000,1000},{5000,1000}})
  }) check_triangle(poly);
  mt19937_64 rng(17291432);
  for (int tc = 0; tc < 10000; ++tc) {
    int n = rng() % 45;
    vector<Point> points;
    double scale = tc % 3 == 0 ? 1e10 : 1;
    for (int i = 0; i < n; ++i)
      points.push_back({(int(rng() % 41) - 20) * scale,
                        (int(rng() % 41) - 20) * scale});
    check_pair(points);
    if (n >= 4) check_pair(points, 1, n - 2);
  }
  for (int tc = 0; tc < 6000; ++tc) {
    vector<Point> points;
    for (int i = 0, n = 3 + rng() % 30; i < n; ++i)
      points.push_back({double(int(rng() % 101) - 50),
                        double(int(rng() % 101) - 50)});
    auto poly = hull(points);
    check_triangle(poly);
    vector<Point> with_collinear;
    for (int i = 0, n = poly.size(); i < n; ++i) {
      Point a = poly[i], b = poly[(i + 1) % n];
      with_collinear.push_back(a);
      if (tc % 2 == 0) with_collinear.push_back(a); // repeated boundary vertex
      with_collinear.push_back({(a.x + b.x) / 2, (a.y + b.y) / 2});
    }
    check_triangle(with_collinear);
  }
  // Structural scaling guards: count geometric evaluations, not wall time.
  int n = 100000;
  p.resize(n);
  vector<int> ys(n); iota(ys.begin(), ys.end(), 0);
  shuffle(ys.begin(), ys.end(), rng);
  for (int i = 0; i < n; ++i) p[i] = {0, double(ys[i])};
  distance_calls = 0;
  require(closest_pair(0, n - 1) == 1, "large vertical closest pair");
  require(distance_calls < 64LL * n * 17, "closest pair evaluation bound");
  vector<Point> poly(1000);
  vector<int> ids(poly.size()); iota(ids.begin(), ids.end(), 0);
  for (int i = 0; i < (int)poly.size(); ++i) {
    double angle = 2 * acos(-1.0) * i / poly.size();
    poly[i] = {cos(angle), sin(angle)};
  }
  cross_calls = 0;
  double area = ConvexHullMaxTriangleArea(poly.data(), ids.data(), poly.size());
  require(area > 1 && cross_calls < 16LL * (int)poly.size() * (int)poly.size(),
          "maximum triangle quadratic evaluation bound");
  cout << "pair/triangle PASS: 10000 pair, 6000 hull + collinear/duplicate cases; "
       << "sentinels, large coordinates, subranges, input preservation and scaling\n";
}
