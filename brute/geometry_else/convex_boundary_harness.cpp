#include <bits/stdc++.h>
using namespace std;
#include "../../codebook/8_Geometry/PointInConvex.cpp"

long long checks = 0, boundaries = 0;
// Independent O(n) oracle; no fan search or template helpers.
int oracle(const vector<IPoint>& h, IPoint p) {
  bool boundary = false;
  for (int i = 0; i < (int)h.size(); ++i) {
    IPoint a = h[i], b = h[(i + 1) % h.size()];
    __int128 dx = (__int128)b.x - a.x, dy = (__int128)b.y - a.y;
    __int128 c = dx * ((__int128)p.y - a.y) - dy * ((__int128)p.x - a.x);
    if (c < 0) return 0;
    boundary |= c == 0;
  }
  return boundary ? 1 : 2;
}
void check(const vector<IPoint>& h, IPoint p, int want) {
  int got = pointInConvex(h, p);
  if (got != want) {
    cerr << "FAIL query " << p.x << ' ' << p.y << ": " << got << " != " << want << '\n';
    exit(1);
  }
  ++checks;
  boundaries += want == 1;
}
vector<IPoint> hull(vector<IPoint> a) {
  sort(a.begin(), a.end(), [](IPoint p, IPoint q) { return tie(p.x,p.y) < tie(q.x,q.y); });
  a.erase(unique(a.begin(), a.end(), [](IPoint p, IPoint q) { return p.x == q.x && p.y == q.y; }), a.end());
  vector<IPoint> h;
  for (auto p : a) {
    while (h.size() > 1 && cross(h[h.size()-2], h.back(), p) <= 0) h.pop_back();
    h.push_back(p);
  }
  int s = h.size();
  for (int i = (int)a.size()-2; i >= 0; --i) {
    while ((int)h.size() > s && cross(h[h.size()-2], h.back(), a[i]) <= 0) h.pop_back();
    h.push_back(a[i]);
  }
  h.pop_back();
  return h;
}
void boundary_cases(vector<IPoint> h) {
  for (int rot = 0; rot < (int)h.size(); ++rot) {
    for (int i = 0; i < (int)h.size(); ++i) {
      IPoint a = h[i], b = h[(i+1)%h.size()];
      long long dx = b.x-a.x, dy = b.y-a.y, g = gcd(abs(dx), abs(dy));
      // Vertices and every lattice point on every edge.
      for (long long j = 0; j <= g; ++j) check(h, {a.x+dx/g*j, a.y+dy/g*j}, 1);
      // Points beyond both endpoints along the supporting line.
      check(h, {a.x-dx/g, a.y-dy/g}, 0);
      check(h, {b.x+dx/g, b.y+dy/g}, 0);
      // Internal fan diagonals must be classified as interior, not boundary.
      if (i >= 2 && i+1 < (int)h.size()) {
        dx = a.x-h[0].x; dy = a.y-h[0].y; g = gcd(abs(dx), abs(dy));
        for (long long j = 1; j < g; ++j)
          check(h, {h[0].x+dx/g*j, h[0].y+dy/g*j}, 2);
      }
    }
    rotate(h.begin(), h.begin()+1, h.end());
  }
}
int main() {
  check({}, {0,0}, 0);
  check({{1,2}}, {1,2}, 1); check({{1,2}}, {1,3}, 0);
  for (auto h : {vector<IPoint>{{0,0},{12,8}}, vector<IPoint>{{12,8},{0,0}}}) {
    for (int i = -1; i <= 5; ++i) check(h, {3*i,2*i}, i >= 0 && i <= 4 ? 1 : 0);
    check(h, {3,3}, 0);
  }
  boundary_cases({{0,0},{12,0},{12,12},{0,12}});
  boundary_cases({{0,0},{18,6},{6,18}});
  boundary_cases({{-12,0},{-6,-12},{6,-12},{12,0},{6,12},{-6,12}});
  mt19937 rng(123);
  for (int it = 0; it < 300; ++it) {
    vector<IPoint> a;
    for (int i = 0; i < 30; ++i) a.push_back({6*((int)(rng()%31)-15),6*((int)(rng()%31)-15)});
    auto h = hull(a);
    boundary_cases(h);
    for (int x = -102; x <= 102; x += 3) for (int y = -102; y <= 102; y += 3)
      check(h, {x,y}, oracle(h, {x,y}));
  }
  const long long B = 1000000000000000000LL;
  vector<IPoint> big = {{-B,-B},{B,-B},{B,B},{-B,B}};
  for (int rot = 0; rot < 4; ++rot) {
    for (auto p : big) check(big, p, 1);
    for (auto p : vector<IPoint>{{0,-B},{B,0},{0,B},{-B,0}}) check(big, p, 1);
    check(big, {0,0}, 2); check(big, {B-1,B-1}, 2);
    rotate(big.begin(), big.begin()+1, big.end());
  }
  cout << checks << " classifications PASS, including " << boundaries << " boundary queries\n";
}
