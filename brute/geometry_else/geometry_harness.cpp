#include <bits/stdc++.h>
using namespace std;
using ld = long double;
const double eps = 1e-9, pi = acos(-1.0);
int sign(double x) { return fabs(x) < eps ? 0 : (x > 0 ? 1 : -1); }
double sq(double x) { return x * x; }

#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/area.cpp"
#include "../../codebook/8_Geometry/sectorArea.cpp"
#include "../../codebook/8_Geometry/circle.cpp"
#include "../../codebook/8_Geometry/CircleTangent.cpp"
#include "../../codebook/8_Geometry/circleOrArea.cpp"
#include "../../codebook/8_Geometry/convexHull.cpp"
#include "../../codebook/8_Geometry/pointInPolygon.cpp"
#include "../../codebook/8_Geometry/halfPlaneIntersect.cpp"

struct pt {
  double x=0,y=0;
  pt(double x=0,double y=0):x(x),y(y){}
  pt operator+(pt b)const{return {x+b.x,y+b.y};}
  pt operator-(pt b)const{return {x-b.x,y-b.y};}
  pt operator/(double k)const{return {x/k,y/k};}
  pt operator*(double k)const{return {x*k,y*k};}
  double operator^(pt b)const{return x*b.y-y*b.x;}
};
double norm2(pt a){return a.x*a.x+a.y*a.y;}
struct circle { pt c; double r; circle(pt c={},double r=0):c(c),r(r){} };
#include "../../codebook/8_Geometry/MinimumEnclosingCircle.cpp"
struct PP { double x,y; } p[64];
double dist(const PP&a,const PP&b){return hypot(a.x-b.x,a.y-b.y);}
#include "../../codebook/8_Geometry/ClosestPair.cpp"

struct Point { double x,y; Point(double x=0,double y=0):x(x),y(y){} Point operator+(Point b)const{return {x+b.x,y+b.y};} Point operator-(Point b)const{return {x-b.x,y-b.y};} Point operator*(double k)const{return {x*k,y*k};} Point operator/(double k)const{return {x/k,y/k};} };
double Cross(Point a, Point b){return a.x*b.y-a.y*b.x;}
double len(Point a){return hypot(a.x,a.y);}
#include "../../codebook/8_Geometry/triangleCenter.cpp"

static bool close(double a,double b,double e=1e-7){return fabs(a-b)<=e*max(1.0,max(fabs(a),fabs(b)));}
static void req(bool x,string s){if(!x){cerr<<"FAIL "<<s<<'\n';exit(1);}}

vector<P> brute_hull(vector<P> a){
  sort(a.begin(),a.end(),[](P x,P y){return x.x<y.x-eps||(fabs(x.x-y.x)<eps&&x.y<y.y);});
  a.erase(unique(a.begin(),a.end(),[](P x,P y){return close(x.x,y.x)&&close(x.y,y.y);}),a.end());
  if(a.size()<=1)return a; vector<P> h;
  for(auto q:a){while(h.size()>=2&&(((h.back()-h[h.size()-2])^(q-h.back()))<=eps))h.pop_back();h.push_back(q);}
  size_t z=h.size();
  for(int i=(int)a.size()-2;i>=0;--i){auto q=a[i];while(h.size()>z&&(((h.back()-h[h.size()-2])^(q-h.back()))<=eps))h.pop_back();h.push_back(q);} h.pop_back(); return h;
}
double brute_pair(vector<PP> a){double z=1e100;for(int i=0;i<(int)a.size();i++)for(int j=0;j<i;j++)z=min(z,dist(a[i],a[j]));return z;}

int main(){
  mt19937 rng(1234567);
  req(close(SectorArea(P(1,0),P(0,1),2),pi),"sectorArea");
  req(SegmentIntersect(P(0,0),P(2,2),P(0,2),P(2,0)),"basic.segment_intersect");
  vector<P> sqp={{0,0},{4,0},{4,4},{0,4}};
  req(pip(sqp,{2,2})==2&&pip(sqp,{0,2})==1&&pip(sqp,{5,2})==0,"pointInPolygon");
  for(int tc=0;tc<200;tc++){
    vector<P>a;for(int i=0;i<12;i++)a.push_back({(int)(rng()%17)-8,(int)(rng()%17)-8});
    auto x=convex(a),y=brute_hull(a); req(x.size()==y.size(),"convexHull size");
    vector<PP> q;for(int i=0;i<12;i++)q.push_back({(int)(rng()%31)-15,(int)(rng()%31)-15});
    for(int i=0;i<12;i++)p[i]=q[i];sort(p,p+12,[](PP a,PP b){return a.x<b.x||(a.x==b.x&&a.y<b.y);});
    req(close(closest_pair(0,11),brute_pair(q)),"closestPair");
    vector<pt> m;for(auto u:q)m.push_back({u.x,u.y});auto ce=min_enclosing(m);for(auto u:m)req(norm2({u.x-ce.c.x,u.y-ce.c.y})<=sq(ce.r)+1e-6,"minimumEnclosingCircle containment");
  }
  req(close(IntersectArea(C({0,0},1),C({0,0},2)),pi),"circle contained area");
  req(close(CircleUnionArea({C({0,0},1),C({4,0},1)}),2*pi),"circle union disjoint");
  auto internal_tangent = Intersect(C({0,0},1), C({1,0},2));
  req(internal_tangent.size()==1 && close(internal_tangent[0].x,-1) &&
      close(internal_tangent[0].y,0), "circle internal tangent dedup");
  auto line_tangent = CircleCrossLine({-2,1},{2,1},{0,0},1);
  req(line_tangent.size()==1 && close(line_tangent[0].x,0) &&
      close(line_tangent[0].y,1), "circle-line tangent dedup");
  req(Intersect(C({0,0},2),C({0,0},2)).empty(),
      "coincident circles use empty finite-list convention");
  req(CircleCrossLine({0,1},{0,1},{0,0},1).size()==1 &&
      CircleCrossLine({0,0},{0,0},{0,0},1).empty(),
      "degenerate circle-segment point convention");
  mt19937 circle_rng(423771);
  for (int tc=0; tc<5000; ++tc) {
    int ax=int(circle_rng()%13)-6, ay=int(circle_rng()%13)-6;
    int bx=int(circle_rng()%13)-6, by=int(circle_rng()%13)-6;
    int ra=1+circle_rng()%6, rb=1+circle_rng()%6;
    long double d=hypotl((long double)ax-bx,(long double)ay-by);
    if (d==0 && ra==rb) continue; // coincident circles have infinitely many intersections.
    int want = (d == ra+rb || d == abs(ra-rb)) ? 1 :
               (abs(ra-rb) < d && d < ra+rb) ? 2 : 0;
    auto got=Intersect(C({(double)ax,(double)ay},ra),
                       C({(double)bx,(double)by},rb));
    req((int)got.size()==want, "circle intersection count oracle");
    for (auto z: got) {
      req(close(hypot(z.x-ax,z.y-ay),ra,1e-6) &&
          close(hypot(z.x-bx,z.y-by),rb,1e-6),
          "circle intersection point oracle");
    }
  }
  for (int tc=0; tc<5000; ++tc) {
    P a{double(int(circle_rng()%17)-8),double(int(circle_rng()%17)-8)};
    P b{double(int(circle_rng()%17)-8),double(int(circle_rng()%17)-8)};
    P o{double(int(circle_rng()%9)-4),double(int(circle_rng()%9)-4)};
    double r=1+circle_rng()%5;
    double dx=b.x-a.x,dy=b.y-a.y,A=dx*dx+dy*dy;
    if (A==0) continue; // a line segment requires distinct endpoints.
    double B=2*(dx*(a.x-o.x)+dy*(a.y-o.y));
    double D=B*B-4*A*(sq(a.x-o.x)+sq(a.y-o.y)-sq(r));
    vector<double> roots;
    if (D>=-1e-9) {
      D=max(0.0,D);
      double u=(-B-sqrt(D))/(2*A),v=(-B+sqrt(D))/(2*A);
      if (u>=-1e-9&&u<=1+1e-9) roots.push_back(u);
      if (fabs(v-u)>1e-8&&v>=-1e-9&&v<=1+1e-9) roots.push_back(v);
    }
    auto got=CircleCrossLine(a,b,o,r);
    req(got.size()==roots.size(), "circle-line intersection count oracle");
    for (auto z: got)
      req(close(hypot(z.x-o.x,z.y-o.y),r,1e-6),
          "circle-line intersection point oracle");
  }
  req(tangent(C({0,0},1),P(2,0)).size()==2,"circle tangent from point");
  vector<L> hs={L({0,0},{1,0}),L({1,0},{1,1}),L({1,1},{0,1}),L({0,1},{0,0})};
  auto hp=HPI(hs);req(hp.size()==4,"halfPlaneIntersection square");
  req(close(TriangleMassCenter({0,0},{3,0},{0,3}).x,1),"triangle center");
  cout<<"geometry PASS: integer/random brute checks completed\n";
}
