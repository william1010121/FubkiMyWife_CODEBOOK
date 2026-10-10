#include <bits/stdc++.h>
using namespace std;
const double eps = 1e-9;

struct Point {
  double x, y;
  Point(double x=0, double y=0): x(x), y(y) {}
  Point operator+(Point b) const { return {x+b.x, y+b.y}; }
  Point operator-(Point b) const { return {x-b.x, y-b.y}; }
  Point operator*(double k) const { return {x*k, y*k}; }
  Point operator/(double k) const { return {x/k, y/k}; }
};
double Cross(Point a, Point b) { return a.x*b.y-a.y*b.x; }

#include "../../codebook/8_Geometry/center.cpp"
#include "../../codebook/8_Geometry/maxTriangleOfConvex.cpp"
#include "../../codebook/8_Geometry/minDistOfTwoConvex.cpp"
#include "../../codebook/8_Geometry/rotatingSweepLine.cpp"

long long dot_calls = 0;
struct P {
  double x, y, z;
  P(double x=0, double y=0, double z=0): x(x), y(y), z(z) {}
  P operator-(P b) const { return {x-b.x, y-b.y, z-b.z}; }
  P operator^(P b) const {
    return {y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x};
  }
  double operator*(P b) const {
    ++dot_calls; return x*b.x+y*b.y+z*b.z;
  }
};
double abs(P a) { return sqrt(a.x*a.x+a.y*a.y+a.z*a.z); }

#include "../../codebook/8_Geometry/convex3D.cpp"

static void req(bool ok, const string &msg) {
  if (!ok) { cerr << "FAIL " << msg << '\n'; exit(1); }
}
static bool close(double a, double b) {
  return fabs(a-b) <= 1e-7 * max(1.0, max(fabs(a), fabs(b)));
}
using Plane = array<long long,4>;
Plane plane(P a, P b, P c) {
  P v=(b-a)^(c-a);
  Plane key{llround(v.x),llround(v.y),llround(v.z),llround(v*a)};
  long long g=0;
  for (auto x:key) g=gcd(g,abs(x));
  if (!g) return {};
  for (auto &x:key) x/=g;
  for (auto x:key) if (x) {
    if (x<0) for (auto &y:key) y=-y;
    break;
  }
  return key;
}
void check_3d(convex3D &h) {
  set<Plane> want, got;
  for (int a=0;a<h.n;++a) for (int b=a+1;b<h.n;++b)
    for (int c=b+1;c<h.n;++c) {
      P v=(h.p[b]-h.p[a])^(h.p[c]-h.p[a]);
      bool pos=false, neg=false;
      for (int i=0;i<h.n;++i) {
        double s=v*(h.p[i]-h.p[a]);
        pos |= s>eps; neg |= s < -eps;
      }
      if (pos != neg) want.insert(plane(h.p[a],h.p[b],h.p[c]));
    }
  map<pair<int,int>,int> edges;
  set<int> vertices;
  for (int i=0;i<h.m;++i) {
    auto t=h.f[i];
    req(t.res, "3D compacted faces are active");
    got.insert(plane(h.p[t.a],h.p[t.b],h.p[t.c]));
    for (int j=0;j<h.n;++j)
      req(!h.on(h.f[i],h.p[j]), "3D random outward supporting face");
    for (auto e:{pair{t.a,t.b},pair{t.b,t.c},pair{t.c,t.a}}) {
      ++edges[e]; vertices.insert(e.first);
    }
  }
  req(want==got, "3D exhaustive supporting-plane oracle");
  req(h.faces()==int(want.size()), "3D geometric facet count oracle");
  for (auto [e,count]:edges)
    req(count==1 && edges[{e.second,e.first}]==1,
        "3D manifold directed edges");
  req(h.m==0 || h.m==2*int(vertices.size())-4, "3D Euler count");
}

int main() {
  vector<Point> square{{0,0},{4,0},{4,4},{0,4}};
  req(close(BaryCenter(square, 4).x, 2) && close(BaryCenter(square, 4).y, 2),
      "polygon centroid");

  mt19937 rng(712367);
  for (int n=3; n<=12; ++n) {
    vector<Point> poly(n);
    for (int i=0; i<n; ++i) {
      double a=2*acos(-1.0)*i/n;
      poly[i]={cos(a)*20, sin(a)*10};
    }
    vector<Point> p=poly;
    vector<int> res(n+1);
    for (int i=0;i<n;++i) res[i]=i;
    double want=0;
    for (int i=0;i<n;++i) for (int j=i+1;j<n;++j)
      for (int k=j+1;k<n;++k)
        want=max(want, fabs(Cross(p[j]-p[i],p[k]-p[i]))/2);
    req(close(ConvexHullMaxTriangleArea(p.data(),res.data(),n),want),
        "maximum triangle");
  }

  Point a[]={{0,0},{1,0},{1,1},{0,1}};
  Point b[]={{3,0},{4,0},{4,1},{3,1}};
  req(close(TwoConvexHullMinDist(a,b,4,4),2), "convex polygon distance");
  Point c[]={{.25,.25},{.75,.25},{.75,.75},{.25,.75}};
  req(close(TwoConvexHullMinDist(a,c,4,4),0), "nested convex polygons");
  Point d[]={{1,0},{2,0},{2,1},{1,1}};
  req(close(TwoConvexHullMinDist(a,d,4,4),0), "touching convex polygons");

  vector<pair<int,int>> ps={{0,3},{2,-1},{5,4},{8,0},{11,2}};
  vector<int> order=rotatingSweepLineOrder(ps), sorted=order;
  sort(sorted.begin(),sorted.end());
  vector<int> want_ids(order.size()); iota(want_ids.begin(),want_ids.end(),0);
  req(sorted==want_ids, "rotating sweep permutation");
  vector<pair<int,int>> extreme{{INT_MIN,0},{0,INT_MAX},{INT_MAX,-1}};
  auto extreme_order=rotatingSweepLineOrder(extreme);
  vector<int> extreme_want{2,1,0};
  req(extreme_order==extreme_want, "rotating sweep integer extremes");
  auto expected_sweep_order = [](const vector<pair<int,int>> &points) {
    vector<int> ids(points.size());
    iota(ids.begin(), ids.end(), 0);
    sort(ids.begin(), ids.end(), [&](int a, int b) {
      return points[a] < points[b];
    });
    reverse(ids.begin(), ids.end());
    return ids;
  };
  // In general position every pair crosses at a distinct direction, so a
  // half-turn reverses the lexicographic order.  This catches signed-x slope
  // comparator errors while avoiding simultaneous-event tie ambiguity.
  auto direction_key = [](pair<int,int> a, pair<int,int> b) {
    long long x = a.first - b.first, y = a.second - b.second;
    if (x < 0 || (x == 0 && y < 0)) x = -x, y = -y;
    long long g = std::gcd(std::llabs(x), std::llabs(y));
    x /= g; y /= g;
    return pair<long long,long long>{x, y};
  };
  mt19937 sweep_rng(819273);
  for (int tc=0; tc<3000; ++tc) {
    int n = 2 + sweep_rng() % 8;
    vector<pair<int,int>> q;
    set<pair<int,int>> used;
    set<int> used_x;
    while ((int)q.size() < n) {
      pair<int,int> z{int(sweep_rng()%101)-50, int(sweep_rng()%101)-50};
      if (!used_x.count(z.first) && used.insert(z).second) {
        used_x.insert(z.first);
        q.push_back(z);
      }
    }
    set<pair<long long,long long>> dirs;
    bool general = true;
    for (int i=0;i<n;++i) for (int j=i+1;j<n;++j)
      general &= dirs.insert(direction_key(q[i],q[j])).second;
    if (!general) { --tc; continue; }
    req(rotatingSweepLineOrder(q)==expected_sweep_order(q),
        "rotating sweep general-position reverse order");
  }
  rotatingSweepLine(ps);

  static convex3D tetra;
  tetra.n=4; tetra.m=0;
  tetra.p[0]={0,0,0}; tetra.p[1]={1,0,0};
  tetra.p[2]={0,1,0}; tetra.p[3]={0,0,1};
  tetra();
  req(tetra.faces()==4, "3D tetrahedron faces");

  static convex3D cube;
  cube.n=8; cube.m=0;
  int z=0;
  for (int x=0;x<=1;++x) for (int y=0;y<=1;++y)
    for (int w=0;w<=1;++w) cube.p[z++]={x,y,w};
  cube();
  req(cube.faces()==6, "3D cube coplanar faces");
  // Every subset of a skewed cube, shuffled to exercise simplex selection.
  static convex3D subset;
  mt19937 subset_rng(861257);
  for (int mask=0;mask<256;++mask) {
    vector<P> points;
    for (int i=0;i<8;++i) if (mask>>i&1) {
      int x=i&1,y=(i>>1)&1,z=i>>2;
      points.push_back({double(1024+16*(2*x+y)),
                        double(-2048+16*(y+z)),
                        double(4096+16*(z+x))});
    }
    for (int order=0;order<8;++order) {
      shuffle(points.begin(),points.end(),subset_rng);
      subset.n=points.size(); subset.m=0;
      copy(points.begin(),points.end(),subset.p); subset();
      check_3d(subset);
      int count=subset.faces();
      req(subset.faces()==count,"3D repeated facet query");
      for (int i=0;i<subset.m;++i) {
        auto t=subset.f[i];
        for (auto e:{pair{t.a,t.b},pair{t.b,t.c},pair{t.c,t.a}})
          req(subset.id[e.first][e.second]==i,"3D subset edge indices");
      }
    }
  }
  // Thin unimodular tetrahedron: exact nonzero volume is only 1/6.
  // All 24 insertion orders, with large, almost parallel integer vectors.
  vector<P> thin{{0,0,0},{1000000,999999,999999},
                 {1000001,1000000,1000000},{1000000,999999,1000000}};
  array<int,4> tetra_order{0,1,2,3};
  do {
    subset.n=4; subset.m=0;
    for (int i=0;i<4;++i) subset.p[i]=thin[tetra_order[i]];
    subset(); check_3d(subset);
    req(subset.m==4 && subset.faces()==4,"3D determinant-one tetrahedron");
  } while (next_permutation(tetra_order.begin(),tetra_order.end()));
  // A reflected/sheared pentagonal prism with points on edges, face
  // interiors, and in its interior. Exact integer predicates throughout.
  vector<pair<int,int>> polygon{{0,0},{6,0},{8,4},{4,8},{0,6}};
  vector<P> prism;
  auto skew = [](int x, int y, int z) {
    return P(1000000+x+2*y, -1000000+y+3*z, 1000000-z);
  };
  for (int z : {0,6}) for (int i=0;i<5;++i) {
    auto [x,y]=polygon[i]; auto [u,v]=polygon[(i+1)%5];
    prism.push_back(skew(x,y,z));
    prism.push_back(skew((x+u)/2,(y+v)/2,z));
  }
  for (int z : {0,3,6}) prism.push_back(skew(3,3,z));
  for (int order=0;order<48;++order) {
    if (order) shuffle(prism.begin(),prism.end(),subset_rng);
    subset.n=prism.size(); subset.m=0;
    copy(prism.begin(),prism.end(),subset.p); subset();
    check_3d(subset);
    req(subset.faces()==7,"3D reflected prism facets");
    for (int i=0;i<subset.m;++i) {
      auto t=subset.f[i];
      req(abs((subset.p[t.b]-subset.p[t.a])^
              (subset.p[t.c]-subset.p[t.a]))>eps,
          "3D prism triangle is nondegenerate");
      for (auto e:{pair{t.a,t.b},pair{t.b,t.c},pair{t.c,t.a}})
        req(subset.id[e.first][e.second]==i,"3D prism edge index");
    }
  }
  // Cyclic polytope: 2n-4 final faces but >8*1010 created faces.
  static convex3D curve;
  curve.n=200; curve.m=0;
  for (int i=0;i<curve.n;++i)
    curve.p[i]={double(i),double(i*i),double(i*i*i)};
  curve();
  req(curve.m==2*curve.n-4, "3D moment-curve face count");
  req(curve.faces()==curve.m, "3D moment-curve planes distinct");
  for (int i=0;i<curve.m;++i) for (int j=0;j<curve.n;++j)
    req(!curve.on(curve.f[i],curve.p[j]), "3D outward supporting face");
  curve.n=3; curve.m=0; curve();
  req(curve.m==0, "3D fewer than four points");
  curve.n=20; curve.m=0;
  for (int i=0;i<curve.n;++i) curve.p[i]={double(i),double(i*i),0};
  curve();
  req(curve.m==0, "3D coplanar input");
  for (int i=0;i<curve.n;++i) curve.p[i]={double(i),0,0};
  curve();
  req(curve.m==0, "3D collinear input");
  // Adversarial insertion orders: boundary grids and interior points.
  mt19937 boundary_rng(516729);
  vector<P> lattice;
  for (int x=-2;x<=2;++x) for (int y=-2;y<=2;++y)
    for (int z=-2;z<=2;++z) lattice.push_back({double(x),double(y),double(z)});
  for (int tc=0;tc<200;++tc) {
    shuffle(lattice.begin(),lattice.end(),boundary_rng);
    curve.n=lattice.size(); curve.m=0;
    copy(lattice.begin(),lattice.end(),curve.p); curve();
    req(curve.faces()==6,"3D lattice coplanar facets");
    map<pair<int,int>,int> edges; set<int> vertices;
    for (int i=0;i<curve.m;++i) {
      auto t=curve.f[i];
      req(t.res && abs((curve.p[t.b]-curve.p[t.a])^
          (curve.p[t.c]-curve.p[t.a]))>eps,"3D nonzero active triangle");
      for (int j=0;j<curve.n;++j)
        req(!curve.on(curve.f[i],curve.p[j]),"3D lattice supports");
      for (auto e:{pair{t.a,t.b},pair{t.b,t.c},pair{t.c,t.a}}) {
        ++edges[e]; vertices.insert(e.first);
        req(curve.id[e.first][e.second]==i,"3D compact edge indices");
      }
    }
    for (auto [e,count]:edges)
      req(count==1 && edges[{e.second,e.first}]==1,"3D lattice manifold");
    req(curve.m==2*int(vertices.size())-4,"3D lattice Euler count");
  }
  for (int n=0;n<4;++n) {
    curve.n=n; curve.m=0; curve();
    req(curve.m==0,"3D empty/small reused object");
  }
  // Full capacity: 200 hull vertices followed by 810 interior points.
  curve.n=convex3D::maxn; curve.m=0;
  for (int i=0;i<200;++i)
    curve.p[i]={double(i),double(i*i),double(i*i*i)};
  for (int i=200;i<curve.n;++i) {
    double w=0.4-0.1*(i-199)/1000.0;
    curve.p[i]={0.2*66+0.3*133+w*199,
                0.2*66*66+0.3*133*133+w*199*199,
                0.2*66*66*66+0.3*133*133*133+w*199*199*199};
  }
  dot_calls=0; curve();
  req(curve.m==396, "3D 1010-point stress hull");
  req(dot_calls<2LL*curve.n*curve.n, "3D stress evaluation bound");
  // Full-capacity strictly convex hull: facet grouping must be linear.
  curve.n=convex3D::maxn; curve.m=0;
  for (int i=0;i<curve.n;++i) {
    double z=1-2.0*(i+0.5)/curve.n, a=i*2.399963229728653;
    curve.p[i]={sqrt(1-z*z)*cos(a)*10000,
                sqrt(1-z*z)*sin(a)*10000,z*10000};
  }
  curve();
  req(curve.m==2*curve.n-4, "3D sphere Euler triangle count");
  dot_calls=0;
  req(curve.faces()==curve.m, "3D sphere distinct geometric facets");
  req(dot_calls<12LL*curve.m, "3D facet grouping evaluation bound");
  for (int tc=0;tc<1000;++tc) {
    set<array<int,3>> points;
    int n=4+rng()%13;
    while (int(points.size())<n)
      points.insert({int(rng()%11)-5,int(rng()%11)-5,int(rng()%11)-5});
    curve.n=n; curve.m=0; int j=0;
    for (auto a:points)
      curve.p[j++]={double(a[0]),double(a[1]),double(a[2])};
    curve(); check_3d(curve);
  }
  cout << "geometry advanced PASS: centroid, triangle, distance, sweep, 3D hull\n";
}
