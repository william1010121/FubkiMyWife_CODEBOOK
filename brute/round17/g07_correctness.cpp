#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using wide = __int128_t;
#define ALL(v) (v).begin(), (v).end()
#define SZ(v) int((v).size())
#define pb push_back
namespace bf {
const int MAXN = 12, INF = INT_MAX;
#include "../../codebook/4_Flow_Matching/Dinic.cpp"
#include "../../codebook/4_Flow_Matching/BoundedFlow.cpp"
}
namespace cir {
const int N = 8;
const ll INF = 1LL << 60;
#include "../../codebook/4_Flow_Matching/MinCostCirculation.cpp"
}
namespace clique {
const int N = 100;
#include "../../codebook/2_Graph/Maximum_Clique_Dyn.cpp"
}
namespace math {
#include "../../codebook/6_Math/ModMin.cpp"
#include "../../codebook/6_Math/Gaussian_gcd.cpp"
#include "../../codebook/6_Math/floor_ceil.cpp"
}
namespace cdq {
#include "../../codebook/9_Else/CDQ_Dominance.cpp"
}
namespace geo {
const double eps = 1e-9;
int sign(double x) { return (x > eps) - (x < -eps); }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/halfPlaneIntersect.cpp"
#include "../../codebook/8_Geometry/convexHull.cpp"
struct Point { double x, y; };
#include "../../codebook/8_Geometry/minDistOfTwoConvex.cpp"
}
mt19937_64 rng(1707);
void check(bool ok, const char *what) {
  if (!ok) throw runtime_error(what);
}

void audit_bounded() {
  struct E { int u,v,lo,hi; };
  bf::BoundedFlow b;
  for (int trial = 0; trial < 800; ++trial) {
    int n = 1+rng()%4;
    vector<E> es(rng()%7);
    for (auto &e : es) {
      e = {int(rng()%n),int(rng()%n),int(rng()%3),0};
      e.hi = e.lo+rng()%3;
    }
    for (int s = 0; s < n; ++s) for (int t = 0; t < n; ++t) {
      int best = -1; vector<int> bal(n);
      auto rec = [&](auto &&self, int i) -> void {
        if (i == SZ(es)) {
          for (int u = 0; u < n; ++u)
            if (u != s && u != t && bal[u]) return;
          if (s == t ? bal[s] != 0 : bal[s]+bal[t] != 0) return;
          if (s == t || bal[s] >= 0) best = max(best,bal[s]);
          return;
        }
        auto e = es[i];
        for (int f = e.lo; f <= e.hi; ++f) {
          bal[e.u] += f, bal[e.v] -= f;
          self(self,i+1);
          bal[e.u] -= f, bal[e.v] += f;
        }
      };
      rec(rec,0); b.init(n); vector<pair<int,int>> refs;
      for (auto e : es) {
        refs.pb({e.u,SZ(b.G[e.u])});
        b.add_edge(e.u,e.v,e.lo,e.hi);
      }
      check(b.solve(s,t) == best,"bounded exhaustive terminals");
      if (best < 0) continue;
      fill(ALL(bal),0);
      for (int i = 0; i < SZ(es); ++i) {
        auto [u,id] = refs[i]; auto e = b.G[u][id];
        auto r = b.G[e.to][e.rev];
        check(r.rev == id && r.to == u,"bounded residual indices");
        check(es[i].lo <= e.flow && e.flow <= es[i].hi,
              "bounded original limits");
        bal[u] += e.flow, bal[e.to] -= e.flow;
      }
      for (int u = 0; u < n; ++u)
        check(bal[u] == (s == t ? 0 : u == s ? best :
              u == t ? -best : 0),"bounded conservation");
      check(b.G[n+1].empty() && b.G[n+2].empty(),"demand cleanup");
    }
  }
  b.init(2); b.add_edge(0,1,0,INT_MAX);
  check(b.solve(0,1) == INT_MAX,"bounded maximum int flow");
  b.init(1); b.add_edge(0,0,INT_MAX,INT_MAX);
  check(b.solve(0,0) == 0,"bounded maximum fixed loop");
  b.init(0); check(b.solve(),"bounded empty circulation");
  cout << "PASS bounded: all terminals, residuals, INT_MAX\n";
}

void audit_circulation() {
  // Feasibility plus absence of negative residual cycles certifies
  // optimality independently, without multiplying costs in ll.
  auto &m = cir::mcmf;
  for (int bit = 0; bit <= 62; ++bit) for (int trial = 0;
       trial < 16; ++trial) {
    int n = 1+rng()%7; m.init(n); vector<pair<int,int>> refs;
    ll peak = 1LL << bit;
    for (int i = 0; i < 24; ++i) {
      int u = rng()%n, v = rng()%n;
      ll cap = i == 0 ? peak : ll(rng()%uint64_t(peak+1));
      ll cost = (ll(rng()%17)-8)*(trial%2 ? cir::INF/128 : 1);
      refs.pb({u,SZ(m.G[u])}); m.add_edge(u,v,cap,cost);
    }
    m.solve(bit); vector<wide> bal(n), d(n);
    for (auto [u,id] : refs) {
      auto &e = m.G[u][id]; auto &r = m.G[e.to][e.rev];
      check(e.cap == e.fcap && 0 <= e.flow && e.flow <= e.cap,
            "circulation full-width capacity");
      check(r.rev == id && r.to == u && r.flow == -e.flow,
            "circulation reciprocal flow");
      bal[u] += e.flow, bal[e.to] -= e.flow;
    }
    for (auto x : bal) check(x == 0,"circulation conservation");
    for (int pass = 0; pass < n; ++pass) {
      bool changed = false;
      for (int u = 0; u < n; ++u) for (auto e : m.G[u])
        if (e.cap > e.flow && d[e.to] > d[u]+e.cost)
          d[e.to] = d[u]+e.cost, changed = true;
      check(pass != n-1 || !changed,"negative residual cycle");
      if (!changed) break;
    }
  }
  m.init(1); m.add_edge(0,0,LLONG_MAX,-1); m.solve(62);
  check(m.G[0][0].flow == LLONG_MAX,"circulation LLONG_MAX loop");
  m.init(0); m.solve(0);
  cout << "PASS circulation: 1008 wide residual optimality certificates\n";
}

void audit_clique() {
  // Blow up a small graph into independent twin classes. Every clique
  // takes at most one twin, so a subset oracle on the quotient suffices.
  clique::MaxClique m;
  for (int trial = 0; trial < 240; ++trial) {
    int k = 1+rng()%12, n = trial%3 == 0 ? 100 : rng()%101;
    vector<int> group(n); for (int &g : group) g = rng()%k;
    bool edge[12][12]{};
    for (int u = 0; u < k; ++u) for (int v = 0; v < u; ++v)
      edge[u][v] = edge[v][u] = rng()%4 != 0;
    int live = 0, want = 0;
    for (int g : group) live |= 1 << g;
    for (int mask = live;; mask = (mask-1)&live) {
      bool ok = true;
      for (int u = 0; u < k; ++u) if (mask>>u&1)
        for (int v = 0; v < u; ++v) if (mask>>v&1)
          ok &= edge[u][v];
      if (ok) want = max(want,popcount(unsigned(mask)));
      if (!mask) break;
    }
    m.init(n);
    for (int u = 0; u < n; ++u) for (int v = 0; v < u; ++v)
      if (edge[group[u]][group[v]]) m.add_edge(u,v);
    for (int rep = 0; rep < 3; ++rep) {
      for (auto &c : m.cs) c.set(); // exercise every scratch reset
      check(m.solve() == want,"clique twin subset oracle");
      set<int> found;
      for (int i = 0; i < want; ++i) {
        int u = m.sol[i];
        check(0 <= u && u < n && found.insert(u).second,
              "clique result membership");
        for (int j = 0; j < i; ++j)
          check(m.G[u][m.sol[j]],"clique result edges");
      }
    }
  }
  cout << "PASS clique: reused 100-vertex twins and poisoned scratch\n";
}

wide sum_floor(ll n, ll m, ll a, ll b) {
  wide ans = 0;
  while (true) {
    ans += wide(n)*(n-1)/2*(a/m)+wide(n)*(b/m);
    a %= m, b %= m;
    wide y = wide(a)*n+b;
    if (y < m) return ans;
    n = y/m, b = y%m; swap(a,m);
  }
}
void audit_math() {
  for (int trial = 0; trial < 6000; ++trial) {
    ll m = 1+rng()%2000000000, a = rng()%m;
    ll l = rng()%m, r = trial%2 ? l : ll(rng()%m);
    if (l > r) swap(l,r);
    auto count = [&](ll n) {
      return sum_floor(n,m,a,m-l)-sum_floor(n,m,a,m-r-1);
    };
    ll low = 0, high = m/gcd(a,m);
    if (!count(high)) check(math::mod_min(a,m,l,r) == -1,
                            "mod min gcd gap");
    else {
      while (low+1 < high) {
        ll mid = (low+high)/2;
        if (count(mid)) high = mid; else low = mid;
      }
      check(math::mod_min(a,m,l,r) == high-1,"mod min count oracle");
    }
  }
  auto norm = [](complex<ll> z) {
    return wide(z.real())*z.real()+wide(z.imag())*z.imag();
  };
  for (int trial = 0; trial < 20000; ++trial) {
    auto value = [&]() { return ll(rng()%2000000001)-1000000000; };
    complex<ll> a(value(),value()), b(value(),value());
    if (b == complex<ll>()) b = {1,0};
    auto x = a, y = b;
    while (y != complex<ll>()) {
      wide den = norm(y);
      wide c = wide(x.real())*y.real()+wide(x.imag())*y.imag();
      wide d = wide(x.imag())*y.real()-wide(x.real())*y.imag();
      auto round = [&](wide z) {
        wide q = z/den, r = z%den;
        return ll(q+(2*(r < 0 ? -r : r) >= den ? (r > 0 ? 1 : -1) : 0));
      };
      complex<ll> rem = x-complex<ll>(round(c),round(d))*y;
      check(norm(rem)*2 <= den,"Gaussian nearest remainder bound");
      x = y, y = rem;
    }
    auto got = math::gaussian_gcd(a,b);
    check(norm(got) == norm(x),"Gaussian wide norm oracle");
    for (auto z : {a,b}) {
      wide c = wide(z.real())*got.real()+wide(z.imag())*got.imag();
      wide d = wide(z.imag())*got.real()-wide(z.real())*got.imag();
      check(c%norm(got) == 0 && d%norm(got) == 0,
            "Gaussian common divisor");
    }
  }
  for (ll x : {LLONG_MIN,0LL,LLONG_MAX})
    check(math::gaussian_gcd({x,0},{1,0}) == complex<ll>(1,0),
          "Gaussian exact divisor before rounding");
  for (int trial = 0; trial < 100000; ++trial) {
    int a = int(rng()), b = int(rng());
    if (!b || (a == INT_MIN && b == -1)) continue;
    ll q = ll(a)/b, r = ll(a)%b;
    check(math::floor(a,b) == q-(r && (r < 0) != (b < 0)),
          "floor wide signed oracle");
    check(math::ceil(a,b) == q+(r && (r < 0) == (b < 0)),
          "ceil wide signed oracle");
  }
  cout << "PASS math: count oracle, wide Gaussian Euclid, "
       << "signed division\n";
}

void audit_dominance() {
  for (int trial = 0; trial < 600; ++trial) {
    int n = rng()%50, k = rng()%9;
    vector<vector<int>> a(n,vector<int>(k));
    for (auto &p : a) for (int &x : p)
      x = array{INT_MIN,-1,0,1,INT_MAX}[rng()%5];
    vector<ll> want(n,-1);
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
      bool ok = true;
      for (int d = 0; d < k; ++d) ok &= a[j][d] <= a[i][d];
      want[i] += ok;
    }
    check(cdq::dominance(a) == want,"dominance direct wide ties");
    vector<int> dims(k); iota(ALL(dims),0); shuffle(ALL(dims),rng);
    auto b = a;
    for (int i = 0; i < n; ++i) for (int d = 0; d < k; ++d)
      b[i][d] = a[i][dims[d]];
    check(cdq::dominance(b) == want,"dominance dimension permutation");
    b = a; b.insert(b.end(),ALL(a));
    auto doubled = want;
    for (ll &x : doubled) x = 2*x+1;
    auto copy = doubled; doubled.insert(doubled.end(),ALL(copy));
    check(cdq::dominance(b) == doubled,"dominance duplicate weights");
  }
  cout << "PASS dominance: 0..8 dimensions, permutation, "
       << "doubled weights\n";
}
long double cross(geo::P a, geo::P b, geo::P c) {
  return (long double)(b.x-a.x)*(c.y-a.y)-
         (long double)(b.y-a.y)*(c.x-a.x);
}
vector<geo::P> hull(vector<geo::P> p) {
  sort(ALL(p),[](auto a, auto b) {
    return tie(a.x,a.y) < tie(b.x,b.y);
  });
  p.erase(unique(ALL(p),[](auto a, auto b) {
    return a.x == b.x && a.y == b.y;
  }),p.end());
  if (p.size() < 2) return p;
  vector<geo::P> lo,hi;
  for (auto q : p) {
    while (lo.size() > 1 && cross(lo[lo.size()-2],lo.back(),q) <= 0)
      lo.pop_back();
    lo.pb(q);
  }
  reverse(ALL(p));
  for (auto q : p) {
    while (hi.size() > 1 && cross(hi[hi.size()-2],hi.back(),q) <= 0)
      hi.pop_back();
    hi.pb(q);
  }
  lo.pop_back(), hi.pop_back(); lo.insert(lo.end(),ALL(hi));
  return lo;
}
bool inside(const vector<geo::P> &h, geo::P p) {
  if (h.size() == 1) return h[0].x == p.x && h[0].y == p.y;
  if (h.size() == 2) return cross(h[0],h[1],p) == 0 &&
    min(h[0].x,h[1].x) <= p.x && p.x <= max(h[0].x,h[1].x) &&
    min(h[0].y,h[1].y) <= p.y && p.y <= max(h[0].y,h[1].y);
  for (int i = 0; i < SZ(h); ++i)
    if (cross(h[i],h[(i+1)%SZ(h)],p) < 0) return false;
  return true;
}
long double distance(const vector<geo::P> &a,
                     const vector<geo::P> &b) {
  if (inside(a,b[0]) || inside(b,a[0])) return 0;
  long double best = INFINITY;
  auto segdist = [](geo::P p, geo::P a, geo::P b) {
    long double x = b.x-a.x, y = b.y-a.y, den = x*x+y*y;
    long double t = den ? clamp(((p.x-a.x)*x+(p.y-a.y)*y)/den,
                                0.L,1.L) : 0;
    return hypotl(p.x-a.x-t*x,p.y-a.y-t*y);
  };
  auto on = [](geo::P a, geo::P b, geo::P p) {
    return cross(a,b,p) == 0 &&
      min(a.x,b.x) <= p.x && p.x <= max(a.x,b.x) &&
      min(a.y,b.y) <= p.y && p.y <= max(a.y,b.y);
  };
  for (int i = 0; i < SZ(a); ++i) for (int j = 0; j < SZ(b); ++j) {
    auto u = a[i], v = a[(i+1)%SZ(a)],
         s = b[j], t = b[(j+1)%SZ(b)];
    auto c1 = cross(u,v,s), c2 = cross(u,v,t),
         c3 = cross(s,t,u), c4 = cross(s,t,v);
    if (on(u,v,s) || on(u,v,t) || on(s,t,u) || on(s,t,v) ||
        (c1*c2 < 0 && c3*c4 < 0)) return 0;
    best = min({best,segdist(u,s,t),segdist(v,s,t),
                     segdist(s,u,v),segdist(t,u,v)});
  }
  return best;
}
void audit_geometry() {
  for (int trial = 0; trial < 2000; ++trial) {
    vector<geo::P> points(rng()%25), other(1+rng()%15);
    for (auto &p : points)
      p = {double(int(rng()%17)-8),double(int(rng()%17)-8)};
    for (auto &p : other)
      p = {double(int(rng()%17)-8+trial%3*12),
           double(int(rng()%17)-8)};
    auto h = hull(points), got = geo::convex(points);
    check(h.size() == got.size(),"convex independent hull size");
    if (!h.empty()) {
      auto it = find_if(ALL(got),[&](auto p) {
        return p.x == h[0].x && p.y == h[0].y;
      });
      check(it != got.end(),"convex independent hull vertex");
      rotate(got.begin(),it,got.end());
      for (int i = 0; i < SZ(h); ++i)
        check(h[i].x == got[i].x && h[i].y == got[i].y,
              "convex CCW order");
    }
    if (h.size() >= 3) {
      geo::CH ch(h);
      for (int x = -12; x <= 12; x += 2)
        for (int y = -12; y <= 12; y += 2) {
          geo::P p(x,y); int a,b;
          bool in = inside(h,p);
          check(ch.contain(p) == in,"CH independent containment");
          check(ch.get_tangent(p,a,b) == !in,"CH tangent existence");
          if (!in) for (auto q : ch.p) {
            check(cross(p,ch.p[a],q) <= 0,"CH tangent a support");
            check(cross(p,ch.p[b],q) >= 0,"CH tangent b support");
          }
          int v = ch.findFarest(p);
          for (auto q : ch.p)
            check(ch.p[v]*p >= q*p,"CH tied support direction");
        }
      for (auto a : h) for (geo::P v : vector<geo::P>{
             {1,0},{0,1},{2,3},{-3,2}}) {
        for (int shift : {-1,0,1}) {
          geo::P p = a+geo::P(v.y,-v.x)*shift;
          long double low = INFINITY, high = -INFINITY;
          long double mn = INFINITY, mx = -INFINITY;
          for (int i = 0; i < SZ(h); ++i) {
            auto u = h[i], w = h[(i+1)%SZ(h)];
            long double x = cross(p,p+v,u), y = cross(p,p+v,w);
            mn = min(mn,x), mx = max(mx,x);
            auto add = [&](long double x, long double y) {
              long double t = ((x-p.x)*v.x+(y-p.y)*v.y)/(v*v);
              low = min(low,t), high = max(high,t);
            };
            if (!x) add(u.x,u.y);
            if (x*y < 0) {
              long double t = x/(x-y);
              add(u.x+t*(w.x-u.x),u.y+t*(w.y-u.y));
            }
          }
          auto chord = ch.getLineIntersect(p,p+v);
          if (mn < 0 && mx > 0) {
            check(chord.size() == 2,"CH crossing chord size");
            long double t[2];
            for (int i = 0; i < 2; ++i) {
              check(abs(cross(p,p+v,chord[i])) < 1e-7,
                    "CH chord lies on line");
              t[i] = ((chord[i]-p)*v)/(v*v);
            }
            if (t[0] > t[1]) swap(t[0],t[1]);
            check(abs(t[0]-low) < 1e-8 && abs(t[1]-high) < 1e-8,
                  "CH independent edge intersections");
          } else check(chord.empty(),"CH tangent/disjoint line");
        }
      }
      vector<geo::L> lines;
      for (int i = 0; i < SZ(h); ++i) {
        auto a = h[i], b = h[(i+1)%SZ(h)], d = b-a;
        for (int scale : {1,2,16}) lines.emplace_back(a,a+d*scale);
        auto out = a+geo::P(d.y,-d.x);
        lines.emplace_back(out,out+d);
        lines.emplace_back(a,a+b-h[(i+SZ(h)-1)%SZ(h)]);
      }
      // Canonical angle representation required by HPI's contract.
      for (auto &l : lines) if (l.o == acos(-1.)) l.o = -acos(-1.);
      shuffle(ALL(lines),rng); auto clipped = geo::HPI(lines);
      long double area = 0, expected = 0;
      for (int i = 0; i < SZ(h); ++i)
        expected += cross({0,0},h[i],h[(i+1)%SZ(h)]);
      for (int i = 0; i < SZ(clipped); ++i) {
        auto p = clipped[i];
        check(isfinite(p.x) && isfinite(p.y),"HPI finite vertex");
        for (auto l : lines)
          check(cross(l.pa,l.pb,p) >= -1e-6,"HPI all half planes");
        area += cross({0,0},p,clipped[(i+1)%SZ(clipped)]);
      }
      check(abs(area-expected) < 1e-7,"HPI redundant-support area");
    }
    if (!h.empty()) {
      auto b = hull(other); long double want = distance(h,b);
      vector<geo::Point> a2,b2;
      auto expand = [](const auto &h, auto &v) {
        for (int i = 0; i < SZ(h); ++i) {
          auto p = h[i], q = h[(i+1)%SZ(h)];
          v.pb({p.x,p.y}); v.pb({p.x,p.y});
          v.pb({(p.x+q.x)/2,(p.y+q.y)/2});
        }
        v.pb(v.front());
      };
      expand(h,a2); expand(b,b2);
      for (int rep = 0; rep < 4; ++rep) {
        double got = geo::TwoConvexHullMinDist(a2.data(),b2.data(),
                                               SZ(a2),SZ(b2));
        check(abs(got-want) < 1e-8L*max(1.L,want),
              "convex distance independent edge oracle");
        rotate(a2.begin(),a2.begin()+1,a2.end());
        reverse(ALL(b2));
      }
    }
  }
  cout << "PASS geometry: 2000 independent hull/support/distance audits\n";
}
int main() {
  audit_bounded(); audit_circulation(); audit_clique(); audit_math();
  audit_dominance(); audit_geometry();
}
