#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define pb push_back
#define SZ(a) ((int)(a).size())
namespace rational {
#include "../../codebook/6_Math/Fraction.cpp"
}
namespace rounding {
#include "../../codebook/6_Math/floor_ceil.cpp"
}
namespace determinant {
const int MAXN = 20;
ll P;
#include "../../codebook/6_Math/Determinant.cpp"
}
namespace ranks {
const int N = 1025;
#include "../../codebook/3_Data_Structure/BIT_kth.cpp"
}
namespace trees {
const int N = 256;
#include "../../codebook/3_Data_Structure/Dynamic_2D_Segment_Tree.cpp"
#include "../../codebook/3_Data_Structure/Heavy_light_Decomposition.cpp"
}
namespace flows {
const int N = 10;
const ll INF = 1LL << 60;
#include "../../codebook/4_Flow_Matching/MincostMaxflow.cpp"
}
namespace cuts {
#include "../../codebook/4_Flow_Matching/SW-mincut.cpp"
}
namespace geometry {
const double eps = 1e-9, pi = acos(-1.);
int sign(double x) { return x < -eps ? -1 : x > eps; }
#include "../../codebook/8_Geometry/_basic.cpp"
struct C { P c; double r; };
#include "../../codebook/8_Geometry/CircleTangent.cpp"
vector<P> p;
double dist(P a, P b) { return (a-b).abs(); }
#include "../../codebook/8_Geometry/ClosestPair.cpp"
}

void numeric_audit() {
  using rational::fraction;
  for (ll x : {0LL, 1LL, -1LL, LLONG_MAX, -LLONG_MAX}) {
    for (ll d : {1LL, -1LL, LLONG_MAX, -LLONG_MAX}) {
      fraction f(x, d), z(0, d);
      assert(f.d > 0 && gcd(f.n, f.d) == 1);
      assert((__int128)f.n*d == (__int128)x*f.d);
      assert(z.n == 0 && z.d == 1);
      assert((-f).n == -f.n && (-f).d == f.d);
      auto q = f / fraction(-1);
      assert(q.n == -f.n && q.d == f.d);
    }
  }
  // Raw products remain within ll, close to sqrt(LLONG_MAX).
  ll b = 3037000499LL;
  for (ll a : {b, b-1, -b, 0LL}) {
    fraction x(a,b), y(-a,b);
    auto z = x+y, m = x*y;
    assert(z.n == 0 && z.d == 1);
    assert((__int128)m.n*x.d*y.d == (__int128)x.n*y.n*m.d);
  }
  mt19937_64 rng(11001);
  for (int tc=0;tc<50000;++tc) {
    int a=int(rng()), b=int(rng());
    if (!b || (a==INT_MIN && b==-1)) continue;
    ll f=rounding::floor(a,b), c=rounding::ceil(a,b);
    if (b>0) assert(f*b<=a && (f+1)*b>a && c*b>=a && (c-1)*b<a);
    else assert(f*b>=a && (f+1)*b<a && c*b<=a && (c-1)*b>a);
  }
  using namespace determinant;
  // Dense rank-one matrices with a diagonal shift have determinant
  // d^(n-1)*(d+n*a), including composite moduli and d=0.
  for (ll mod : {1LL, 8LL, 12LL, LLONG_MAX-1, LLONG_MAX})
    for (int n=0;n<=MAXN;++n) for (ll d : {0LL,1LL,2LL}) {
      P=mod; ll a=P-1, want=1%P;
      if (n) {
        for (int i=1;i<n;++i) want=(__int128)want*d%P;
        want=(__int128)want*(((__int128)n*a+d)%P)%P;
      }
      Matrix mat{}; mat.n=mat.m=n;
      for (int i=0;i<n;++i) for (int j=0;j<n;++j)
        mat.M[i][j]=((__int128)a+(i==j ? d : 0))%P;
      assert(mat.det()==want);
    }
}

void rank_and_tree_audit() {
  // Move a single INT_MAX-sized atom through every Fenwick boundary.
  using namespace ranks;
  fill_n(bit,N+1,0); int last=0;
  auto update=[](int x,int delta) {
    for (;x<=N;x+=x&-x) bit[x]+=delta;
  };
  for (int i=1;i<=N;++i) {
    if (last) update(last,-INT_MAX);
    update(i,INT_MAX); last=i;
    for (int k : {1,2,INT_MAX/2,INT_MAX-1,INT_MAX})
      assert(query_kth(k)==i);
  }
  using trees::Heavy_light_Decomposition;
  Heavy_light_Decomposition h;
  mt19937 rng(11002);
  // Random labels prevent the root/heavy chain matching insertion order.
  for (int tc=0;tc<30;++tc) {
    int n=255;
    vector<int> label(n); iota(label.begin(),label.end(),1);
    shuffle(label.begin(),label.end(),rng);
    vector<vector<int>> g(n+1);
    h.init(n);
    for (int i=1;i<n;++i) {
      int a=label[i], b=label[(i-1)/2];
      g[a].pb(b),g[b].pb(a); h.add_edge(a,b);
    }
    vector<int> v(n+1);
    for (int i=1;i<=n;++i) h.val[i]=v[i]=(int)(rng()%2001)-1000;
    h.build();
    for (int step=0;step<8;++step) {
      int l=rng()%n, r=l+1+rng()%(n-l), delta=int(rng()%11)-5;
      h.seg.update(l,r,delta);
      for (int u=1;u<=n;++u) if (l<=h.pl[u] && h.pl[u]<r) v[u]+=delta;
      for (int s=1;s<=n;++s) {
        auto walk=[&](auto &&self,int u,int parent,int sum)->void {
          sum+=v[u]; assert(h.query(s,u)==sum);
          for (int x:g[u]) if (x!=parent) self(self,x,u,sum);
        };
        walk(walk,s,0,0);
      }
    }
  }
}

void graph_audit() {
  using flows::MinCostMaxFlow;
  for (bool neg : {false,true}) {
    MinCostMaxFlow mf; mf.init(0); mf.init(3);
    // A single bottleneck can exceed INF without overflowing ll cost.
    mf.add_edge(0,0,LLONG_MAX,0);
    mf.add_edge(0,1,LLONG_MAX,-1);
    mf.add_edge(1,2,LLONG_MAX,0);
    ll f,c;
    mf.solve(0,2,f,c,neg); assert(f==LLONG_MAX && c==-LLONG_MAX);
    mf.solve(0,2,f,c,neg); assert(f==0 && c==0);
    mf.solve(2,0,f,c,neg); assert(f==LLONG_MAX && c==LLONG_MAX);
    mf.solve(0,0,f,c,neg); assert(f==0 && c==0);
    // Reallocation after solve must not leave a live predecessor pointer.
    for (int i=0;i<100;++i) mf.add_edge(0,2,0,0);
    mf.solve(0,2,f,c,neg); assert(f==LLONG_MAX && c==-LLONG_MAX);
  }
  auto &sw=cuts::sw;
  // Total edge weight may exceed int, while every cut remains in range.
  for (int n=2;n<=9;++n) {
    int largest=n*n/4, w=INT_MAX/largest;
    for (int isolated : {0,1}) {
      sw.init(n);
      for (int u=0;u<n;++u) sw.addEdge(u,u,INT_MAX);
      for (int u=isolated;u<n;++u) for (int v=u+1;v<n;++v)
        sw.addEdge(u,v,w/2),sw.addEdge(v,u,w-w/2);
      assert(sw.solve(n)==(isolated ? 0 : (n-1)*w));
    }
  }
}

void geometry_audit() {
  using namespace geometry;
  // Translation, unequal radii, exchanged arguments and exact contacts.
  for (double scale : {1.,8.,1024.}) for (int r=1;r<=4;++r)
    for (int s=1;s<=4;++s) for (int x=0;x<=9;++x)
      for (int y=0;y<=9;++y) {
        C a{{13*scale,-17*scale},r*scale};
        C b{{(13+x)*scale,(-17+y)*scale},s*scale};
        int d2=x*x+y*y, lo=(r-s)*(r-s), hi=(r+s)*(r+s);
        int count=!d2 || d2<lo ? 0 : d2==lo ? 1 :
          d2<hi ? 2 : d2==hi ? 3 : 4;
        for (auto cs : {pair{a,b},pair{b,a}}) {
          auto lines=tangent(cs.first,cs.second);
          assert((int)lines.size()==count);
          for (L l:lines) {
            P u=l.pb-l.pa; assert(u.abs()>0);
            for (C c : {a,b})
              assert(abs(abs(u^(c.c-l.pa))/u.abs()-c.r)<1e-9*scale);
          }
        }
        auto lines=tangent(a,b.c);
        int want=d2<r*r ? 0 : d2==r*r ? 1 : 2;
        assert((int)lines.size()==want);
        for (L l:lines) {
          P u=l.pb-l.pa; assert(u.abs()>0);
          assert(abs(u^(b.c-l.pa))<1e-8*scale*scale);
          assert(abs(abs(u^(a.c-l.pa))/u.abs()-a.r)<1e-9*scale);
        }
      }
  mt19937 rng(11003);
  for (int tc=0;tc<2000;++tc) {
    p.clear(); int n=2+rng()%60;
    for (int i=0;i<n;++i) p.emplace_back(i/6,int(rng()%31)-15);
    // Large representable translations do not change pair distances.
    for (P &a:p) a.x+=1099511627776.,a.y-=1099511627776.;
    int l=rng()%(n-1), r=l+1+rng()%(n-l-1);
    double want=INFINITY;
    for (int i=l;i<=r;++i) for (int j=i+1;j<=r;++j)
      want=min(want,hypot(p[i].x-p[j].x,p[i].y-p[j].y));
    auto saved=p;
    assert(closest_pair(l,r)==want);
    for (int i=0;i<n;++i) assert(p[i].x==saved[i].x && p[i].y==saved[i].y);
  }
  assert(closest_pair(1,0)==1e9 && closest_pair(0,0)==1e9);
}

int main() {
  numeric_audit(); rank_and_tree_audit(); graph_audit(); geometry_audit();
  cout << "PASS round11: arithmetic, ranks, trees, flow, cuts, geometry\n";
}
