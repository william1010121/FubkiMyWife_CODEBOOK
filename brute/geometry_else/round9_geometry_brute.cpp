#include <bits/stdc++.h>
using namespace std;
const double eps = 1e-9, pi = acos(-1.);
int sign(double x) { return abs(x) < eps ? 0 : (x > 0 ? 1 : -1); }
double sq(double x) { return x * x; }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/sectorArea.cpp"
#include "../../codebook/8_Geometry/circle.cpp"
#include "../../codebook/8_Geometry/circleOrArea.cpp"
// Round-start formulas: check result order and append preservation.
vector<P> reference_cross(P a, P b, P o, double r) {
  P u = b - a, v = a - o; double A = u * u;
  if (!A) // Zero-length segment is a point.
    return same((a - o).abs(), r) ? vector<P>{a} : vector<P>{};
  // Compensated cross: long tilted lines can lose their small offset.
  double t0 = u.y * v.x, z = fma(u.x, v.y, -t0) - fma(u.y, v.x, -t0);
  double mid = -(u * v) / A, d = sq(r) - sq(z) / A, tol = eps / sqrt(A);
  P p = P(-u.y, u.x) * (z / A);
  vector<P> t;
  if (d >= -eps) {
    double root = sqrt(max(0., d) / A), i = mid - root, j = mid + root;
    if (i <= 1 + tol && i >= -tol) t.push_back(o + p - u * root);
    if (root > 0 && j <= 1 + tol && j >= -tol)
      t.push_back(o + p + u * root);
  } return t;
}
void reference_cover(C &a, C &b, vector<pair<double, double>> &res) {
  double d = (a.c - b.c).abs();
  if (same(a.r + b.r, d)) return;
  if (d <= abs(a.r - b.r) + eps) {
    if (a.r < b.r) res.emplace_back(0, 2 * pi);
  } else if (d < abs(a.r + b.r) - eps) {
    double o = acos((sq(a.r) + sq(d) - sq(b.r)) / (2 * a.r * d)),
      z = (b.c - a.c).angle();
    if (z < 0) z += 2 * pi;
    double l = z - o, r = z + o;
    if (l < 0) l += 2 * pi;
    if (r > 2 * pi) r -= 2 * pi;
    if (l > r) res.emplace_back(l, 2 * pi), res.emplace_back(0, r);
    else res.emplace_back(l, r);
  }
}
void require(bool ok) { if (!ok) { cerr << "FAIL\n"; exit(1); } }
int main() {
  mt19937_64 rng(90009);
  for (int i = 0; i < 200000; ++i) {
    P o{double(int(rng()%201)-100), double(int(rng()%201)-100)};
    double r = 1 + rng()%10, h = (int(rng()%401)-200) / 10.;
    P a = o + P{-20, h}, b = o + P{20, h};
    if (i%7 == 0) a = b;
    if (i%7 == 1) a = o;
    if (i%7 == 2) a = o + P{-r, 0}, b = o + P{r, 0};
    if (i%7 == 3) a = o + P{-1e8, .5}, b = o + P{1e8, .5};
    auto x = CircleCrossLine(a, b, o, r), y = reference_cross(a,b,o,r);
    require(x.size() == y.size());
    for (int j = 0; j < (int)x.size(); ++j)
      if (x[j].x != y[j].x || x[j].y != y[j].y) {
        cerr << setprecision(18) << "case " << i << " root " << j
          << " got " << x[j].x << ',' << x[j].y << " old "
          << y[j].x << ',' << y[j].y << '\n';
        require(false);
      }
  }
  for (int i = 0; i < 100000; ++i) {
    C a({double(int(rng()%101)-50), double(int(rng()%101)-50)},
      rng()%100 / 10.);
    C b({double(int(rng()%101)-50), double(int(rng()%101)-50)},
      rng()%100 / 10.);
    if (i%3 == 0) {
      double d = a.r + b.r + (int(rng()%5)-2) * eps / 2;
      b.c = a.c + P{d, 0}; // Exact and epsilon-near external contact.
    }
    vector<pair<double,double>> x{{-1,-1}}, y = x;
    AppendCoverSegment(a, b, x); reference_cover(a, b, y);
    require(x == y);
  }
  vector<C> row, column;
  for (int i = 0; i < 2000; ++i) {
    row.emplace_back(P{4.*i, 0}, 1);
    column.emplace_back(P{0, 4.*i}, 1);
  }
  require(abs(CircleUnionArea(row) - 2000*pi) < 1e-7);
  require(abs(CircleUnionArea(column) - 2000*pi) < 1e-7);
  vector<C> nested;
  for (int i = 0; i < 50000; ++i) nested.emplace_back(P{}, 1+i*.01);
  double want = pi * sq(nested.back().r);
  require(abs(CircleUnionArea(nested) - want) < want * 1e-12);
  reverse(nested.begin(), nested.end());
  require(abs(CircleUnionArea(nested) - want) < want * 1e-12);
  cout << "round9 PASS: segment/arc differential and 2000-circle stress\n";
}
