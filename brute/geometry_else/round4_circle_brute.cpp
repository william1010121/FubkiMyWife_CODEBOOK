#include <bits/stdc++.h>
using namespace std;
const double eps = 1e-9, pi = acos(-1.);
int sign(double x) { return abs(x) < eps ? 0 : (x > 0 ? 1 : -1); }
double sq(double x) { return x * x; }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/sectorArea.cpp"
#include "../../codebook/8_Geometry/circle.cpp"
#include "../../codebook/8_Geometry/CircleTangent.cpp"
// Round-start angular formulas verify ordering, endpoints and tolerances.
vector<P> reference_intersect(C a, C b) {
  if (a.r > b.r) swap(a, b);
  double d = (a.c - b.c).abs(); vector<P> p;
  if (same(d, 0)) return p; // Coincident: no finite point list.
  if (same(a.r + b.r, d))
    p.push_back(a.c + (b.c - a.c) / d * a.r);
  else if (same(d + a.r, b.r))
    p.push_back(a.c - (b.c - a.c) / d * a.r);
  else if (a.r + b.r > d && d + a.r >= b.r) {
    double o = acos((sq(a.r) + sq(d) - sq(b.r)) / (2 * a.r * d));
    P i = (b.c - a.c) / d;
    p.push_back(a.c + i.rot(o) * a.r);
    p.push_back(a.c + i.rot(-o) * a.r);
  } return p;
}
// Positive radii; coincident circles have no finite tangent list.
vector<L> reference_tangent(C a, C b) {
  if (a.r < b.r) swap(a, b);
  vector<L> z; double d = (a.c - b.c).abs();
  if (same(d, 0) || d + b.r < a.r) return z;
  P u = (b.c - a.c) / d;
  auto one = [&] {
    P i = u * a.r, j(i.y, -i.x);
    z.emplace_back(a.c + i, a.c + i + j);
  };
  auto two = [&](double e, double side) {
    double o = acos(e / d);
    for (double t : {o, -o}) {
      P v = u.rot(t);
      z.emplace_back(a.c + v * a.r, b.c + v * (side * b.r));
    }
  };
  if (same(d + b.r, a.r)) one();
  else {
    two(a.r - b.r, 1);
    if (same(d, a.r + b.r)) one();
    else if (d > a.r + b.r) two(a.r + b.r, -1);
  } return z;
}
vector<L> reference_tangent(C c, P p) {
  vector<L> z; double d = (p - c.c).abs();
  if (same(d, c.r)) z.emplace_back(p, p + (p - c.c).rot(pi / 2));
  else if (d > c.r) {
    double o = acos(c.r / d); P u = (p - c.c) / d;
    for (double t : {o, -o}) z.emplace_back(c.c + u.rot(t) * c.r, p);
  } return z;
}

void require(bool ok, string message) {
  if (!ok) { cerr << "FAIL " << message << '\n'; exit(1); }
}
bool close(P a, P b) {
  return (a - b).abs() <= 2e-8 * max({1., a.abs(), b.abs()});
}
void check_pair(C a, C b) {
  auto got = Intersect(a, b), want = reference_intersect(a, b);
  require(got.size() == want.size(), "circle intersection count");
  for (int i = 0; i < (int)got.size(); ++i)
    require(close(got[i], want[i]), "circle intersection order/point");
  auto ts = tangent(a, b), ref = reference_tangent(a, b);
  require(ts.size() == ref.size(), "common tangent count");
  for (int i = 0; i < (int)ts.size(); ++i) {
    require(close(ts[i].pa, ref[i].pa) && close(ts[i].pb, ref[i].pb),
      "common tangent order/contact endpoints");
    P u = ts[i].pb - ts[i].pa;
    require(u.abs() > 0, "common tangent has a nonzero direction");
    for (C c : {a, b}) {
      double distance = abs(u ^ (c.c - ts[i].pa)) / u.abs();
      require(abs(distance - c.r) <= 2e-6 * max(1., c.r),
        "independent center-to-tangent distance");
    }
  }
}
void check_point(C c, P p) {
  auto got = tangent(c, p), ref = reference_tangent(c, p);
  require(got.size() == ref.size(), "point tangent count");
  for (int i = 0; i < (int)got.size(); ++i)
    require(close(got[i].pa, ref[i].pa) && close(got[i].pb, ref[i].pb),
      "point tangent order/endpoints");
}
int main() {
  mt19937_64 rng(980113);
  auto coord = [&] { return double(int(rng() % 20001) - 10000) / 97; };
  for (int tc = 0; tc < 20000; ++tc) {
    C a{{coord(), coord()}, .125 + double(rng() % 10000) / 71},
      b{{coord(), coord()}, .125 + double(rng() % 10000) / 71};
    check_pair(a, b);
    check_point(a, {coord(), coord()});
  }
  // Exact and epsilon-near contacts, containment, concentricity, swapping.
  for (double r : {1e-6, 1., 1000.})
    for (double q : {.25 * r, r})
      for (double delta : {-2 * eps, -.5 * eps, 0., .5 * eps, 2 * eps})
        for (double d : {r - q + delta, r + q + delta}) {
          if (d < 0) continue;
          C a{{0, 0}, r}, b{{d, 0}, q};
          check_pair(a, b), check_pair(b, a);
          check_point(a, {r + delta, 0});
        }
  for (double r : {1e-6, 1., 1000.}) {
    check_pair(C({0, 0}, r), C({0, 0}, r));
    check_pair(C({0, 0}, r), C({0, 0}, .5 * r));
    // Nonnegative radii apply to intersections; tangents require positive.
    for (double d : {0., r / 2, r, 2 * r}) {
      auto got = Intersect(C({0, 0}, 0), C({d, 0}, r));
      auto ref = reference_intersect(C({0, 0}, 0), C({d, 0}, r));
      require(got.size() == ref.size(), "zero-radius intersections");
      for (int i = 0; i < (int)got.size(); ++i)
        require(close(got[i], ref[i]), "zero-radius point");
    }
  }
  // Both contact tests can hold at once for a very small inner circle.
  check_pair(C({0, 0}, 1), C({1 - 1e-12, 0}, 1e-12));
  check_pair(C({0, 0}, 6), C({3, 4}, 1));
  check_pair(C({0, 0}, 3), C({3, 4}, 2));
  check_point(C({0, 0}, 5), {3, 4});
  cout << "round4 circle PASS: 20000 random pairs/points, "
       << "contact ordering/endpoints, epsilon contacts and zero radii\n";
}
