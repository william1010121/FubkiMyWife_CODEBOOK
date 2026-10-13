#include "../../codebook/1_Basic/Default_code.cpp"
const double eps = 1e-9, pi = acos(-1.);
int sign(double x) { return x < -eps ? -1 : x > eps; }
double sq(double x) { return x * x; }
#include "../../codebook/8_Geometry/_basic.cpp"
struct C { P c; double r; };
#include "../../codebook/8_Geometry/circleOrArea.cpp"
int main() {
  vector<C> zero;
  for (int i = 0; i < 20000; ++i)
    zero.pb({P(i % 200 * 3., i / 200 * 3.), 0.});
  assert(CircleUnionArea(zero) == 0);
  mt19937 rng(1802);
  for (int n : {1, 2, 17, 1000}) {
    // Equal disk chain: only adjacent disks overlap, no triple overlap.
    vector<C> c;
    for (int i = 0; i < n; ++i) {
      c.pb({P(1.5 * i, 0), 1});
      for (P p : {P(1.5 * i, 0), P(1.5 * i, 1),
                  P(1.5 * i, 1 + 2 * eps), P(1.5 * i, 4)})
        c.pb({p, 0});
    }
    double lens = 2 * acos(.75) - .75 * sqrt(4 - 2.25);
    double want = n * pi - (n - 1) * lens;
    for (int rep = 0; rep < 3; ++rep) {
      shuffle(ALL(c), rng);
      assert(abs(CircleUnionArea(c) - want) < 1e-6);
    }
  }
  // Distinct, tiny positive circles must not be mistaken for zero radii.
  vector<C> tiny{{P(0, 0), 1e-8}, {P(1, 0), 0},
                 {P(2, 0), 2e-8}};
  assert(abs(CircleUnionArea(tiny) - 5e-16 * pi) < 1e-22);
  cout << "PASS zero-radius union: 20k points, disk chains, tiny disks\n";
}
