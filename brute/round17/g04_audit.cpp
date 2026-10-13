#include "../../codebook/1_Basic/Default_code.cpp"

namespace flow {
constexpr int N = 16;
constexpr ll INF = 32;
#include "../../codebook/4_Flow_Matching/MincostMaxflow.cpp"
struct Oracle {
  struct Edge { int u, v; ll cap; };
  int n; vector<Edge> es;
  void add(int u, int v, ll cap) {
    es.pb({u,v,cap}); es.pb({v,u,0});
  }
  ll solve(int s, int t) {
    if (s == t) return 0;
    ll total = 0;
    for (;;) {
      vector<int> prev(n,-1), q{s}; prev[s] = -2;
      for (int k = 0; k < SZ(q); ++k)
        for (int i = 0; i < SZ(es); ++i)
          if (es[i].u == q[k] && es[i].cap && prev[es[i].v] == -1)
            prev[es[i].v] = i, q.pb(es[i].v);
      if (prev[t] == -1) return total;
      ll delta = LLONG_MAX;
      for (int v = t; v != s; v = es[prev[v]].u)
        delta = min(delta,es[prev[v]].cap);
      for (int v = t; v != s; v = es[prev[v]].u)
        es[prev[v]].cap -= delta, es[prev[v]^1].cap += delta;
      total += delta;
    }
  }
};
void check(bool stress) {
  // The zero-cost capacity may reach LLONG_MAX: all arithmetic fits.
  ll cap = stress ? LLONG_MAX : 320000;
  MinCostMaxFlow mf; mf.init(2); mf.add_edge(0,1,cap,0);
  for (bool neg : {false,true}) {
    ll f,c; mf.solve(0,1,f,c,neg); assert(f == cap && c == 0);
    mf.solve(0,1,f,c,!neg); assert(f == 0 && c == 0);
    mf.solve(1,0,f,c,neg); assert(f == cap && c == 0);
  }
  mf.init(0); mf.init(1); mf.add_edge(0,0,cap,0);
  ll f,c; mf.solve(0,0,f,c); assert(f == 0 && c == 0);
  mt19937 rng(1704);
  ll scale = stress ? 10000000000LL : 1;
  for (int tc = 0; tc < 300; ++tc) {
    int n = 1+rng()%8; mf.init(n); Oracle ref{n,{}};
    vector<int> h(n); for (int &x : h) x = int(rng()%9)-4;
    for (int stage = 0; stage < 8; ++stage) {
      for (int k = 0; k < 4; ++k) {
        int u = rng()%n, v = rng()%n;
        ll capacity = (rng()%101)*scale;
        mf.add_edge(u,v,capacity,h[v]-h[u]); ref.add(u,v,capacity);
      }
      int s = rng()%n, t = rng()%n;
      mf.solve(s,t,f,c,stage&1); ll want = ref.solve(s,t);
      assert(f == want && c == want*(h[t]-h[s]));
      for (int u = 0; u < n; ++u)
        for (int i = 0; i < SZ(mf.G[u]); ++i) {
          auto e = mf.G[u][i], r = mf.G[e.to][e.rev];
          assert(r.to == u && r.rev == i && e.flow == -r.flow);
          assert(e.flow <= e.cap);
        }
    }
  }
}
}

namespace sat {
#include "../../codebook/2_Graph/SCC.cpp"
#include "../../codebook/2_Graph/2SAT.cpp"
void check() {
  mt19937 rng(1705);
  for (int n : {0,1,2,7,64,257,2000})
    for (bool conflict : {false,true}) {
      if (!n && conflict) continue;
      vector<int> val(n), perm(n); iota(ALL(perm),0);
      shuffle(ALL(perm),rng); SAT s(n);
      vector<pii> clauses;
      for (int i = 0; i < n; ++i) {
        val[i] = rng()&1; int x = i+(val[i] ? 0 : n);
        clauses.pb({x,x}); clauses.pb({i,i+n});
      }
      for (int k = 1; k < n; ++k) {
        int a = perm[k-1]+(val[perm[k-1]] ? 0 : n);
        int b = perm[k]+(val[perm[k]] ? 0 : n);
        clauses.pb({s.rv(a),b}); clauses.pb({s.rv(b),a});
      }
      if (conflict) clauses.pb({s.rv(clauses[0].X),
                               s.rv(clauses[0].X)});
      shuffle(ALL(clauses),rng);
      for (auto [a,b] : clauses)
        for (int copy = 0; copy < 4; ++copy) s.add_clause(a,b);
      for (int repeat = 0; repeat < 3; ++repeat) {
        assert(s.solve() == !conflict);
        if (!conflict) for (int i = 0; i < n; ++i) {
          assert(s.istrue[i] == val[i]);
          assert(s.istrue[i+n] != s.istrue[i]);
        }
      }
    }
}
}

namespace geometry {
constexpr double eps = 1e-9;
int sign(double x) { return x < -eps ? -1 : x > eps; }
#include "../../codebook/8_Geometry/_basic.cpp"
struct I { ll x,y; };
__int128 cross(I a,I b,I c) {
  return (__int128)(b.x-a.x)*(c.y-a.y)-
         (__int128)(b.y-a.y)*(c.x-a.x);
}
int sg(__int128 x) { return (x>0)-(x<0); }
bool inside(I a,I b,I p) {
  return !cross(a,b,p) && min(a.x,b.x)<=p.x && p.x<=max(a.x,b.x)
    && min(a.y,b.y)<=p.y && p.y<=max(a.y,b.y);
}
bool intersect(I a,I b,I c,I d) {
  if (inside(a,b,c)||inside(a,b,d)||inside(c,d,a)||inside(c,d,b))
    return true;
  return sg(cross(a,b,c))*sg(cross(a,b,d))<0 &&
    sg(cross(c,d,a))*sg(cross(c,d,b))<0;
}
void check() {
  mt19937 rng(1706);
  for (int tc = 0; tc < 30000; ++tc) {
    array<I,4> a;
    for (auto &p : a) p = {int(rng()%2001)-1000,
                          int(rng()%2001)-1000};
    if (tc%3 == 0) a[1] = a[0];
    if (tc%5 == 0) a[3] = a[2];
    if (tc%7 == 0) for (auto &p : a) p.y = 3*p.x+5;
    if (tc%11 == 0) a[2] = a[1];
    for (double scale : {1.,1./1024}) {
      array<P,4> p;
      for (int i = 0; i < 4; ++i)
        p[i] = {(a[i].x+1048576)*scale,
                (a[i].y-1048576)*scale};
      assert(SegmentIntersect(p[0],p[1],p[2],p[3]) ==
             intersect(a[0],a[1],a[2],a[3]));
      L line(p[0],p[1]);
      assert(line.inside(p[2]) == inside(a[0],a[1],a[2]));
      if (a[0].x == a[1].x && a[0].y == a[1].y) continue;
      P q = line.project(p[2]), r = line.reflect(p[2]);
      assert(abs((p[2]-q)*(p[1]-p[0])) < 1e-6);
      assert((line.reflect(r)-p[2]).abs() < 1e-8);
      double t = line.get_ratio(p[2]);
      assert((q-(p[0]+(p[1]-p[0])*t)).abs() < 1e-8);
    }
  }
  assert(P().abs() == 0 && L().inside(P()));
  assert(abs(P(3,4).unit().abs()-1) < eps);
}
}

namespace primes {
#include "../../codebook/6_Math/PiCount.cpp"
void check() {
  constexpr int limit = 2000000;
  vector<bool> composite(limit+1); vector<int> pi(limit+1);
  for (int p = 2; p*p <= limit; ++p) if (!composite[p])
    for (int k = p*p; k <= limit; k += p) composite[k] = true;
  for (int n = 2; n <= limit; ++n) pi[n] = pi[n-1]+!composite[n];
  for (ll n : {LLONG_MIN,-1LL,0LL,1LL}) assert(PrimeCount(n) == 0);
  mt19937 rng(1707);
  for (int k = 0; k < 1000; ++k) {
    int n = rng()%(limit+1); assert(PrimeCount(n) == pi[n]);
  }
  for (ll p = 2; p <= 41; ++p) for (int power : {2,3,4}) {
    ll center = 1; for (int i = 0; i < power; ++i) center *= p;
    for (ll n = max(0LL,center-2); n <= min<ll>(limit,center+2); ++n)
      assert(PrimeCount(n) == pi[n]);
  }
}
}

namespace strings {
constexpr int kN = 257;
#include "../../codebook/5_String/MainLorentz.cpp"
void kmp(const string &s,const string &t) {
  ostringstream out;
  {
#define cout out
#include "../../codebook/5_String/KMP.cpp"
#undef cout
    for (int i = 0; i < SZ(t); ++i) {
      int want = 0;
      for (int l = 1; l <= i; ++l)
        if (t.compare(0,l,t,i+1-l,l) == 0) want = l;
      assert(nxt[i] == want);
    }
  }
  vector<int> got,want; istringstream in(out.str());
  for (int x; in >> x;) got.pb(x);
  for (int i = 0; i+SZ(t) <= SZ(s); ++i)
    if (s.compare(i,t.size(),t) == 0) want.pb(i+1);
  assert(got == want);
}
void lorentz(const string &s,int offset) {
  main_lorentz(s,offset); int n = SZ(s);
  vector<vector<int>> count(kN,vector<int>(n));
  for (int l = 0; l < kN; ++l) for (auto [a,b] : rep[l]) {
    assert(l && a <= b);
    for (ll p = a; p <= b; ++p) {
      ll local = p-offset;
      assert(0 <= local && local+2*l <= n);
      ++count[l][local];
    }
  }
  for (int l = 1; l < kN; ++l) for (int p = 0; p < n; ++p)
    assert(count[l][p] == (p+2*l <= n &&
           s.compare(p,l,s,p+l,l) == 0));
}
void check() {
  mt19937 rng(1708); string alphabet;
  for (int c = 0; c < 256; ++c) alphabet += char(c);
  vector<string> words{"",string(1,'\0'),alphabet,alphabet+alphabet};
  for (int period : {1,2,3,7,16,31,64,127,128}) {
    string unit = alphabet.substr(0,period), s;
    while (SZ(s) < 513) s += unit;
    s.resize(513); words.pb(s);
    for (int at : {0,period-1,period,256,512}) {
      string changed = s; changed[at] ^= char(255); words.pb(changed);
    }
  }
  for (const auto &s : words) {
    lorentz(s,INT_MIN); lorentz(s,INT_MAX-max(0,SZ(s)-1));
    for (int at : {0,SZ(s)/2,SZ(s)}) {
      string t = s.substr(at,129); kmp(s,t); kmp(t,s);
      if (!t.empty()) t.back() ^= char(255);
      kmp(s,t);
    }
    kmp(s,""); lorentz("",0);
  }
}
}

namespace queens {
#include "../../codebook/9_Else/NQueens.cpp"
void check() {
  vector<int> ret{INT_MIN,INT_MAX}; mt19937 rng(1709);
  for (int stage = 0; stage < 300; ++stage) {
    int n = stage < 5 ? stage : rng()%20000;
    auto saved = ret; solve(ret,n);
    assert(equal(ALL(saved),ret.begin()));
    assert(ret.size() == saved.size()+(n==2||n==3 ? 0 : n));
    if (n==2 || n==3) continue;
    vector<bool> cols(n+1),up(2*n+1),down(2*n+1);
    for (int r = 0; r < n; ++r) {
      int c = ret[saved.size()+r]; assert(1 <= c && c <= n);
      assert(!cols[c] && !up[r+c] && !down[r-c+n]);
      cols[c] = up[r+c] = down[r-c+n] = true;
    }
  }
}
}
int main(int argc,char **) {
  flow::check(argc>1); sat::check(); geometry::check();
  primes::check(); strings::check(); queens::check();
  cout << "PASS round17 g04: wide residual flow, SAT, exact geometry, "
          "prime powers, byte-string mutations, random queen appends\n";
}
