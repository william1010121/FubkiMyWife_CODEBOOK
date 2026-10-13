#include "../../codebook/1_Basic/Default_code.cpp"
constexpr double eps = 1e-9, pi = acos(-1.);
int sign(double x) { return (x > eps) - (x < -eps); }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/halfPlaneIntersect.cpp"
int main() {
  mt19937 rng(1504);
  // Powers of two preserve directions exactly across 180 decades.
  for (auto [a, b] : vector<pii>{{1, 2}, {-3, 4}, {5, -2}, {-2, -7}}) {
    vector<L> ls;
    for (int i = 0; i < 200000; ++i) {
      double scale = ldexp(1., int(rng() % 601) - 300);
      L l; l.a = a * scale, l.b = b * scale;
      l.c = (int(rng() % 2001) - 1000) * scale;
      l.o = atan2(-double(a), double(b));
      ls.pb(l);
    }
    sort(ALL(ls), cmp);
    for (int i = 1; i < SZ(ls); ++i)
      assert((long double)ls[i - 1].c / ls[i - 1].a * a <=
             (long double)ls[i].c / ls[i].a * a);
  }
  // A max-norm quotient can overflow although hypot offsets are finite.
  L a, b; a.a = a.b = b.a = b.b = 1e-200;
  a.c = 1.9e108, b.c = 2e108, a.o = b.o = -pi / 4;
  assert(isfinite(a.c / hypot(a.a, a.b)));
  assert(isfinite(b.c / hypot(b.a, b.b)));
  assert(cmp(a, b) && !cmp(b, a));
  vector<L> ls;
  P normal[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  for (int i = 0; i < 200000; ++i) {
    P n = normal[i % 4], p = n * (1. + (i / 4) % 97 * .01);
    double scale = ldexp(1., int(rng() % 601) - 300);
    P d(-n.y * scale, n.x * scale);
    ls.emplace_back(p, p + d);
  }
  shuffle(ALL(ls), rng);
  auto p = HPI(ls);
  assert(p.size() == 4);
  double area = 0;
  for (int i = 0; i < 4; ++i) {
    assert(isfinite(p[i].x) && isfinite(p[i].y));
    assert(abs(abs(p[i].x) - 1) < eps);
    assert(abs(abs(p[i].y) - 1) < eps);
    area += p[i] ^ p[(i + 1) % 4];
  }
  assert(abs(abs(area) / 2 - 4) < eps);
  cout << "PASS HPI normalized offsets and 200k scaled parallels\n";
}
