#include <bits/stdc++.h>
using namespace std;
struct Point { double x, y; };
#include "../../codebook/8_Geometry/minDistOfTwoConvex.cpp"
static void require(bool ok, string msg) {
  if (!ok) { cerr << "FAIL " << msg << '\n'; exit(1); }
}
long double cross(Point a, Point b, Point c) {
  return ((long double)b.x-a.x)*((long double)c.y-a.y)
    - ((long double)b.y-a.y)*((long double)c.x-a.x);
}
vector<Point> hull(vector<Point> p) {
  sort(p.begin(), p.end(), [](Point a, Point b) {
    return tie(a.x,a.y) < tie(b.x,b.y);
  });
  p.erase(unique(p.begin(), p.end(), [](Point a, Point b) {
    return a.x==b.x && a.y==b.y;
  }), p.end());
  if (p.size()<2) return p;
  vector<Point> h;
  for (auto q:p) {
    while (h.size()>1 && cross(h[h.size()-2],h.back(),q)<=0)
      h.pop_back();
    h.push_back(q);
  }
  int k=h.size();
  for (int i=(int)p.size()-2;i>=0;--i) {
    while ((int)h.size()>k && cross(h[h.size()-2],h.back(),p[i])<=0)
      h.pop_back();
    h.push_back(p[i]);
  }
  h.pop_back(); return h;
}
long double segment_dist(Point p, Point a, Point b) {
  long double x=(long double)b.x-a.x, y=(long double)b.y-a.y;
  long double t=x*x+y*y;
  t=t?clamp((((long double)p.x-a.x)*x+
    ((long double)p.y-a.y)*y)/t,0.L,1.L):0;
  return hypotl(((long double)p.x-a.x)-t*x,
                ((long double)p.y-a.y)-t*y);
}
bool contains(const vector<Point>& a, Point p) {
  if (a.size()==1) return a[0].x==p.x && a[0].y==p.y;
  long double area=0;
  for (int i=1;i+1<(int)a.size();++i)
    area+=cross(a[0],a[i],a[i+1]);
  if (area==0) {
    auto less=[](Point u,Point v) {
      return tie(u.x,u.y)<tie(v.x,v.y);
    };
    auto [l,r]=minmax_element(a.begin(),a.end(),less);
    return cross(*l,*r,p)==0 && min(l->x,r->x)<=p.x &&
      p.x<=max(l->x,r->x) && min(l->y,r->y)<=p.y &&
      p.y<=max(l->y,r->y);
  }
  bool pos=false, neg=false;
  for (int i=0;i<(int)a.size();++i) {
    auto z=cross(a[i],a[(i+1)%a.size()],p);
    pos|=z>0; neg|=z<0;
  }
  return !(pos && neg);
}
long double oracle(const vector<Point>& a,const vector<Point>& b) {
  if (contains(a,b[0]) || contains(b,a[0])) return 0;
  long double ans=1e100L;
  for (int i=0;i<(int)a.size();++i)
    for (int j=0;j<(int)b.size();++j) {
      Point u=a[i],v=a[(i+1)%a.size()];
      Point s=b[j],t=b[(j+1)%b.size()];
      auto c1=cross(u,v,s),c2=cross(u,v,t);
      auto c3=cross(s,t,u),c4=cross(s,t,v);
      auto on=[](Point u,Point v,Point p) {
        return cross(u,v,p)==0 && min(u.x,v.x)<=p.x &&
          p.x<=max(u.x,v.x) && min(u.y,v.y)<=p.y &&
          p.y<=max(u.y,v.y);
      };
      if (on(u,v,s)||on(u,v,t)||on(s,t,u)||on(s,t,v)||
          (((c1>0)!=(c2>0)) && ((c3>0)!=(c4>0)) &&
           c1!=0 && c2!=0 && c3!=0 && c4!=0)) return 0;
      ans=min({ans,segment_dist(u,s,t),segment_dist(s,u,v)});
    }
  return ans;
}
void check(vector<Point> a, vector<Point> b) {
  long double want=oracle(a,b);
  auto original_a=a,original_b=b;
  for (int dir=0;dir<4;++dir) {
    double got=TwoConvexHullMinDist(a.data(),b.data(),a.size(),b.size());
    if (!(fabsl(got-want)<1e-8L*max(1.L,want))) {
      cerr << setprecision(20) << "got=" << got << " want=" << want << '\n';
      for (auto p:a) cerr << "A " << p.x << ' ' << p.y << '\n';
      for (auto p:b) cerr << "B " << p.x << ' ' << p.y << '\n';
      require(false,"distance oracle");
    }
    rotate(a.begin(),a.begin()+1,a.end());
    rotate(b.begin(),b.begin()+1,b.end());
    if (dir&1) reverse(a.begin(),a.end());
    else reverse(b.begin(),b.end());
  }
  a=original_a;b=original_b;
  TwoConvexHullMinDist(a.data(),b.data(),a.size(),b.size());
  for (int i=0;i<(int)a.size();++i)
    require(a[i].x==original_a[i].x && a[i].y==original_a[i].y,
      "first polygon unchanged");
  for (int i=0;i<(int)b.size();++i)
    require(b[i].x==original_b[i].x && b[i].y==original_b[i].y,
      "second polygon unchanged");
}
int main(int argc,char**) {
  mt19937 rng(378871);
  vector<Point> square={{0,0},{4,0},{4,4},{0,4}};
  check(square,square);
  check(square,{{1,1},{2,1},{2,2},{1,2}});
  check(square,{{4,0},{8,0},{8,4},{4,4}});
  check(square,{{5,5},{9,5},{9,9},{5,9}});
  // Exhaust pairs of all nonempty 3x2 lattice subsets, including points
  // and segments. Dyadic subdivision introduces zero and parallel edges.
  for (double scale : {0.125, 4096.0}) {
    vector<vector<Point>> polygons;
    for (int mask=1;mask<64;++mask) {
      vector<Point> p;
      for (int i=0;i<6;++i) if (mask>>i&1)
        p.push_back({1073741824.0+(i%3-1)*scale,
                     -1073741824.0+(i/3)*scale});
      auto h=hull(p); p.clear();
      for (int i=0;i<(int)h.size();++i) {
        Point a=h[i],b=h[(i+1)%h.size()];
        p.push_back(a); p.push_back(a);
        p.push_back({(a.x+b.x)/2,(a.y+b.y)/2});
      }
      p.push_back(p[0]); polygons.push_back(p);
    }
    for (auto a:polygons) for (auto b:polygons) {
      check(a,b);
      for (auto &p:b) p.x+=4*scale, p.y+=scale;
      check(a,b); // Separated shapes and shared parallel directions.
    }
  }
  for (int tc=0;tc<10000;++tc) {
    vector<Point> a,b;
    for (int i=0;i<20;++i) {
      a.push_back({double(int(rng()%101)-50),
                   double(int(rng()%101)-50)});
      b.push_back({double(int(rng()%101)-50+int(tc%3)*60),
                   double(int(rng()%101)-50)});
    }
    a=hull(a);b=hull(b);
    if (tc%2==0) {
      auto add=[](vector<Point> p) {
        vector<Point> q;
        for (int i=0;i<(int)p.size();++i) {
          Point a=p[i],b=p[(i+1)%p.size()];
          q.push_back(a);q.push_back(a);
          q.push_back({(a.x+b.x)/2,(a.y+b.y)/2});
        }
        q.push_back(q[0]);return q;
      };
      a=add(a);b=add(b);
    }
    check(a,b);
  }
  if (argc>1) {
    check({{0,0}},{{3,4}});
    check({{0,0},{1,0}},{{3,0},{4,0}});
    check({{0,0},{2,0}},{{1,-1},{1,1}});
    check({{0,0},{1,0},{2,0}},{{4,0},{5,0},{6,0}});
    check(square,{{10,10}});
    for (int tc=0;tc<3000;++tc) {
      vector<Point> a,b;
      for (int i=0;i<1+tc%4;++i)
        a.push_back({double(int(rng()%21)-10),
                     double(int(rng()%21)-10)});
      for (int i=0;i<1+tc%3;++i)
        b.push_back({double(int(rng()%21)-10),
                     double(int(rng()%21)-10)});
      check(hull(a),hull(b));
    }
  }
  // Linear implementation stress; baseline uses only the small oracle.
  if (argc>1) {
    int n=200000;
    vector<Point> a(n),b(n);
    for (int i=0;i<n;++i) {
      double t=2*acos(-1.)*i/n;
      a[i]={cos(t),sin(t)};b[i]={4+cos(t),sin(t)};
    }
    require(fabs(TwoConvexHullMinDist(a.data(),b.data(),n,n)-2)<1e-8,
      "200000-vertex distance stress");
  }
  cout << "convex distance PASS: 10000 hull pairs, CW/CCW, shifts, "
       << "duplicates, collinear edges, containment and touching\n";
}
