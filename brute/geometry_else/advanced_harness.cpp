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

struct P {
  double x, y, z;
  P(double x=0, double y=0, double z=0): x(x), y(y), z(z) {}
  P operator-(P b) const { return {x-b.x, y-b.y, z-b.z}; }
  P operator^(P b) const {
    return {y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x};
  }
  double operator*(P b) const { return x*b.x+y*b.y+z*b.z; }
};
double abs(P a) { return sqrt(a.x*a.x+a.y*a.y+a.z*a.z); }

#include "../../codebook/8_Geometry/convex3D.cpp"

static void req(bool ok, const string &msg) {
  if (!ok) { cerr << "FAIL " << msg << '\n'; exit(1); }
}
static bool close(double a, double b) {
  return fabs(a-b) <= 1e-7 * max(1.0, max(fabs(a), fabs(b)));
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
  cout << "geometry advanced PASS: centroid, triangle, distance, sweep, 3D hull\n";
}
