#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define SZ(a) ((int)a.size())
#define ALL(v) v.begin(), v.end()
#define pb push_back
#include "../../codebook/8_Geometry/Triangulation.cpp"
using delaunay::Point;

ll cr(Point o, Point a, Point b) { return (a - o) ^ (b - o); }
// strictly inside circumcircle of CCW triangle abc
bool inCirc(Point p, Point a, Point b, Point c) {
  a = a - p, b = b - p, c = c - p;
  __int128 x = __int128(a.Norm()) * (b ^ c) + __int128(b.Norm()) * (c ^ a) + __int128(c.Norm()) * (a ^ b);
  return x > 0;
}
// boundary points count (including collinear) and doubled hull area
pair<int, ll> hull(vector<Point> p) {
  int n = p.size(), b = 0;
  sort(p.begin(), p.end());
  vector<Point> h;
  for (int pass = 0; pass < 2; ++pass) {
    size_t st = h.size();
    for (auto q : p) {
      while (h.size() >= st + 2 && cr(h[h.size() - 2], h.back(), q) <= 0) h.pop_back();
      h.push_back(q);
    }
    h.pop_back();
    reverse(p.begin(), p.end());
  }
  ll A = 0;
  for (size_t i = 0; i < h.size(); ++i) A += h[i] ^ h[(i + 1) % h.size()];
  for (auto q : p) {
    bool on = false;
    for (size_t i = 0; i < h.size() && !on; ++i) {
      Point a = h[i], c = h[(i + 1) % h.size()];
      if (cr(a, c, q) == 0 && min(a.x, c.x) <= q.x && q.x <= max(a.x, c.x) &&
          min(a.y, c.y) <= q.y && q.y <= max(a.y, c.y)) on = true;
    }
    b += on;
  }
  (void)n;
  return {b, A};
}

int main() {
  auto *saved = delaunay::MakeEdge({-1, -2}, {-3, -4});
  mt19937 rng(11);
  for (int it = 0; it < 20000; ++it) {
    int n = rng() % 30, R = 1 + rng() % 8;
    bool big = it % 10 == 0;
    set<pair<ll, ll>> s;
    while ((int)s.size() < n) {
      if (big) s.insert({(ll)(rng() % 1000000001) - 500000000, (ll)(rng() % 1000000001) - 500000000});
      else {
        if ((int)s.size() >= (R + 1) * (R + 1)) break;
        s.insert({rng() % (R + 1), rng() % (R + 1)});
      }
    }
    vector<Point> p;
    for (auto [x, y] : s) p.push_back(Point(x, y));
    n = p.size();
    auto tri = delaunay::Triangulate(p);
    if (delaunay::edges.size() != 1 || saved->p.x != -1 ||
        saved->F().x != -3 || saved->r()->r() != saved) {
      puts("FAIL edge storage lifetime"); return 1;
    }
    bool allcol = true;
    for (int i = 2; i < n; ++i) if (cr(p[0], p[1], p[i]) != 0) allcol = false;
    auto fail = [&](const char *m) {
      printf("FAIL %s it=%d n=%d tris=%zu\n", m, it, n, tri.size());
      for (auto q : p) printf("(%lld,%lld) ", q.x, q.y);
      puts("");
      exit(1);
    };
    if (n < 3 || allcol) { if (!tri.empty()) fail("expected empty"); continue; }
    auto [b, A] = hull(p);
    if ((int)tri.size() != 2 * n - 2 - b) fail("count");
    ll sum = 0;
    for (auto &t : tri) {
      ll c = cr(t[0], t[1], t[2]);
      if (c <= 0) fail("not ccw");
      sum += c;
      for (auto q : p) if (inCirc(q, t[0], t[1], t[2])) fail("not delaunay");
    }
    if (sum != A) fail("area");
  }
  vector<Point> grid;
  for (int x = 0; x < 250; ++x) for (int y = 0; y < 200; ++y)
    grid.emplace_back(x, y);
  auto tri = delaunay::Triangulate(grid);
  if (tri.size() != 2 * 249 * 199 || delaunay::edges.size() != 1) {
    puts("FAIL 50000-point grid count/lifetime"); return 1;
  }
  for (auto t : tri) if (cr(t[0], t[1], t[2]) != 1) {
    puts("FAIL grid triangle orientation/area"); return 1;
  }
  delaunay::edges.clear();
  puts("OK: 20000 brute cases, repeated calls, 50000-point grid");
}
