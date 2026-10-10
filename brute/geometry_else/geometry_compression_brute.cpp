#include <bits/stdc++.h>
using namespace std;
const double eps=1e-9, pi=acos(-1.);
int sign(double x) { return fabs(x)<eps?0:(x>0?1:-1); }
double sq(double x) { return x*x; }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/sectorArea.cpp"
#include "../../codebook/8_Geometry/halfPlaneIntersect.cpp"
#include "../../codebook/8_Geometry/circle.cpp"
#include "../../codebook/8_Geometry/CircleTangent.cpp"
#include "../../codebook/8_Geometry/circleOrArea.cpp"
struct Point {
  double x,y;
  Point(double x=0,double y=0):x(x),y(y){}
  Point operator+(Point p) const { return {x+p.x,y+p.y}; }
  Point operator-(Point p) const { return {x-p.x,y-p.y}; }
  Point operator*(double t) const { return {x*t,y*t}; }
  Point operator/(double t) const { return {x/t,y/t}; }
};
double Cross(Point a,Point b) { return a.x*b.y-a.y*b.x; }
double len(Point p) { return hypot(p.x,p.y); }
#include "../../codebook/8_Geometry/triangleCenter.cpp"
#include "../../codebook/8_Geometry/center.cpp"
void require(bool ok,string msg) {
  if (!ok) { cerr << "FAIL " << msg << '\n';exit(1); }
}
bool close(double a,double b) {
  return abs(a-b)<1e-7*max({1.,abs(a),abs(b)});
}
int main(int argc,char**) {
  mt19937 rng(1963801);
  for (int tc=0;tc<3000;++tc) {
    C a{{double(int(rng()%21)-10),double(int(rng()%21)-10)},
        double(1+rng()%6)};
    C b{{double(int(rng()%21)-10),double(int(rng()%21)-10)},
        double(1+rng()%6)};
    double d=(a.c-b.c).abs();
    if (d==0) continue;
    int want=d<abs(a.r-b.r)?0:same(d,abs(a.r-b.r))?1:
      d<a.r+b.r?2:same(d,a.r+b.r)?3:4;
    auto ts=tangent(a,b);
    require((int)ts.size()==want,"common tangent count");
    for (auto l:ts) {
      P u=l.pb-l.pa;
      require(u.abs()>0,"nondegenerate tangent line");
      require(close(abs(l.a*a.c.x+l.b*a.c.y+l.c)/u.abs(),a.r),
        "first circle tangent distance");
      require(close(abs(l.a*b.c.x+l.b*b.c.y+l.c)/u.abs(),b.r),
        "second circle tangent distance");
    }
    Point p{double(int(rng()%21)-10),double(int(rng()%21)-10)};
    Point q{double(int(rng()%21)-10),double(int(rng()%21)-10)};
    Point r{double(int(rng()%21)-10),double(int(rng()%21)-10)};
    if (Cross(q-p,r-p)==0) continue;
    auto cc=TriangleCircumCenter(p,q,r);
    require(close(len(cc-p),len(cc-q)) && close(len(cc-p),len(cc-r)),
      "circumcenter equal distances");
    auto hc=TriangleOrthoCenter(p,q,r);
    require(close((hc.x-p.x)*(q.x-r.x)+(hc.y-p.y)*(q.y-r.y),0),
      "orthocenter altitude");
    auto ic=TriangleInnerCenter(p,q,r);
    require(close(abs(Cross(q-p,ic-p))/len(q-p),
                  abs(Cross(r-q,ic-q))/len(r-q)),
      "incenter equal side distances");
    int x=int(rng()%21)-10,y=int(rng()%21)-10;
    double w=1+rng()%10,h=1+rng()%10;
    vector<Point> rect={{double(x),double(y)}, {x+w,double(y)},
                       {x+w,y+h},{double(x),y+h}};
    if (tc%2) reverse(rect.begin(),rect.end());
    auto cen=BaryCenter(rect,99); // Legacy n argument is unused.
    require(close(cen.x,x+w/2) && close(cen.y,y+h/2),
      "CW/CCW centroid and unused n");
    vector<L> box={L({double(x),double(y)},{x+w,double(y)}),
      L({x+w,double(y)},{x+w,y+h}),
      L({x+w,y+h},{double(x),y+h}),
      L({double(x),y+h},{double(x),double(y)})};
    box.push_back(box[rng()%4]);
    shuffle(box.begin(),box.end(),rng);
    auto poly=HPI(box);double area=0;
    for (int i=0;i<(int)poly.size();++i)
      area+=poly[i]^poly[(i+1)%poly.size()];
    require(poly.size()==4 && close(abs(area)/2,w*h),
      "half-plane rectangle and repeated constraints");
  }
  require(close(CircleUnionArea({C({0,0},2),C({1,0},2)}),
    8*pi-IntersectArea(C({0,0},2),C({1,0},2))),"overlap circle union");
  require(close(CircleUnionArea({C({0,0},1),C({0,0},2)}),4*pi),
    "contained circle union");
  require(close(AreaOfCircleTriangle(vector<P>{{0,0},{1,0},{0,1}},2),
    .5),"circle contains triangle");
  require(close(AreaOfCircleTriangle(vector<P>{{-10,-10},
    {10,-10},{0,10}},1),pi),"triangle contains circle");
  if (argc>1) {
    vector<L> empty, one={L({0,0},{1,0})},
      parallel={L({0,0},{1,0}),L({0,1},{1,1})};
    require(HPI(empty).empty() && HPI(one).empty() &&
      HPI(parallel).empty(),"HPI insufficient directions");
    require(tangent(C({0,0},1),C({0,0},1)).empty(),
      "coincident circles have no finite tangent list");
  }
  cout << "geometry compression PASS: 3000 tangent/center/HPI cases, "
       << "circle union and triangle areas\n";
}
