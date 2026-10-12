#include <bits/stdc++.h>
#define ALL(v) v.begin(), v.end()
using namespace std;
using ll = long long;
using pii = pair<int, int>;
#define X first
#define Y second
const double eps = 1e-9, pi = acos(-1.);
int sign(double x) { return abs(x) < eps ? 0 : (x > 0 ? 1 : -1); }
double sq(double x) { return x * x; }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/sectorArea.cpp"
#include "../../codebook/8_Geometry/halfPlaneIntersect.cpp"
#include "../../codebook/8_Geometry/rotatingSweepLine.cpp"
#include "../../codebook/8_Geometry/PointInConvex.cpp"
#include "../../codebook/8_Geometry/circle.cpp"
#include "../../codebook/8_Geometry/CircleTangent.cpp"
#include "../../codebook/8_Geometry/circleOrArea.cpp"
struct Point {
  double x, y;
  Point(double x = 0, double y = 0) : x(x), y(y) {}
  Point operator+(Point p) const { return {x + p.x, y + p.y}; }
  Point operator-(Point p) const { return {x - p.x, y - p.y}; }
  Point operator*(double t) const { return {x * t, y * t}; }
  Point operator/(double t) const { return {x / t, y / t}; }
};
double Cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }
double len(Point p) { return hypot(p.x, p.y); }
#include "../../codebook/8_Geometry/triangleCenter.cpp"
#include "../../codebook/8_Geometry/center.cpp"
#include "../../codebook/8_Geometry/maxTriangleOfConvex.cpp"
#include "../../codebook/8_Geometry/minDistOfTwoConvex.cpp"
using ld = long double;
void require(bool ok, string msg) {
  if (!ok) { cerr << "FAIL " << msg << '\n'; exit(1); }
}
void near(double got, ld want, ld tol, string msg) {
  if (!isfinite(got) || abs(got - want) > tol) {
    cerr << setprecision(18) << "got=" << got << " want=" << want
      << " tol=" << tol << '\n';
    require(false, msg);
  }
}
ld turn(Point a, Point b, Point c) {
  return ((ld)b.x - a.x) * ((ld)c.y - a.y) -
    ((ld)b.y - a.y) * ((ld)c.x - a.x);
}
vector<Point> hull(vector<Point> p) {
  auto less = [](Point a, Point b) {
    return tie(a.x, a.y) < tie(b.x, b.y);
  };
  sort(p.begin(), p.end(), less);
  p.erase(unique(p.begin(), p.end(), [](Point a, Point b) {
    return a.x == b.x && a.y == b.y;
  }), p.end());
  if (p.size() < 2) return p;
  vector<Point> h;
  for (Point q : p) {
    while (h.size() > 1 && turn(h[h.size()-2], h.back(), q) <= 0)
      h.pop_back();
    h.push_back(q);
  }
  int k = h.size();
  for (int i = (int)p.size() - 2; i >= 0; --i) {
    while ((int)h.size() > k && turn(h[h.size()-2], h.back(), p[i]) <= 0)
      h.pop_back();
    h.push_back(p[i]);
  }
  h.pop_back(); return h;
}
ld segment_distance(Point p, Point a, Point b) {
  ld dx = (ld)b.x - a.x, dy = (ld)b.y - a.y, z = dx*dx + dy*dy;
  ld t = z ? clamp(((p.x-a.x)*dx + (p.y-a.y)*dy)/z, 0.L, 1.L) : 0;
  return hypotl(p.x - a.x - t*dx, p.y - a.y - t*dy);
}
int linear_inside(const vector<Point> &p, Point q) {
  if (p.empty()) return 0;
  if (p.size() < 3) return turn(p[0],p.back(),q) == 0 &&
    min(p[0].x,p.back().x) <= q.x && q.x <= max(p[0].x,p.back().x) &&
    min(p[0].y,p.back().y) <= q.y && q.y <= max(p[0].y,p.back().y);
  bool boundary = false;
  for (int i = 0; i < (int)p.size(); ++i) {
    ld z = turn(p[i], p[(i+1)%p.size()], q);
    if (z < 0) return 0;
    boundary |= z == 0;
  }
  return boundary ? 1 : 2;
}
vector<Point> repeat_vertices(const vector<Point> &p, mt19937 &rng) {
  vector<Point> q;
  for (int i = 0; i < (int)p.size(); ++i) {
    for (int k = 1 + rng()%4; k; --k) q.push_back(p[i]);
    Point a = p[i], b = p[(i+1)%p.size()];
    q.push_back((a*3+b)/4); q.push_back((a+b*3)/4);
  }
  q.push_back(q[0]); q.push_back(q[0]); return q;
}

void primitives() {
  vector<P> p;
  for (int x = -2; x <= 2; ++x) for (int y = -2; y <= 2; ++y)
    p.push_back({double(x), double(y)});
  auto on = [](P a, P b, P q) {
    return ((b-a)^(q-a)) == 0 &&
      min(a.x,b.x) <= q.x && q.x <= max(a.x,b.x) &&
      min(a.y,b.y) <= q.y && q.y <= max(a.y,b.y);
  };
  for (double scale : {1., 1e6}) {
    vector<P> q; for (P a : p) q.push_back(a*scale);
    for (P a : q) for (P b : q) {
      L l(a,b);
      for (P c : q) near(l.inside(c), on(a,b,c), 0,
        "line segment membership including point segments");
      for (P c : q) for (P d : q) {
        double x = (b-a)^(c-a), y = (b-a)^(d-a),
          z = (d-c)^(a-c), w = (d-c)^(b-c);
        bool want = on(a,b,c) || on(a,b,d) || on(c,d,a) ||
          on(c,d,b) || (x*y < 0 && z*w < 0);
        require(SegmentIntersect(a,b,c,d) == want,
          "scaled exhaustive segment intersection");
      }
      if ((b-a).abs()) {
        near((b-a).unit().abs(), 1, 1e-15, "unit vector");
        for (double t : {-2., 0., .25, 1., 3.}) {
          P c = a+(b-a)*t;
          near(l.get_ratio(c), t, 1e-12, "line ratio");
          require((l.project(c)-c).abs() < 1e-8,
            "projection fixes line points");
        }
      }
    }
  }
  for (P a : p) for (P b : p) if (a.abs() && b.abs())
    near(SectorArea(a,b,3), 4.5L*abs(atan2l(a^b,a*b)), 1e-14,
      "sector independent dot/cross angle");
}

void halfplanes() {
  mt19937 rng(851278);
  for (int tc=0;tc<1500;++tc) {
    vector<P> want{{-20,-10},{20,-10},{20,10},{-20,10}};
    vector<L> ls;
    for (int i=0;i<4;++i) ls.emplace_back(want[i],want[(i+1)%4]);
    for (int i=0;i<40;++i) {
      int x=int(rng()%21)-10, y=int(rng()%21)-10;
      if (!x && !y) continue;
      double d=1+rng()%12;
      P a{y*d,-x*d}, u{double(x),double(y)};
      ls.emplace_back(a,a+u);
      if (i%3==0) ls.emplace_back(a,a+u*8);
    }
    shuffle(ls.begin(),ls.end(),rng);
    // Sequential clipping is independent of sorting/deque closure.
    for (L l:ls) {
      vector<P> next;
      for (int i=0;i<(int)want.size();++i) {
        P a=want[i],b=want[(i+1)%want.size()];
        auto side=[&](P p) { return
          (ld)(l.pb.x-l.pa.x)*(p.y-l.pa.y) -
          (ld)(l.pb.y-l.pa.y)*(p.x-l.pa.x); };
        ld x=side(a),y=side(b);
        if (x>=0) next.push_back(a);
        if ((x<0)!=(y<0)) next.push_back(a+(b-a)*double(x/(x-y)));
      }
      want=next;
    }
    auto got=HPI(ls);
    auto area=[](const vector<P> &p) {
      ld z=0;
      for (int i=0;i<(int)p.size();++i)
        z+=(ld)p[i].x*p[(i+1)%p.size()].y -
          (ld)p[i].y*p[(i+1)%p.size()].x;
      return abs(z)/2;
    };
    require(!want.empty(),"clipping oracle has positive-area interior");
    near(area(got),area(want),1e-8,"adversarial redundant halfplanes");
    for (P p:got) {
      require(isfinite(p.x)&&isfinite(p.y),"finite HPI vertices");
      for (L l:ls) require(((l.pb-l.pa)^(p-l.pa))>=-1e-8,
        "every HPI vertex satisfies every constraint");
    }
  }
}

void polygons() {
  mt19937 rng(809171);
  vector<vector<Point>> polys;
  for (int mask = 1; mask < 512; ++mask) {
    vector<Point> p;
    for (int k = 0; k < 9; ++k) if (mask>>k & 1)
      p.push_back({double(k%3-1), double(k/3-1)});
    polys.push_back(hull(p));
  }
  require(pointInConvex({}, {0,0}) == 0, "empty convex input");
  for (auto p : polys) {
    // Exact linear half-plane oracle versus all fan origins.
    for (ll scale : {1LL, 100000000000000000LL}) {
      vector<IPoint> q;
      for (auto a : p) q.push_back({ll(a.x)*scale, ll(a.y)*scale});
      for (int k = 0; k < (int)p.size(); ++k) {
        for (int x = -3; x <= 3; ++x) for (int y = -3; y <= 3; ++y)
          require(pointInConvex(q, {x*scale,y*scale}) ==
            linear_inside(p, {double(x),double(y)}),
            "convex fan origins and large exact coordinates");
        rotate(q.begin(), q.begin()+1, q.end());
      }
    }
    auto q = repeat_vertices(p, rng);
    int n = q.size(); ld best = 0;
    for (int i = 0; i < n; ++i) for (int j = i+1; j < n; ++j)
      for (int k = j+1; k < n; ++k)
        best = max(best, abs(turn(q[i],q[j],q[k]))/2);
    vector<int> id(n); iota(id.begin(),id.end(),0);
    shuffle(id.begin(),id.end(),rng);
    vector<Point> data(n);
    for (int i = 0; i < n; ++i) data[id[i]] = q[i];
    for (int dir = 0; dir < 2; ++dir) {
      for (int k = 0; k < n; ++k) {
        near(ConvexHullMaxTriangleArea(data.data(),id.data(),n),
          best, 0, "unequal duplicate runs and indirect hull indices");
        rotate(id.begin(),id.begin()+1,id.end());
      }
      reverse(id.begin(),id.end());
    }
    if (p.size() > 2) {
      // Independent shoelace centroid, rather than the template's fan.
      ld area = 0, x = 0, y = 0;
      for (int i = 0; i < (int)p.size(); ++i) {
        auto a = p[i], b = p[(i+1)%p.size()];
        ld z = (ld)a.x*b.y - (ld)a.y*b.x;
        area += z; x += (a.x+b.x)*z; y += (a.y+b.y)*z;
      }
      for (int dir = 0; dir < 2; ++dir) {
        auto c = BaryCenter(q, -123);
        near(c.x,x/(3*area),1e-14,"redundant-vertex centroid x");
        near(c.y,y/(3*area),1e-14,"redundant-vertex centroid y");
        reverse(q.begin(),q.end());
      }
    }
  }
  for (int tc = 0; tc < 4000; ++tc) {
    auto a = polys[rng()%polys.size()], b = polys[rng()%polys.size()];
    for (auto &p : b) p.x += (tc%5)*2, p.y += tc%3;
    vector<Point> sums;
    for (auto p : a) for (auto q : b) sums.push_back(p-q);
    auto h = hull(sums); ld want = numeric_limits<ld>::infinity();
    if (linear_inside(h, {0,0})) want = 0;
    else for (int i = 0; i < (int)h.size(); ++i)
      want = min(want,segment_distance({0,0},h[i],h[(i+1)%h.size()]));
    a = repeat_vertices(a,rng); b = repeat_vertices(b,rng);
    for (int dir = 0; dir < 4; ++dir) {
      double got=TwoConvexHullMinDist(a.data(),b.data(),a.size(),b.size());
      if (abs(got-want)>1e-12) {
        cerr << "distance case " << tc << " orientation " << dir << '\n';
        for (auto p:a) cerr << p.x << ',' << p.y << ' ';
        cerr << '\n';
        for (auto p:b) cerr << p.x << ',' << p.y << ' ';
        cerr << '\n';
        for (auto p:h) cerr << p.x << ',' << p.y << ' ';
        cerr << '\n';
      }
      near(got,
        want,1e-12,"Minkowski oracle built from all pair differences");
      if (dir&1) reverse(a.begin(),a.end());
      else reverse(b.begin(),b.end());
    }
  }
}

void triangles_and_sweep() {
  vector<Point> grid;
  for (int x = -1; x <= 1; ++x) for (int y = -1; y <= 1; ++y)
    grid.push_back({double(x),double(y)});
  for (auto a : grid) for (auto b : grid) for (auto c : grid)
    if (turn(a,b,c)) {
      Point o = TriangleCircumCenter(a,b,c);
      near(len(o-a),len(o-b),1e-14,"circumcenter equal distances");
      near(len(o-a),len(o-c),1e-14,"circumcenter equal distances");
      auto h = TriangleOrthoCenter(a,b,c);
      near((h.x-a.x)*(b.x-c.x)+(h.y-a.y)*(b.y-c.y),0,1e-14,
        "orthocenter altitude");
      auto g = TriangleMassCenter(a,b,c);
      near(3*g.x,a.x+b.x+c.x,1e-14,"mass center x");
      near(3*g.y,a.y+b.y+c.y,1e-14,"mass center y");
      auto i = TriangleInnerCenter(a,b,c);
      ld d = abs(turn(a,b,i))/len(b-a);
      near(abs(turn(b,c,i))/len(c-b),d,1e-14,"incenter side distance");
      near(abs(turn(c,a,i))/len(a-c),d,1e-14,"incenter side distance");
    }
  int cases = 0;
  for (int mask = 0; mask < 512; ++mask) {
    vector<pair<int,int>> p;
    for (int k = 0; k < 9; ++k) if (mask>>k & 1)
      p.emplace_back(k%3-1,k/3-1);
    bool valid = true;
    for (int i = 0; i < (int)p.size(); ++i)
      for (int j = i+1; j < (int)p.size(); ++j)
        for (int k = j+1; k < (int)p.size(); ++k)
          valid &= (p[j].first-p[i].first)*(p[k].second-p[i].second) !=
            (p[j].second-p[i].second)*(p[k].first-p[i].first);
    if (!valid) continue;
    sort(p.begin(),p.end());
    do {
      vector<int> want(p.size()); iota(want.begin(),want.end(),0);
      sort(want.begin(),want.end(),[&](int a,int b) {return p[a]>p[b];});
      auto got=rotatingSweepLineOrder(p);
      if (got!=want) {
        cerr << "sweep mask " << mask << '\n';
        for (auto [x,y]:p) cerr << x << ',' << y << ' ';
        cerr << "\ngot "; for (int i:got) cerr << i << ' ';
        cerr << "\nwant "; for (int i:want) cerr << i << ' ';
        cerr << '\n';
      }
      require(got == want,
        "every lattice subset/permutation without three collinear");
      ++cases;
    } while (next_permutation(p.begin(),p.end()));
    auto saved = p; rotatingSweepLine(p);
    require(p == saved,"legacy sweep preserves input");
  }
  cout << "sweep permutations=" << cases << '\n';
}

ld integrate(function<ld(ld)> &f, ld a, ld b, ld fa, ld fm, ld fb,
    ld whole, ld tol, int depth) {
  ld mid=(a+b)/2, fl=f((a+mid)/2), fr=f((mid+b)/2);
  ld l=(mid-a)*(fa+4*fl+fm)/6, r=(b-mid)*(fm+4*fr+fb)/6;
  if (!depth || abs(l+r-whole) <= 15*tol)
    return l+r+(l+r-whole)/15;
  return integrate(f,a,mid,fa,fl,fm,l,tol/2,depth-1) +
    integrate(f,mid,b,fm,fr,fb,r,tol/2,depth-1);
}
ld disk_union_oracle(const vector<C> &c) {
  vector<ld> cuts;
  for (auto a : c) if (a.r)
    for (int s : {-1,0,1}) cuts.push_back((ld)a.c.x+s*a.r);
  sort(cuts.begin(),cuts.end());
  function<ld(ld)> slice = [&](ld x) {
    vector<pair<ld,ld>> v;
    for (auto a : c) if (a.r && abs(x-a.c.x) < a.r) {
      ld h=sqrt((ld)a.r*a.r-(x-a.c.x)*(x-a.c.x));
      v.emplace_back(a.c.y-h,a.c.y+h);
    }
    sort(v.begin(),v.end()); ld z=0, right=-1e30L;
    for (auto [a,b] : v) z+=max(0.L,b-max(a,right)),right=max(right,b);
    return z;
  };
  ld z=0;
  for (int i=1;i<(int)cuts.size();++i) {
    ld a=cuts[i-1],b=cuts[i]; if (a==b) continue;
    ld fa=slice(a),fm=slice((a+b)/2),fb=slice(b);
    z+=integrate(slice,a,b,fa,fm,fb,(b-a)*(fa+4*fm+fb)/6,1e-10L,28);
  }
  return z;
}
void circles() {
  mt19937 rng(871228);
  for (int tc=0;tc<200;++tc) {
    vector<C> c;
    for (int n=rng()%7;n;--n) {
      C a{{double(int(rng()%21)-10),double(int(rng()%21)-10)},
        double(rng()%6)};
      bool duplicate=false;
      for (auto b:c) duplicate |= a.c.x==b.c.x && a.c.y==b.c.y &&
        a.r==b.r;
      if (!duplicate) c.push_back(a);
    }
    near(CircleUnionArea(c),disk_union_oracle(c),2e-7,
      "union independent vertical-slice integration");
    if (c.size()==2) near(IntersectArea(c[0],c[1]),
      pi*(sq(c[0].r)+sq(c[1].r))-disk_union_oracle(c),2e-7,
      "lens independent vertical-slice integration");
  }
  for (int x=-6;x<=6;++x) for (int y=-6;y<=6;++y)
    for (int r=1;r<=4;++r) for (int s=1;s<=4;++s) {
      C a{{0,0},double(r)},b{{double(x),double(y)},double(s)};
      int d2=x*x+y*y, sum=(r+s)*(r+s), diff=(r-s)*(r-s);
      int count=!d2 || d2<diff ? 0 : d2==diff ? 1 :
        d2<sum ? 2 : d2==sum ? 3 : 4;
      auto ls=tangent(a,b);
      require((int)ls.size()==count,"common tangent exact counts");
      for (auto l:ls) {
        P u=l.pb-l.pa;
        require(u.abs()>0,"common tangent nonzero direction");
        near(abs(u^(a.c-l.pa))/u.abs(),r,1e-12,"first tangent radius");
        near(abs(u^(b.c-l.pa))/u.abs(),s,1e-12,"second tangent radius");
      }
      int intersections=!d2 || d2>sum || d2<diff ? 0 :
        d2==sum || d2==diff ? 1 : 2;
      auto ps=Intersect(a,b);
      require((int)ps.size()==intersections,"circle exact counts");
      for (auto p:ps) {
        near((p-a.c).abs(),r,1e-12,"first intersection radius");
        near((p-b.c).abs(),s,1e-12,"second intersection radius");
      }
      P p{double(x),double(y)};
      auto ts=tangent(a,p);
      require((int)ts.size()==(d2<r*r ? 0 : d2==r*r ? 1 : 2),
        "point tangent exact counts");
      for (auto l:ts) {
        P u=l.pb-l.pa;
        near(abs(u^(a.c-l.pa))/u.abs(),r,1e-12,"point tangent radius");
        near(u^(p-l.pa),0,1e-12,"point lies on tangent");
      }
    }
}

// An inside start can admit an earlier root in the endpoint eps band.
// The exit is the LAST root, even though the segment returns both roots.
void endpoint_regression() {
  for (double r : {.5,1.,1000.}) for (double f : {.25,.5,.75})
    for (double delta : {eps/8,eps/2,2*eps}) {
      double y=r*f, x=sqrt(r*r-y*y);
      P a{-x+delta,y}, b{2*r,y};
      ld want=(ld)y*(x-a.x)/2 +
        (ld)r*r*(atan2l(y,x)-atan2l(y,b.x))/2;
      for (double angle : {0.,.3,1.1,pi/2}) {
        P u=a.rot(angle),v=b.rot(angle);
        require(u.abs()<r,"regression starts strictly inside");
        auto roots=CircleCrossLine(u,v,{0,0},r);
        require(!roots.empty(),"regression has a real exit root");
        ld tol=1e-9L*max(1.,r*r);
        near(AreaOfCircleTriangle(u,v,r),want,tol,
          "inside endpoint selects exit, not tolerated entry");
        near(AreaOfCircleTriangle(v,u,r),want,tol,
          "reversed inside endpoint selects entry");
        vector<P> tri{{0,0},u,v};
        near(AreaOfCircleTriangle(tri,r),want,tol,
          "vector triangle near boundary");
        reverse(tri.begin(),tri.end());
        near(AreaOfCircleTriangle(tri,r),want,tol,
          "vector triangle near boundary CW/CCW");
      }
    }
}
void distance_regression() {
  // A split vertical edge must not merge with the opposite segment edge.
  for (double scale : {1.,2.,16.,1e6}) {
    vector<Point> a{{-1,1},{0,-1},{1,-1},{1,0},{1,1}},
      b{{-1,0},{-1,2}};
    for (auto &p:a) p=p*scale;
    for (auto &p:b) p=p*scale;
    for (int dir=0;dir<4;++dir) {
      for (int k=0;k<(int)a.size();++k) {
        near(TwoConvexHullMinDist(a.data(),b.data(),a.size(),b.size()),
          0,0,"split edge touching antiparallel segment");
        near(TwoConvexHullMinDist(b.data(),a.data(),b.size(),a.size()),
          0,0,"split edge touching with argument order reversed");
        rotate(a.begin(),a.begin()+1,a.end());
      }
      if (dir&1) reverse(a.begin(),a.end());
      else reverse(b.begin(),b.end());
    }
  }
}
int main(int argc,char **argv) {
  string mode=argc>1 ? argv[1] : "all";
  if (mode=="all" || mode=="audit") {
    primitives(); halfplanes(); polygons(); triangles_and_sweep();
    circles();
  }
  if (mode=="all" || mode=="endpoint") endpoint_regression();
  if (mode=="all" || mode=="distance") distance_regression();
  if (mode=="sweep") triangles_and_sweep();
  if (mode=="circles") circles();
  cout << "round8 " << mode << " PASS\n";
}
