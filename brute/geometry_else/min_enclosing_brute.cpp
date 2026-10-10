#include <bits/stdc++.h>
using namespace std;
#define SZ(a) ((int)a.size())
#define ALL(v) v.begin(), v.end()
struct pt {
  double x=0, y=0;
  pt(double x=0, double y=0): x(x), y(y) {}
  pt operator+(pt b) const { return {x+b.x,y+b.y}; }
  pt operator-(pt b) const { return {x-b.x,y-b.y}; }
  pt operator/(double k) const { return {x/k,y/k}; }
  double operator^(pt b) const { return x*b.y-y*b.x; }
};
long long norm_calls=0;
double norm2(pt a) { ++norm_calls; return a.x*a.x+a.y*a.y; }
struct circle { pt c; double r; };
#include "../../codebook/8_Geometry/MinimumEnclosingCircle.cpp"
using ld = long double;
struct LP { ld x, y; };
ld distance(LP a, LP b) { return hypotl(a.x-b.x,a.y-b.y); }
void check(vector<pt> p) {
  auto original=p;
  vector<LP> a;
  pt origin=p.empty()?pt():p[0];
  for (pt q:p) a.push_back({ld(q.x)-origin.x,ld(q.y)-origin.y});
  // Normalize first: long double is also 64-bit on some platforms.
  ld scale=0;
  for (LP q:a) scale=max(scale,max(fabsl(q.x),fabsl(q.y)));
  if (scale==0) scale=1;
  for (LP &q:a) q.x/=scale,q.y/=scale;
  ld best=numeric_limits<ld>::infinity();
  auto offer=[&](LP c, ld r) {
    for (LP q:a) if (distance(q,c)>r+1e-15L*r) return;
    best=min(best,r);
  };
  if (a.empty()) best=0;
  for (int i=0;i<SZ(a);++i) {
    offer(a[i],0);
    for (int j=0;j<i;++j) {
      LP c{(a[i].x+a[j].x)/2,(a[i].y+a[j].y)/2};
      offer(c,max(distance(c,a[i]),distance(c,a[j])));
      for (int k=0;k<j;++k) {
        ld x=a[j].x-a[i].x, y=a[j].y-a[i].y;
        ld u=a[k].x-a[i].x, v=a[k].y-a[i].y, d=2*(x*v-y*u);
        if (!d) continue;
        ld b=x*x+y*y, c=u*u+v*v;
        LP z{a[i].x+(b*v-c*y)/d,a[i].y+(c*x-b*u)/d};
        offer(z,max({distance(z,a[i]),distance(z,a[j]),
                     distance(z,a[k])}));
      }
    }
  }
  if (!isfinite(best)) { cerr<<"FAIL oracle has no support\n"; exit(1); }
  best*=scale;
  auto got=min_enclosing(p);
  ld tolerance=3e-12L*best+8*numeric_limits<double>::epsilon()*
    max(fabsl(origin.x),fabsl(origin.y));
  bool ok=isfinite(got.r)&&isfinite(got.c.x)&&isfinite(got.c.y)&&
    fabsl(got.r-best)<=tolerance;
  for (pt q:original)
    ok &= hypotl(ld(q.x)-got.c.x,ld(q.y)-got.c.y)<=got.r+tolerance;
  if (!ok) {
    cerr<<setprecision(17)<<"FAIL MEC radius="<<got.r<<" want="<<best
        <<" center="<<got.c.x<<','<<got.c.y<<'\n';
    for (pt q:original) cerr<<'('<<q.x<<','<<q.y<<") ";
    cerr<<'\n'; exit(1);
  }
  sort(ALL(p),[](pt a,pt b){return tie(a.x,a.y)<tie(b.x,b.y);});
  sort(ALL(original),[](pt a,pt b){return tie(a.x,a.y)<tie(b.x,b.y);});
  for (int i=0;i<SZ(p);++i) if (p[i].x!=original[i].x ||
      p[i].y!=original[i].y) { cerr<<"FAIL permutation\n"; exit(1); }
  if (a.empty() && (got.c.x || got.c.y || got.r)) exit(1);
}
int main(int argc,char **argv) {
  bool extreme=argc>1 && string(argv[1])=="extreme";
  if (argc>1 && string(argv[1])=="regression") {
    for (int i=0;i<1000;++i)
      check({{-5,1},{9,1},{-9,0},{-3,6},{-6,-9},{2,8},
             {1,4},{-6,2},{-9,-4},{3,-3},{2,8},{-7,-4}});
    cout<<"MEC PASS: 1000 repeated roundoff regression cases\n"; return 0;
  }
  for (auto p:vector<vector<pt>>{{},{{7,-2}},{{7,-2},{7,-2}},
      {{-5,0},{5,0},{0,0}},{{0,0},{1,0},{0,1}},
      {{-1,0},{1,0},{0,1e-12}},{{-1,0},{1,0},{0,1e12}},
      {{0,0},{2,0},{2,2},{0,2},{0,0},{2,2}}}) check(p);
  mt19937_64 rng(516731);
  for (int tc=0;tc<15000;++tc) {
    vector<pt> p;
    double s=ldexp(1.0,200*(tc%3-1));
    for (int i=0,n=rng()%13;i<n;++i) {
      double x=int(rng()%21)-10, y=int(rng()%21)-10;
      if (tc%5==0) y=3*x; // Exactly collinear, incl. duplicates.
      p.push_back({s*x,s*y});
    }
    check(p);
  }
  for (int tc=0;tc<3000;++tc) {
    vector<pt> p;
    double shift=tc%2?1e12:0;
    for (int i=0;i<12;++i) {
      double x=double(rng()%1000000)/997;
      p.push_back({shift+x,shift+3*x});
    }
    check(p);
  }
  for (int tc=0;tc<3000;++tc) {
    vector<pt> p;
    double s=ldexp(1.0,200*(tc%3-1));
    for (int i=0;i<10;++i) {
      double x=double(rng()%1000000)/997;
      double delta=(int(rng()%3)-1)*1e-11;
      p.push_back({x*s,(3*x+delta)*s});
    }
    check(p);
  }
  if (extreme) for (int e:{-400,400}) {
    double s=ldexp(1.0,e);
    check({{0,0},{2*s,0},{0,2*s}});
    check({{-s,0},{s,0},{0,2*s}});
    for (bool reverse:{false,true}) {
      pt c=reverse?center({s,0},{-s,0},{0,2*s}):
                   center({-s,0},{s,0},{0,2*s});
      if (abs(c.x)>s*1e-12 || abs(c.y/s-0.75)>1e-12) {
        cerr<<"FAIL standalone extreme circumcenter\n"; return 1;
      }
    }
  }
  vector<pt> line;
  for (int i=0;i<100000;++i) line.push_back({double(i),3.0*i});
  norm_calls=0; auto stress=min_enclosing(line);
  if (abs(stress.r-hypot(99999,299997)/2)>1e-6 ||
      norm_calls>128LL*SZ(line)) {
    cerr<<"FAIL MEC collinear stress/evaluation bound\n"; return 1;
  }
  cout<<"MEC PASS: empty, duplicates, collinear, 21000 oracle cases, "
      <<"100000-point stress"
      <<(extreme?", exponent +/-400":"")<<'\n';
}
