#include <bits/stdc++.h>
using namespace std;

using i128 = __int128_t;
struct Point {
  long long x = 0, y = 0;
  Point() = default;
  Point(long long x_, long long y_) : x(x_), y(y_) {}
  Point operator-(Point b) const { return {x - b.x, y - b.y}; }
  long long operator^(Point b) const { return x * b.y - y * b.x; }
  long long Norm() const { return x * x + y * y; }
  bool operator==(Point b) const { return x == b.x && y == b.y; }
  bool operator<(Point b) const { return tie(x, y) < tie(b.x, b.y); }
};
constexpr long long inf = (1LL << 60);
int Sign(long long value) { return (value > 0) - (value < 0); }
#include "../../codebook/8_Geometry/Triangulation.cpp"

static void require(bool condition, const string &message) {
  if (!condition) {
    cerr << "FAIL " << message << '\n';
    exit(1);
  }
}

static long long cross(Point a, Point b, Point c) {
  return (b - a) ^ (c - a);
}

static vector<Point> hull(vector<Point> points) {
  sort(points.begin(), points.end());
  vector<Point> result;
  for (Point p : points) {
    while (result.size() >= 2 && cross(result[result.size() - 2], result.back(), p) <= 0)
      result.pop_back();
    result.push_back(p);
  }
  size_t lower = result.size();
  for (int i = points.size() - 2; i >= 0; --i) {
    while (result.size() > lower && cross(result[result.size() - 2], result.back(), points[i]) <= 0)
      result.pop_back();
    result.push_back(points[i]);
  }
  result.pop_back();
  return result;
}

static i128 incircle_det(Point p, Point a, Point b, Point c) {
  a = a - p, b = b - p, c = c - p;
  i128 value = i128(a.Norm()) * (b ^ c) + i128(b.Norm()) * (c ^ a) +
               i128(c.Norm()) * (a ^ b);
  return value * Sign((b - a) ^ (c - a));
}

static bool general_position(const vector<Point> &points) {
  int n = points.size();
  for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
    for (int k = j + 1; k < n; ++k) {
      if (!cross(points[i], points[j], points[k])) return false;
      for (int l = k + 1; l < n; ++l)
        if (!incircle_det(points[l], points[i], points[j], points[k])) return false;
    }
  return true;
}

static void check(const vector<Point> &points) {
  auto triangles = delaunay::Triangulate(points);
  auto boundary = hull(points);
  require((int)triangles.size() == 2 * (int)points.size() - 2 - (int)boundary.size(),
          "Delaunay triangle count");
  set<array<Point, 3>> unique_triangles;
  i128 triangle_area2 = 0;
  for (auto triangle : triangles) {
    for (Point p : triangle)
      require(find(points.begin(), points.end(), p) != points.end(), "Delaunay vertex origin");
    long long area2 = cross(triangle[0], triangle[1], triangle[2]);
    require(area2 > 0, "Delaunay triangle CCW");
    triangle_area2 += area2;
    auto canonical = triangle;
    sort(canonical.begin(), canonical.end());
    require(unique_triangles.insert(canonical).second, "Delaunay duplicate triangle");
    for (Point p : points)
      require(incircle_det(p, triangle[0], triangle[1], triangle[2]) <= 0,
              "Delaunay empty circumcircle");
  }
  i128 hull_area2 = 0;
  for (int i = 0; i < (int)boundary.size(); ++i)
    hull_area2 += boundary[i] ^ boundary[(i + 1) % boundary.size()];
  require(triangle_area2 == hull_area2, "Delaunay area partition");
}

int main() {
  check({{0, 0}, {4, 0}, {1, 3}});
  check({{0, 0}, {5, 0}, {4, 4}, {0, 3}, {2, 1}});
  mt19937 rng(31415926);
  int cases = 0;
  while (cases < 1000) {
    int n = 3 + rng() % 9;
    set<Point> selected;
    while ((int)selected.size() < n)
      selected.insert({(int)(rng() % 61) - 30, (int)(rng() % 61) - 30});
    vector<Point> points(selected.begin(), selected.end());
    if (!general_position(points)) continue;
    check(points), ++cases;
  }
  cout << "Delaunay PASS\n";
}
