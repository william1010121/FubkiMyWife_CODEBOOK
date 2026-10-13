#include <bits/stdc++.h>
using namespace std;
const double eps = 1e-9, pi = acos(-1.);
int sign(double x) { return fabs(x) < eps ? 0 : (x > 0 ? 1 : -1); }
double sq(double x) { return x * x; }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/sectorArea.cpp"
#include "../../codebook/8_Geometry/halfPlaneIntersect.cpp"
#include "../../codebook/8_Geometry/circle.cpp"
using ld = long double;
void require(bool ok, string s) {
  if (!ok) { cerr << "FAIL " << s << '\n'; exit(1); }
}
ld polygon_area(const vector<P> &p) {
  ld a = 0;
  for (int i = 0; i < (int)p.size(); ++i) {
    auto u = p[i], v = p[(i + 1) % p.size()];
    a += (ld)u.x * v.y - (ld)u.y * v.x;
  }
  return abs(a) / 2;
}
vector<P> clip(vector<P> p, L l) {
  auto side = [&](P q) {
    return (ld)(l.pb.x - l.pa.x) * (q.y - l.pa.y) -
      (ld)(l.pb.y - l.pa.y) * (q.x - l.pa.x);
  };
  vector<P> out;
  for (int i = 0; i < (int)p.size(); ++i) {
    P a = p[i], b = p[(i + 1) % p.size()];
    ld x = side(a), y = side(b);
    if (x >= 0) out.push_back(a);
    if ((x < 0) != (y < 0))
      out.push_back(a + (b - a) * double(x / (x - y)));
  }
  return out;
}
void test_order() {
  vector<L> ls;
  for (double angle : {0., .75 * eps, 1.5 * eps})
    for (double y : {0., .75 * eps, 1.5 * eps})
      for (double scale : {1., 2.})
        ls.emplace_back(P{0, y}, P{scale, y + scale * angle});
  auto eq = [&](L a, L b) { return !cmp(a, b) && !cmp(b, a); };
  for (L a : ls) for (L b : ls) for (L c : ls) {
    require(!cmp(a, a), "HPI comparator irreflexive");
    require(!(cmp(a, b) && cmp(b, a)), "HPI comparator asymmetric");
    if (cmp(a, b) && cmp(b, c))
      require(cmp(a, c), "HPI comparator transitive");
    if (eq(a, b) && eq(b, c))
      require(eq(a, c), "HPI comparator equivalence transitive");
  }
}
void test_near_direction() {
  vector<L> near = {L({0,0},{1e6,0}), L({1e6,0},{1e6,2}),
    L({1e6,2},{0,2}), L({0,2},{0,0}), L({0,1},{1,1}),
    L({0,.99975},{1e6,1.00025})};
  require(abs(polygon_area(HPI(near)) - 999937.5L) < 1e-5,
    "distinct directions closer than epsilon must not be merged");
}
void test_hpi() {
  mt19937 rng(627338);
  for (int tc = 0; tc < 400; ++tc) {
    vector<P> expected = {{-10,-10},{10,-10},{10,10},{-10,10}};
    vector<L> ls;
    for (int i = 0; i < 4; ++i)
      ls.emplace_back(expected[i], expected[(i + 1) % 4]);
    for (int i = 0; i < 25; ++i) {
      int x = int(rng() % 11) - 5, y = int(rng() % 11) - 5;
      if (!x && !y) continue;
      double k = 1 + rng() % 3;
      P a{y * k, -x * k}, d{double(x), double(y)};
      ls.emplace_back(a, a + d);
      ls.emplace_back(a, a + d * 2); // Same line, different scale.
    }
    shuffle(ls.begin(), ls.end(), rng);
    for (auto l : ls) expected = clip(expected, l);
    auto got = HPI(ls);
    require(abs(polygon_area(got) - polygon_area(expected)) < 1e-7,
      "HPI independent clipping area");
    for (P p : got) for (L l : ls)
      require(((l.pb - l.pa) ^ (p - l.pa)) >= -1e-7,
        "HPI all vertices feasible");
  }
}
ld simpson(function<ld(ld)> &f, ld a, ld b, ld fa, ld fm,
    ld fb, ld whole, ld tol, int depth) {
  ld m = (a + b) / 2, fl = f((a + m) / 2), fr = f((m + b) / 2);
  ld l = (m - a) * (fa + 4 * fl + fm) / 6,
    r = (b - m) * (fm + 4 * fr + fb) / 6;
  if (!depth || abs(l + r - whole) < 15 * tol)
    return l + r + (l + r - whole) / 15;
  return simpson(f, a, m, fa, fl, fm, l, tol / 2, depth - 1) +
    simpson(f, m, b, fm, fr, fb, r, tol / 2, depth - 1);
}
// Independent oracle: integrate vertical triangle/disk slices.
ld triangle_disk_area(vector<P> p, ld radius) {
  if (((p[1] - p[0]) ^ (p[2] - p[0])) == 0) return 0;
  vector<ld> breaks = {-radius, radius};
  for (int i = 0; i < 3; ++i) {
    P a = p[i], b = p[(i + 1) % 3];
    breaks.push_back(a.x);
    ld dx = (ld)b.x - a.x, dy = (ld)b.y - a.y;
    ld A = dx * dx + dy * dy, B = 2 * (a.x * dx + a.y * dy),
      C = (ld)a.x * a.x + (ld)a.y * a.y - radius * radius;
    ld D = B * B - 4 * A * C;
    if (A && D >= 0) for (int s : {-1, 1}) {
      ld t = (-B + s * sqrt(D)) / (2 * A);
      if (t >= 0 && t <= 1) breaks.push_back(a.x + t * dx);
    }
  }
  sort(breaks.begin(), breaks.end());
  function<ld(ld)> slice = [&](ld x) {
    vector<ld> ys;
    for (int i = 0; i < 3; ++i) {
      P a = p[i], b = p[(i + 1) % 3];
      if (x < min(a.x, b.x) || x > max(a.x, b.x)) continue;
      if (a.x == b.x) ys.push_back(a.y), ys.push_back(b.y);
      else ys.push_back(a.y + (x - a.x) * (b.y - a.y) / (b.x - a.x));
    }
    if (ys.empty()) return 0.L;
    ld cap = sqrt(max(0.L, radius * radius - x * x));
    return max(0.L, min(cap, *max_element(ys.begin(), ys.end())) -
      max(-cap, *min_element(ys.begin(), ys.end())));
  };
  ld area = 0;
  ld lo = max(-radius, (ld)min({p[0].x, p[1].x, p[2].x})),
    hi = min(radius, (ld)max({p[0].x, p[1].x, p[2].x}));
  for (int i = 1; i < (int)breaks.size(); ++i) {
    ld a = max(lo, breaks[i - 1]), b = min(hi, breaks[i]);
    if (a >= b) continue;
    ld fa = slice(a), fm = slice((a + b) / 2), fb = slice(b),
      whole = (b - a) * (fa + 4 * fm + fb) / 6;
    area += simpson(slice, a, b, fa, fm, fb, whole, 1e-10L, 24);
  }
  return area;
}
void test_short_segment() {
  auto p = CircleCrossLine({.99999, 0}, {1.00001, 0}, {0, 0}, 1);
  require(p.size() == 1 && (p[0] - P{1, 0}).abs() < 1e-10,
    "short nonzero segment crosses circle");
  require(isfinite(AreaOfCircleTriangle({.99999, 0}, {1.00001, 0}, 1)),
    "short crossing segment triangle area has no out-of-range access");
}
void test_circle_area() {
  mt19937 rng(529139);
  for (int tc = 0; tc < 300; ++tc) {
    vector<P> p;
    for (int i = 0; i < 3; ++i)
      p.push_back({double(int(rng() % 13) - 6),
        double(int(rng() % 13) - 6)});
    double r = 1 + rng() % 4;
    ld want = triangle_disk_area(p, r);
    for (int direction = 0; direction < 2; ++direction) {
      double got = AreaOfCircleTriangle(p, r);
      if (abs(got - want) >= 1e-7) {
        cerr << setprecision(18) << "case " << tc << " r " << r
             << " got " << got << " want " << want << '\n';
        for (auto q : p) cerr << q.x << ',' << q.y << ' ';
        cerr << '\n';
      }
      require(abs(got - want) < 1e-7,
        "triangle/disk slice integral, both orientations");
      reverse(p.begin(), p.end());
    }
  }
  for (auto p : vector<vector<P>>{
    {{0,0},{0,0},{0,0}}, {{-3,0},{1,0},{4,0}},
    {{1,1},{2,2},{3,3}}, {{0,0},{1,0},{0,1}}})
    require(abs(AreaOfCircleTriangle(p, 5) - polygon_area(p)) < 1e-10,
      "contained and degenerate triangle areas");
}
int main(int argc, char **argv) {
  string mode = argc > 1 ? argv[1] : "all";
  if (mode == "order") { test_order(); return 0; }
  if (mode == "near") { test_near_direction(); return 0; }
  if (mode == "short") { test_short_segment(); return 0; }
  if (mode != "circle") {
    if (mode != "baseline") test_order(), test_near_direction();
    test_hpi();
  }
  if (mode != "hpi") {
    if (mode != "baseline") test_short_segment();
    test_circle_area();
  }
  cout << "round2 " << mode << " PASS: requested geometry checks\n";
}
