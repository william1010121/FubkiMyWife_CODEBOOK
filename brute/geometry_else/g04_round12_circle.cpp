#include <bits/stdc++.h>
using namespace std;
const double eps = 1e-9, pi = acos(-1.);
double sq(double x) { return x*x; }
int sign(double x) { return (x > eps) - (x < -eps); }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/sectorArea.cpp"
#include "../../codebook/8_Geometry/circle.cpp"

int main() {
  const vector<P> triangle = {{0, 0}, {2, 0}, {0, 2}};
  double total = 0;
  for (int i = 0; i < 200000; ++i)
    total += AreaOfCircleTriangle(triangle, 1);
  assert(abs(total - 200000*pi/4) < 1e-5);
  assert(triangle[1].x == 2 && triangle[2].y == 2);
  assert(abs(AreaOfCircleTriangle(vector<P>{{0,0},{0,2},{2,0}}, 1)
             - pi/4) < 1e-12);
  mt19937 rng(1204);
  for (int tc = 0; tc < 10000; ++tc) {
    const vector<P> t = {
      {double(int(rng()%11)-5), double(int(rng()%11)-5)},
      {double(int(rng()%11)-5), double(int(rng()%11)-5)},
      {double(int(rng()%11)-5), double(int(rng()%11)-5)}};
    double a = AreaOfCircleTriangle(t, 2);
    assert(a >= 0 && a <= 4*pi + 1e-8);
    assert(abs(a - AreaOfCircleTriangle(
      vector<P>{t[2],t[1],t[0]}, 2)) < 1e-8);
  }
  cout << "PASS circle const/lvalue/rvalue and 200k repeated areas\n";
}
