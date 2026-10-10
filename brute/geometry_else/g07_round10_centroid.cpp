#include <bits/stdc++.h>
using namespace std;
struct Point {
  double x, y;
  Point(double x = 0, double y = 0) : x(x), y(y) {}
  Point operator+(Point p) const { return {x + p.x, y + p.y}; }
  Point operator-(Point p) const { return {x - p.x, y - p.y}; }
  Point operator*(double t) const { return {x * t, y * t}; }
  Point operator/(double t) const { return {x / t, y / t}; }
};
double Cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }
#include "../../codebook/8_Geometry/center.cpp"
int main() {
  int cases = 0;
  for (double side : {100., 10000., 1000000.})
    for (double width : {1., 3., 10.})
      for (double shift : {0., 1e9, -1e12, 1e12, 1e15}) {
        // U = bottom rectangle + two disjoint vertical rectangles.
        long double l = side, w = width;
        long double area = w * l + 2 * w * (l - w);
        long double moment = w * l * w / 2
                           + w * (l - w) * (l + w);
        Point expected(shift + side / 2,
                       double((long double)shift + moment / area));
        vector<Point> p = {{0, 0}, {side, 0}, {side, side},
          {side - width, side}, {side - width, width}, {width, width},
          {width, side}, {0, side}};
        for (auto &q : p) q.x += shift, q.y += shift;
        double scale = abs(shift) + side;
        double tol = 2e-4 + 4 * (nextafter(scale, INFINITY) - scale);
        for (int orientation = 0; orientation < 2; ++orientation) {
          for (int start = 0; start < 8; ++start) {
            Point got = BaryCenter(p, -123); // n is deliberately unused.
            if (abs(got.x - expected.x) > tol ||
                abs(got.y - expected.y) > tol) {
              cerr << setprecision(17) << "centroid FAIL: side=" << side
                   << " width=" << width << " shift=" << shift
                   << " start=" << start << " direction=" << orientation
                   << " got=" << got.x << ',' << got.y << " expected="
                   << expected.x << ',' << expected.y << '\n';
              return 1;
            }
            ++cases;
            rotate(p.begin(), p.begin() + 1, p.end());
          }
          reverse(p.begin(), p.end());
        }
      }
  // 200k collinear subdivisions of a translated simple rectangle.
  vector<Point> p;
  const int steps = 50000;
  const double b = 1e12, side = 1e6;
  for (int edge = 0; edge < 4; ++edge)
    for (int i = 0; i < steps; ++i) {
      double t = side * i / steps;
      Point q = edge == 0 ? Point(t, 0) : edge == 1 ? Point(side, t)
              : edge == 2 ? Point(side - t, side) : Point(0, side - t);
      p.push_back({b + q.x, b + q.y});
    }
  for (int orientation = 0; orientation < 2; ++orientation) {
    Point got = BaryCenter(p, 0);
    assert(abs(got.x - (b + side / 2)) <= 0.001);
    assert(abs(got.y - (b + side / 2)) <= 0.001);
    reverse(p.begin(), p.end());
  }
  cout << "translated concave centroids PASS: " << cases
       << " cases + 200k-vertex CW/CCW stress\n";
}
