#include <bits/stdc++.h>
using namespace std;

struct VoronoiPoint {
  long double x = 0, y = 0;
  auto operator<=>(const VoronoiPoint &) const = default;
};

long double voronoi_cross(VoronoiPoint a, VoronoiPoint b, VoronoiPoint c) {
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

vector<char> voronoi_unbounded(const vector<VoronoiPoint> &points) {
  int n = points.size();
  vector<int> order(n);
  iota(order.begin(), order.end(), 0);
  sort(order.begin(), order.end(), [&](int a, int b) { return points[a] < points[b]; });
  vector<int> hull;
  for (int id : order) {
    while (hull.size() >= 2 &&
           voronoi_cross(points[hull[hull.size() - 2]], points[hull.back()], points[id]) < 0)
      hull.pop_back();
    hull.push_back(id);
  }
  size_t lower = hull.size();
  for (int z = n - 2; z >= 0; --z) {
    int id = order[z];
    while (hull.size() > lower &&
           voronoi_cross(points[hull[hull.size() - 2]], points[hull.back()], points[id]) < 0)
      hull.pop_back();
    hull.push_back(id);
  }
  if (n > 1) hull.pop_back();
  vector<char> result(n);
  for (int id : hull) result[id] = true;
  return result;
}

// Area of every Voronoi cell. Hull sites have unbounded cells and return -1. Unique sites are required. O(n^4), intended as a compact verified reference.
vector<long double> voronoi_cell_areas(const vector<VoronoiPoint> &points) {
  int n = points.size();
  if (!n) return {};
  vector<VoronoiPoint> sorted = points;
  sort(sorted.begin(), sorted.end());
  if (adjacent_find(sorted.begin(), sorted.end()) != sorted.end())
    throw invalid_argument("Voronoi sites must be unique");
  vector<char> unbounded = voronoi_unbounded(points);
  vector<long double> answer(n, -1);
  const long double eps = 1e-12L;
  for (int site = 0; site < n; ++site) {
    if (unbounded[site]) continue;
    struct Constraint { long double a, b, c; };
    vector<Constraint> constraints;
    for (int j = 0; j < n; ++j) if (j != site) {
      long double a = 2 * (points[j].x - points[site].x);
      long double b = 2 * (points[j].y - points[site].y);
      long double c = points[j].x * points[j].x + points[j].y * points[j].y -
                      points[site].x * points[site].x - points[site].y * points[site].y;
      constraints.push_back({a, b, c});
    }
    vector<VoronoiPoint> vertices;
    for (int j = 0; j < (int)constraints.size(); ++j)
      for (int k = j + 1; k < (int)constraints.size(); ++k) {
        auto p = constraints[j], q = constraints[k];
        long double det = p.a * q.b - p.b * q.a;
        if (fabsl(det) <= eps) continue;
        VoronoiPoint vertex{(p.c * q.b - p.b * q.c) / det,
                            (p.a * q.c - p.c * q.a) / det};
        bool valid = true;
        for (auto half_plane : constraints) {
          long double lhs = half_plane.a * vertex.x + half_plane.b * vertex.y;
          long double scale = 1 + fabsl(half_plane.c) + fabsl(lhs);
          if (lhs > half_plane.c + eps * scale) { valid = false; break; }
        }
        if (valid) vertices.push_back(vertex);
      }
    sort(vertices.begin(), vertices.end(), [](auto a, auto b) {
      return tie(a.x, a.y) < tie(b.x, b.y);
    });
    vertices.erase(unique(vertices.begin(), vertices.end(), [&](auto a, auto b) {
      return fabsl(a.x - b.x) <= eps * (1 + fabsl(a.x) + fabsl(b.x)) &&
             fabsl(a.y - b.y) <= eps * (1 + fabsl(a.y) + fabsl(b.y));
    }), vertices.end());
    if (vertices.size() < 3) continue;
    VoronoiPoint center{};
    for (auto p : vertices) center.x += p.x, center.y += p.y;
    center.x /= vertices.size(), center.y /= vertices.size();
    sort(vertices.begin(), vertices.end(), [&](auto a, auto b) {
      return atan2l(a.y - center.y, a.x - center.x) <
             atan2l(b.y - center.y, b.x - center.x);
    });
    long double twice_area = 0;
    for (int i = 0; i < (int)vertices.size(); ++i) {
      auto a = vertices[i], b = vertices[(i + 1) % vertices.size()];
      twice_area += a.x * b.y - a.y * b.x;
    }
    answer[site] = fabsl(twice_area) / 2;
  }
  return answer;
}

#ifndef VORONOI_LIBRARY_ONLY
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  if (!(cin >> n)) return 0;
  vector<VoronoiPoint> points(n);
  for (auto &point : points) cin >> point.x >> point.y;
  for (long double area : voronoi_cell_areas(points)) {
    if (area < 0) cout << -1 << '\n';
    else cout << fixed << setprecision(12) << area << '\n';
  }
}
#endif
