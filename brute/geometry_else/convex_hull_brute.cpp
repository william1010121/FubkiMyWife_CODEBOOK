#include <bits/stdc++.h>
using namespace std;
const double eps = 1e-9, pi = acos(-1.0);
int sign(double x) { return (x > eps) - (x < -eps); }
#include "../../codebook/8_Geometry/_basic.cpp"
#define ALL(v) v.begin(), v.end()
#define SZ(v) ((int)v.size())
#define pb push_back
#include "../../codebook/8_Geometry/convexHull.cpp"

void require(bool ok, const char *msg) {
  if (!ok) { cerr << "FAIL " << msg << '\n'; exit(1); }
}
bool equal_point(P a, P b) { return a.x == b.x && a.y == b.y; }
vector<P> oracle(vector<P> a) {
  sort(ALL(a), [](P p, P q) { return tie(p.x,p.y)<tie(q.x,q.y); });
  a.erase(unique(ALL(a), equal_point), a.end());
  if (SZ(a) < 2) return a;
  vector<P> h;
  for (int pass = 0; pass < 2; ++pass) {
    int start = SZ(h);
    for (P q : a) {
      while (SZ(h) >= start + 2 &&
          crx(h[SZ(h)-2], h.back(), q) <= 0) h.pop_back();
      h.pb(q);
    }
    h.pop_back(); reverse(ALL(a));
  }
  return h;
}
void check_hull(vector<P> a) {
  auto got = convex(a), want = oracle(a);
  require(SZ(got) == SZ(want), "hull size, including degeneracies");
  for (int i = 0; i < SZ(want); ++i)
    require(equal_point(got[i], want[i]), "hull order/vertices");
}
void check_queries(vector<P> h, mt19937 &rng) {
  if (SZ(h) < 3) return;
  for (int shift = 0; shift < 3; ++shift) {
    rotate(h.begin(), h.begin()+1, h.end());
    CH ch(h);
    for (int it = 0; it < 40; ++it) {
      P q{double(int(rng()%61)-30), double(int(rng()%61)-30)};
      if (it < ch.n) q = ch.p[it];
      bool inside = true;
      for (int i = 0; i < ch.n; ++i)
        inside &= crx(ch.p[i], ch.p[(i+1)%ch.n], q) >= -eps;
      require(ch.contain(q) == inside, "CH containment vs half-planes");
      int a = -1, b = -1;
      require(ch.get_tangent(q,a,b) == !inside, "CH tangent status");
      if (!inside) for (P z : ch.p) {
        require(crx(q,ch.p[a],z) <= eps, "CH tangent a support");
        require(crx(q,ch.p[b],z) >= -eps, "CH tangent b support");
      }
      P v{double(int(rng()%21)-10), double(int(rng()%21)-10)};
      double want = -1e100;
      for (P z : ch.p) want = max(want, z*v);
      require(ch.p[ch.findFarest(v)]*v == want, "CH extreme point");
      if (v.x == 0 && v.y == 0) continue;
      P end = q+v;
      double low = 1e100, high = -1e100;
      for (P z : ch.p) {
        double s = crx(q,end,z); low=min(low,s); high=max(high,s);
      }
      auto cross = ch.getLineIntersect(q,end);
      require(SZ(cross) == (low < -eps && high > eps ? 2 : 0),
              "CH line intersection status, including tangency");
      for (P z : cross) {
        require(abs(crx(q,end,z)) < 1e-6, "intersection on line");
        bool on = false;
        for (int i = 0; i < ch.n; ++i) {
          P a = ch.p[i], b = ch.p[(i+1)%ch.n];
          on |= abs(crx(a,b,z)) < 1e-6 && (z-a)*(z-b) <= 1e-6;
        }
        require(on, "intersection on hull boundary");
      }
    }
  }
}
int main(int argc, char **argv) {
  string mode = argc > 1 ? argv[1] : "all";
  if (mode == "all" || mode == "order") {
    vector<P> near{{0,2},{0.75*eps,1},{1.5*eps,0}};
    for (P a : near) for (P b : near) for (P c : near) {
      require(!(a < b && b < c) || a < c, "sort transitivity");
      require((a > b) == (b < a), "reverse ordering");
    }
  }
  if (mode == "all" || mode == "small") {
    check_hull({}); check_hull({{3,7}});
    check_hull({{3,7},{3,7},{3,7}});
    check_hull({{1,1},{0,0},{2,2},{0,0}});
  }
  if (mode == "all" || mode == "large") {
    for (double y : {-2e12, 2e12}) {
      CH ch({{0,y},{2,y},{2,y+2},{0,y+2}});
      require(ch.contain({0,y+1}), "large-coordinate left boundary");
      require(ch.contain({2,y+1}), "large-coordinate right boundary");
      require(!ch.contain({1,y+3}), "large-coordinate outside");
    }
  }
  if (mode == "all" || mode == "stress") {
    // Every integer point on this parabola is a strict hull vertex.
    for (int side : {-1, 1}) {
      vector<P> h;
      for (int x = -50000; x <= 50000; ++x)
        h.pb(P(x, -side * double(x) * x));
      if (side > 0) reverse(ALL(h)); // CCW upper parabola.
      CH ch(h);
      for (int k = -50000; k <= 50000; ++k) {
        P q = ch.p[ch.findFarest(P(2.0*k, side))];
        require(q.x == k, "100001-vertex parabola extreme point");
        // A supporting edge gives two equal, adjacent maxima.
        if (k < 50000) {
          q = ch.p[ch.findFarest(P(2.0*k+1, side))];
          require(q.x == k || q.x == k+1, "adjacent equal maxima");
        }
      }
      for (P v : vector<P>{{1,0},{-1,0},{0,1},{0,-1},{0,0}}) {
        double want = -1e100;
        for (P q : ch.p) want = max(want, q*v);
        require(ch.p[ch.findFarest(v)]*v == want,
                "horizontal, vertical and zero-direction extrema");
      }
    }
  }
  if (mode == "all" || mode == "random") {
    mt19937 rng(8731);
    for (int tc = 0; tc < 2000; ++tc) {
      vector<P> a;
      int n = 3 + rng()%40;
      for (int i = 0; i < n; ++i)
        a.pb(P(int(rng()%41)-20, int(rng()%41)-20));
      check_hull(a); check_queries(oracle(a), rng);
    }
  }
  cout << "convex hull PASS: " << mode << '\n';
}
