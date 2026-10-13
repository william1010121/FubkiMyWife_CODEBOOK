#include <bits/stdc++.h>
using namespace std;
const double eps = 1e-9, pi = acos(-1.0);
int sign(double x) { return abs(x) < eps ? 0 : x > 0 ? 1 : -1; }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/halfPlaneIntersect.cpp"

double area(const vector<P> &p) {
  double z = 0;
  for (size_t i = 0; i < p.size(); ++i)
    z += p[i] ^ p[(i + 1) % p.size()];
  return abs(z) / 2;
}

int main() {
  const int n = 200000;
  mt19937 rng(1203);
  for (string shape : {"boundary", "parallel", "redundant"}) {
    vector<L> lines;
    for (int i = 0; i < n; ++i) {
      double angle = 2 * pi * (shape == "parallel" ?
          double(i % 4) / 4 : double(i) / n);
      P p(cos(angle), sin(angle)), d(-sin(angle), cos(angle));
      if (shape == "boundary") p = p * 1000;
      if (shape == "parallel") {
        P axis[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
        p = axis[i % 4], d = P(-p.y, p.x);
      }
      if (shape == "redundant") p = p * 2;
      lines.emplace_back(p, p + d * (1 + i % 11));
    }
    if (shape == "redundant") {
      vector<P> box{{-1, -1}, {1, -1}, {1, 1}, {-1, 1}};
      for (int i = 0; i < 4; ++i)
        lines.emplace_back(box[i], box[(i + 1) % 4]);
    }
    shuffle(lines.begin(), lines.end(), rng);
    auto p = HPI(lines);
    assert(p.size() == size_t(shape == "boundary" ? n : 4));
    double want = shape == "boundary" ? n * 1e6 * tan(pi / n) : 4;
    assert(abs(area(p) - want) < 1e-7 * want);
    for (P v : p) {
      assert(isfinite(v.x) && isfinite(v.y));
      if (shape == "boundary")
        assert(abs(v.abs() - 1000 / cos(pi / n)) < 1e-4);
      else assert(abs(abs(v.x) - 1) < 1e-7 &&
                  abs(abs(v.y) - 1) < 1e-7);
    }
  }
  cout << "HPI stress PASS: 200k boundary/parallel/redundant lines\n";
}
