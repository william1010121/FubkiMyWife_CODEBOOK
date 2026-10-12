// Round 17: independent edge oracles for all ten assigned templates.
#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/3_Data_Structure/Dynamic_2D_Segment_Tree.cpp"
#include "../../codebook/3_Data_Structure/IntervalContainer.cpp"
#include "../../codebook/9_Else/simulated_annealing.cpp"
namespace cent {
constexpr int N = 129;
#ifdef _LIBCPP_VERSION
// Test-only libc++ adapter for the template's GNU std::__lg helper.
constexpr int __lg(int n) { return bit_width(unsigned(n))-1; }
#endif
#include "../../codebook/3_Data_Structure/Centroid_Decomposition.cpp"
}
namespace mmc {
constexpr int N = 12;
constexpr ll INF = LLONG_MAX;
#include "../../codebook/2_Graph/MinimumMeanCycle.cpp"
}
namespace aho {
constexpr int len = 65, sigma = 3;
#include "../../codebook/5_String/Aho-Corasick_Automatan.cpp"
}
namespace geo {
struct Point {
  double x, y;
  Point operator-(Point b) const { return {x-b.x, y-b.y}; }
};
double Cross(Point a, Point b) { return a.x*b.y-a.y*b.x; }
double dist(Point a, Point b) { return hypot(a.x-b.x, a.y-b.y); }
vector<Point> p;
#include "../../codebook/8_Geometry/ClosestPair.cpp"
#include "../../codebook/8_Geometry/maxTriangleOfConvex.cpp"
}
namespace hull {
using std::abs;
constexpr double eps = 1e-9;
struct P {
  double x, y, z;
  P operator-(P b) const { return {x-b.x, y-b.y, z-b.z}; }
  P operator^(P b) const {
    return {y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x};
  }
  double operator*(P b) const { return x*b.x+y*b.y+z*b.z; }
};
double abs(P p) { return sqrt(p*p); }
#include "../../codebook/8_Geometry/convex3D.cpp"
}
mt19937_64 rng(1701);

void reclaim(Seg::Node *p) {
  if (!p) return;
  reclaim(p->l); reclaim(p->r); delete p;
}
void reclaim(Seg2D::Node *p) {
  if (!p) return;
  reclaim(p->l); reclaim(p->r);
  reclaim(p->sum.root); reclaim(p->lazy.root); delete p;
}
void force_push(Seg &s, Seg::Node *p, int l, int r) {
  if (!p || r-l <= 1) return;
  s.push(p,l,r); int m = l+(r-l)/2;
  force_push(s,p->l,l,m); force_push(s,p->r,m,r);
}
void force_push(Seg2D::Node *p, int m) {
  if (!p) return;
  force_push(p->sum,p->sum.root,0,m);
  force_push(p->lazy,p->lazy.root,0,m);
  force_push(p->l,m); force_push(p->r,m);
}
void dynamic_audit() {
  for (int n = 0; n <= 8; ++n) for (int m = 0; m <= 8; ++m) {
    Seg2D a(n,m), b(m,n); vector<vector<int>> grid(n,vector<int>(m));
    for (int step = 0; step < 120; ++step) {
      int l = rng()%(n+1), r = rng()%(n+1);
      int d = rng()%(m+1), u = rng()%(m+1), v = int(rng()%19)-9;
      if (l > r) swap(l,r);
      if (d > u) swap(d,u);
      a.update(l,r,d,u,v);
      // Partition an identical update along both axes, in reverse order.
      int x = (l+r)/2, y = (d+u)/2;
      for (auto [yl,yr] : {pii{y,u},pii{d,y}})
        for (auto [xl,xr] : {pii{x,r},pii{l,x}})
          b.update(yl,yr,xl,xr,v);
      for (int i = l; i < r; ++i) for (int j = d; j < u; ++j)
        grid[i][j] += v;
      if (step%3 == 0) force_push(a.root,m);
      for (int q = 0; q < 30; ++q) {
        int xl = rng()%(n+1), xr = rng()%(n+1);
        int yl = rng()%(m+1), yr = rng()%(m+1);
        if (xl > xr) swap(xl,xr);
        if (yl > yr) swap(yl,yr);
        int want = 0;
        for (int i = xl; i < xr; ++i) for (int j = yl; j < yr; ++j)
          want += grid[i][j];
        assert(a.query(xl,xr,yl,yr) == want);
        assert(b.query(yl,yr,xl,xr) == want);
      }
    }
    reclaim(a.root); reclaim(b.root);
  }
  Seg s(INT_MAX);
  s.update(INT_MAX-2,INT_MAX,INT_MAX/2);
  assert(s.query(INT_MAX-2,INT_MAX) == INT_MAX-1);
  s.update(INT_MAX-2,INT_MAX,-INT_MAX/2);
  assert(s.query(0,INT_MAX) == 0); reclaim(s.root);
}

void centroid_audit() {
  static cent::Cent_Dec c;
  for (int n : {128,1,64,2,127,3,65,1})
    for (int shape = 0; shape < 3; ++shape) {
      c.init(n); vector<int> ids(n); iota(ALL(ids),1);
      shuffle(ALL(ids),rng); vector<vector<pll>> g(n+1);
      for (int i = 1; i < n; ++i) {
        int j = shape == 0 ? i-1 : shape == 1 ? (i-1)/2 : rng()%i;
        int u = ids[i], v = ids[j];
        int w = i%3 == 0 ? INT_MIN : i%3 == 1 ? INT_MAX : 0;
        c.add_edge(u,v,w); g[u].pb({v,w}); g[v].pb({u,w});
      }
      c.build(); vector<ll> want(n+1);
      for (int v = 1; v <= n; ++v) {
        assert(c.query(v) == 0 && !c.done[v]);
        assert(0 <= c.layer[v] && c.layer[v] < bit_width(unsigned(n)));
      }
      for (int t = 0; t < 300; ++t) {
        int u = t%4 ? 1+rng()%n : ids.back(); c.modify(u);
        auto walk = [&](auto &&self, int v, int p, ll d) -> void {
          want[v] += d;
          for (auto [w,cost] : g[v]) if (w != p)
            self(self,w,v,d+cost);
        };
        walk(walk,u,0,0);
        for (int v = 1; v <= n; ++v) assert(c.query(v) == want[v]);
      }
    }
}

pll cycle_oracle(int n) {
  pll best{-1,-1}; vector<bool> used(n);
  auto dfs = [&](auto &&self,int start,int u,int k,ll sum) -> void {
    for (int v = start; v < n; ++v) if (mmc::road[u][v] != mmc::INF) {
      ll total = sum+mmc::road[u][v];
      if (v == start) {
        if (best.Y < 0 || (__int128)total*best.Y <
                         (__int128)best.X*(k+1)) best = {total,k+1};
      } else if (!used[v]) {
        used[v] = true; self(self,start,v,k+1,total); used[v] = false;
      }
    }
  };
  for (int s = 0; s < n; ++s) {
    used[s] = true; dfs(dfs,s,s,0,0); used[s] = false;
  }
  if (best.Y > 0) {
    ll d = gcd(best.X,best.Y); best.X /= d; best.Y /= d;
  } return best;
}
void cycle_audit() {
  mmc::MinimumMeanCycle c;
  for (int t = 0; t < 400; ++t) {
    int n = t%9; c.init(n); vector<ll> potential(n);
    for (ll &x : potential) x = (ll(rng()%3)-1)*(1LL<<55);
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
      mmc::road[u][v] = rng()%3 ? ll(rng()%61)-30 : mmc::INF;
    pll want = cycle_oracle(n); assert(c.solve() == want);
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
      if (mmc::road[u][v] != mmc::INF)
        mmc::road[u][v] += potential[v]-potential[u]+17;
    if (want.Y > 0) want.X += 17*want.Y;
    assert(c.solve() == want && cycle_oracle(n) == want);
    // Independently check every public DP row on the transformed graph.
    vector<ll> prev(n), next(n);
    for (int k = 1; k <= n; ++k) {
      fill(ALL(next),mmc::INF);
      for (int v = 0; v < n; ++v) for (int u = 0; u < n; ++u)
        if (prev[u] != mmc::INF && mmc::road[u][v] != mmc::INF)
          next[v] = min(next[v],prev[u]+mmc::road[u][v]);
      for (int v = 0; v < n; ++v) assert(c.dp[k][v] == next[v]);
      prev.swap(next);
    }
  }
}

void aho_audit() {
  for (int t = 0; t < 200; ++t) {
    aho::ac.init(); vector<string> patterns{"","A","B","C"};
    // Skinny trie with all suffixes and duplicates: long failure chains.
    string longest = t%2 ? "AAAAAAAAAAAAAAAAAAAAAAAB" : "ABCABCABC";
    for (int i = 0; i <= SZ(longest); ++i)
      patterns.pb(longest.substr(i));
    shuffle(ALL(patterns),rng); vector<int> end;
    for (auto &s : patterns) end.pb(aho::ac.input(s));
    for (auto &s : patterns) aho::ac.input(s);
    assert(aho::ac.top <= aho::len); aho::ac.make_fl();
    string text = t%7 ? longest+longest+"CCCCAAAA" : "";
    for (int batch = 0; batch < 4; ++batch) {
      for (int i = 1; i < aho::ac.top; ++i) aho::ac.cnt[i] = 0;
      aho::ac.cnt[1] = 1; int state = 1;
      // Stop/resume at every third character; preserve scan state.
      for (int i = 0; i < SZ(text); i += 3)
        for (int j = i; j < min(i+3,SZ(text)); ++j)
          ++aho::ac.cnt[state = aho::ac.rnx[state][text[j]-'A']];
      aho::ac.solve();
      for (int i = 0; i < SZ(patterns); ++i) {
        int want = 0;
        for (int j = 0; j+SZ(patterns[i]) <= SZ(text); ++j)
          want += text.compare(j,SZ(patterns[i]),patterns[i]) == 0;
        assert(aho::ac.cnt[end[i]] == want);
      }
      text += longest;
    }
  }
}

void closest_audit() {
  for (int t = 0; t < 600; ++t) {
    int n = t%21; vector<geo::Point> base;
    for (int i = 0; i < n; ++i)
      base.pb({double(int(rng()%17)-8),double(int(rng()%17)-8)});
    double s = ldexp(1., t%3 == 0 ? -400 : t%3 == 1 ? 0 : 400);
    for (auto &v : base) v = {(v.x+1024)*s,(v.y-1024)*s};
    shuffle(ALL(base),rng);
    sort(ALL(base),[](auto a,auto b) { return a.x < b.x; });
    geo::p = base;
    for (int l = 0; l <= n; ++l) for (int r = l-1; r < n; ++r) {
      double want = l >= r ? 1e9 : numeric_limits<double>::max();
      for (int i = l; i <= r; ++i) for (int j = i+1; j <= r; ++j)
        want = min(want,geo::dist(base[i],base[j]));
      assert(geo::closest_pair(l,r) == want);
    }
    for (int i = 0; i < n; ++i)
      assert(geo::p[i].x == base[i].x && geo::p[i].y == base[i].y);
  }
}

void triangle_audit() {
  using geo::Point;
  for (int t = 0; t < 300; ++t) {
    // Weakly convex rectangle boundaries: long edge plateaus, repeated
    // vertices and closing duplicates, with arbitrary cyclic starts.
    vector<Point> v;
    for (int x = 0; x < 6; ++x) v.pb({double(x),0});
    for (int y = 0; y < 4; ++y) v.pb({6,double(y)});
    for (int x = 6; x > 0; --x) v.pb({double(x),4});
    for (int y = 4; y > 0; --y) v.pb({0,double(y)});
    vector<Point> points;
    for (auto p : v) for (int i = 0, n = 1+rng()%4; i < n; ++i)
      points.pb(p);
    if (t&1) reverse(ALL(points));
    rotate(points.begin(),points.begin()+rng()%points.size(),points.end());
    points.pb(points.front());
    int a = int(rng()%7)-3, b = int(rng()%7)-3;
    // Nonzero determinant, including reflected and thin parallelograms.
    for (auto &p : points) p = {p.x+a*p.y+1000,
                               b*p.x+(a*b+1)*p.y-1000};
    vector<int> ids(points.size()); iota(ALL(ids),0);
    shuffle(ALL(ids),rng); vector<Point> storage(points.size());
    for (int i = 0; i < SZ(ids); ++i) storage[ids[i]] = points[i];
    double want = 0;
    for (int i = 0; i < SZ(points); ++i)
      for (int j = i+1; j < SZ(points); ++j)
        for (int k = j+1; k < SZ(points); ++k)
          want = max(want,abs(geo::Cross(points[j]-points[i],
                                        points[k]-points[i]))/2);
    assert(geo::ConvexHullMaxTriangleArea(storage.data(),ids.data(),
                                        SZ(ids)) == want);
  }
  for (int n : {0,1,2})
    assert(geo::ConvexHullMaxTriangleArea(nullptr,nullptr,n) == 0);
}

void convex_audit() {
  using hull::P; static hull::convex3D h;
  for (int t = 0; t < 500; ++t) {
    set<array<int,3>> coords; int n = t%18;
    while (SZ(coords) < n) coords.insert({int(rng()%7)-3,
      int(rng()%7)-3,t%5 ? int(rng()%7)-3 : 0});
    vector<P> points;
    for (auto q : coords) points.pb({double(q[0]+2*q[1]+100),
      double(q[1]+q[2]-200),double(-q[2]+300)});
    shuffle(ALL(points),rng); h.n = n; h.m = 0;
    copy(ALL(points),h.p); h(); set<unsigned> want, got;
    // Every supporting plane is identified by its full incident point set.
    for (int a = 0; a < n; ++a) for (int b = a+1; b < n; ++b)
      for (int c = b+1; c < n; ++c) {
        P normal = (h.p[b]-h.p[a])^(h.p[c]-h.p[a]);
        bool pos = false, neg = false; unsigned incident = 0;
        for (int j = 0; j < n; ++j) {
          double d = normal*(h.p[j]-h.p[a]);
          pos |= d > 0; neg |= d < 0;
          if (d == 0) incident |= 1U<<j;
        }
        if (pos != neg) want.insert(incident);
      }
    for (int i = 0; i < h.m; ++i) {
      auto f = h.f[i]; unsigned incident = 0;
      P normal = (h.p[f.b]-h.p[f.a])^(h.p[f.c]-h.p[f.a]);
      assert(normal*normal > 0 && f.res);
      for (int j = 0; j < n; ++j) {
        double d = normal*(h.p[j]-h.p[f.a]);
        assert(d <= 0);
        if (d == 0) incident |= 1U<<j;
      }
      got.insert(incident);
      for (auto [a,b] : {pii{f.a,f.b},pii{f.b,f.c},pii{f.c,f.a}}) {
        assert(h.id[a][b] == i);
        int j = h.id[b][a]; assert(0 <= j && j < h.m);
        auto g = h.f[j];
        assert((g.a == b && g.b == a) || (g.b == b && g.c == a) ||
               (g.c == b && g.a == a));
      }
    }
    assert(got == want && h.faces() == SZ(want));
    assert(h.faces() == SZ(want));
  }
}

set<pii> canonical(unsigned mask, const vector<int> &x) {
  set<pii> out;
  for (int i = 0; i+1 < SZ(x);) {
    if (!(mask>>i&1)) { ++i; continue; }
    int l = i;
    while (i+1 < SZ(x) && (mask>>i&1)) ++i;
    out.emplace(x[l],x[i]);
  } return out;
}
void interval_audit() {
  vector<int> x{INT_MIN,INT_MIN+1,-1,0,1,INT_MAX-1,INT_MAX};
  for (unsigned mask = 0; mask < 64; ++mask)
    for (int l = 0; l < SZ(x); ++l) for (int r = l; r < SZ(x); ++r)
      for (int op = 0; op < 2; ++op) {
        auto s = canonical(mask,x); unsigned changed = mask;
        for (int i = l; i < r; ++i)
          if (op) changed |= 1U<<i; else changed &= ~(1U<<i);
        if (op) {
          auto it = addInterval(s,x[l],x[r]);
          if (l == r) assert(it == s.end());
          else assert(it != s.end() && it->X <= x[l] && it->Y >= x[r]);
        } else removeInterval(s,x[l],x[r]);
        assert(s == canonical(changed,x));
      }
}

struct CopyOnly {
  int score; string trace;
  CopyOnly(int score, string trace) : score(score), trace(trace) {}
  CopyOnly(const CopyOnly &) = default;
  CopyOnly &operator=(const CopyOnly &) = default;
  CopyOnly(CopyOnly &&) = delete;
  CopyOnly &operator=(CopyOnly &&) = delete;
};
void anneal_audit() {
  // Equal-score plateau must consume exactly one uniform draw per step;
  // accepted ties change the current state, but never replace the best.
  for (double temp : {-100.,0.,1e-300,1e100}) {
    mt19937_64 r(1709), expected = r; int calls = 0;
    auto proposal = [&](const CopyOnly &s, auto &) {
      assert(s.trace == string(calls,'x')); ++calls;
      return CopyOnly(7,s.trace+'x');
    };
    auto score = [](const CopyOnly &s) { return double(s.score); };
    auto best = simulated_annealing(CopyOnly(7,""),score,proposal,r,
                                   200,temp,1.);
    uniform_real_distribution<double> unit(0,1);
    for (int i = 0; i < 200; ++i) unit(expected);
    assert(calls == 200 && best.trace.empty() && r == expected);
  }
  // Every uphill proposal is rejected at a zero temperature, without
  // mutating either the input's payload or the stored best state.
  mt19937_64 r(1709); int calls = 0;
  auto next = [&](const CopyOnly &s, auto &) {
    assert(s.score == -5 && s.trace == "initial"); ++calls;
    return CopyOnly(5,"proposal");
  };
  auto best = simulated_annealing(CopyOnly(-5,"initial"),
    [](const CopyOnly &s) { return double(s.score); },next,r,100,0.,0.);
  assert(calls == 100 && best.score == -5 && best.trace == "initial");
}

int main() {
  auto check = [](auto f, const char *s) {
    f(); cout << "PASS " << s << '\n' << flush;
  };
  check(dynamic_audit,"2D split/transpose/forced-push oracle");
  check(centroid_audit,"centroid capacity/reuse/signed paths");
  check(cycle_audit,"cycle potentials/translation/every DP cell");
  check(aho_audit,"Aho chunked scans/repeated batches/suffixes");
  check(closest_audit,"closest-pair scaled inclusive subranges");
  check(triangle_audit,"triangle long plateaus/affine mappings");
  check(convex_audit,"3D lattice supporting planes/adjacency");
  check(interval_audit,"interval exhaustive extreme initial states");
  check(anneal_audit,"annealing payload/tie RNG/rejections");
}
