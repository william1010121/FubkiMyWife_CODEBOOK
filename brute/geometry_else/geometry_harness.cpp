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
  req(tangent(C({0,0},1),P(2,0)).size()==2,"circle tangent from point");
  vector<L> hs={L({0,0},{1,0}),L({1,0},{1,1}),L({1,1},{0,1}),L({0,1},{0,0})};
  auto hp=HPI(hs);req(hp.size()==4,"halfPlaneIntersection square");
  req(close(TriangleMassCenter({0,0},{3,0},{0,3}).x,1),"triangle center");
  cout<<"geometry PASS: integer/random brute checks completed\n";
}
