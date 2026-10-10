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
void edge_queries(vector<P> h) {
  if (SZ(h)<3) return;
  for (int shift=0;shift<SZ(h);++shift) {
    rotate(h.begin(),h.begin()+1,h.end()); CH ch(h);
    for (int x=-2;x<=6;++x) for (int y=-2;y<=6;++y) {
      P q(x*0.5,y*0.5);
      bool inside=true;
      for (int i=0;i<ch.n;++i)
        inside &= crx(ch.p[i],ch.p[(i+1)%ch.n],q)>=-eps;
      require(ch.contain(q)==inside,"exhaustive grid containment");
      int a,b;
      require(ch.get_tangent(q,a,b)==!inside,"grid tangent status");
      if (!inside) for (P z:ch.p) {
        require(crx(q,ch.p[a],z)<=eps,"grid tangent a");
        require(crx(q,ch.p[b],z)>=-eps,"grid tangent b");
      }
      // Lines through every vertex, incl. edges and vertex crossings.
      for (P end:ch.p) {
        if (equal_point(q,end)) continue;
        double lo=1e100,hi=-1e100;
        for (P z:ch.p) lo=min(lo,crx(q,end,z)),
                      hi=max(hi,crx(q,end,z));
        auto hits=ch.getLineIntersect(q,end);
        require(SZ(hits)==(lo < -eps && hi > eps ? 2:0),
                "grid line through vertex status");
        for (P z:hits) {
          require(abs(crx(q,end,z))<1e-7,"vertex crossing on line");
          bool on=false;
          for (int i=0;i<ch.n;++i) {
            P a=ch.p[i],b=ch.p[(i+1)%ch.n];
            on |= abs(crx(a,b,z))<1e-7 && (z-a)*(z-b)<=eps;
          }
          require(on,"vertex crossing on boundary");
        }
      }
    }
  }
}
int main(int argc, char **argv) {
  string mode = argc > 1 ? argv[1] : "all";
  if (mode == "all" || mode == "affine") {
    // Anisotropic, reflected and translated hulls; queries on edge rays.
    for (int swap_axes : {0,1}) for (int side : {-1,1}) {
      vector<P> input;
      for (P q : vector<P>{{0,0},{4,0},{5,1},{5,3},{3,5},
                           {1,5},{0,4},{-1,2},{2,2},{4,0}}) {
        if (swap_axes) swap(q.x,q.y);
        input.pb(P(1024+side*ldexp(q.x,-10),-2048+ldexp(q.y,10)));
      }
      check_hull(input);
      auto h=oracle(input);
      for (int shift=0;shift<SZ(h);++shift) {
        rotate(h.begin(),h.begin()+1,h.end()); CH ch(h);
        for (int i=0;i<ch.n;++i) {
          P a=ch.p[i],b=ch.p[(i+1)%ch.n],edge=b-a;
          P v(edge.y,-edge.x);
          double want=-INFINITY;
          for (P z:ch.p) want=max(want,z*v);
          require(ch.p[ch.findFarest(v)]*v==want,
                  "anisotropic supporting-edge extreme tie");
          require(ch.getLineIntersect(a,b).empty(),
                  "anisotropic supporting-edge intersection");
          for (double t : {-2.0,-0.5,0.0,0.5,1.0,1.5,3.0}) {
            P q=a+edge*t; bool inside=true;
            for (int j=0;j<ch.n;++j)
              inside &= crx(ch.p[j],ch.p[(j+1)%ch.n],q)>=-eps;
            require(ch.contain(q)==inside,"affine edge-ray containment");
            int x,y;
            require(ch.get_tangent(q,x,y)==!inside,"affine tangent status");
            if (!inside) for (P z:ch.p) {
              require(crx(q,ch.p[x],z)<=eps,"affine tangent a support");
              require(crx(q,ch.p[y],z)>=-eps,"affine tangent b support");
            }
          }
        }
      }
    }
    P hit=isLL({0,0},{100000,99999},{0,1},{99999,99999});
    require(equal_point(hit,{9999900000.0,9999800001.0}),
            "nonparallel integer lines with determinant one");
  }
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
  if (mode == "all" || mode == "grid") {
    for (int mask=0;mask<512;++mask) {
      vector<P> a;
      for (int i=0;i<9;++i) if (mask>>i&1)
        a.pb(P(i%3,i/3));
      check_hull(a); edge_queries(oracle(a));
      // Duplicates at every boundary location.
      auto twice=a; twice.insert(twice.end(),ALL(a)); check_hull(twice);
    }
  }
  if (mode == "all" || mode == "random") {
    mt19937 rng(8731);
    // Integer shears retain exact cross signs while making long skinny
    // hulls with near-parallel edges and widely separated dot products.
    for (int mask=0;mask<512;++mask) {
      vector<P> input;
      for (int i=0;i<9;++i) if (mask>>i&1) {
        double x=i%3-1,y=i/3-1;
        input.pb(P(1024*x+y,x)); input.pb(input.back());
      }
      check_hull(input); check_queries(oracle(input),rng);
      for (auto &p:input) swap(p.x,p.y), p.x=-p.x;
      check_hull(input); check_queries(oracle(input),rng);
    }
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
