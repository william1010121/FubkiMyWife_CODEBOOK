#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll, ll>;
#define X first
#define Y second
#define SZ(a) int((a).size())
#define ALL(a) (a).begin(), (a).end()
#define pb push_back

namespace gh {
const int MAXN = 8, INF = 3;
#include "../../codebook/4_Flow_Matching/Dinic.cpp"
#include "../../codebook/4_Flow_Matching/Gomory_Hu_tree.cpp"
}
#include "../../codebook/5_String/SAIS-C++20.cpp"
#include "../../codebook/6_Math/ax+by=gcd.cpp"
#include "../../codebook/3_Data_Structure/Leftist_Tree.cpp"
#include "../../codebook/9_Else/AdaptiveSimpson.cpp"
namespace sam {
const int N = 256, CNUM = 2;
#include "../../codebook/5_String/exSAM.cpp"
}
const double eps = 1e-9, pi = acos(-1.);
int sign(double x) { return (x > eps) - (x < -eps); }
double sq(double x) { return x*x; }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/sectorArea.cpp"
#include "../../codebook/8_Geometry/circle.cpp"
#include "../../codebook/8_Geometry/halfPlaneIntersect.cpp"
#include "../../codebook/8_Geometry/convexHull.cpp"

void check(bool ok, const char *message) {
  if (!ok) throw runtime_error(message);
}
void near(double got, long double want, long double tol,
          const char *message) {
  check(isfinite(got) && abs(got-want) <= tol, message);
}

void cuts() {
  // All ternary K4 capacities, disconnected graphs, parallel arcs,
  // previous nonzero flow, and vertex labels unrelated to cut structure.
  mt19937 rng(1405);
  for (int code = 0; code < 729; ++code) {
    vector<int> labels{0,1,2,3}; shuffle(ALL(labels), rng);
    int c[4][4]{};
    gh::Dinic.init(4);
    int z = code;
    for (int i = 0; i < 4; ++i) for (int j = i+1; j < 4; ++j) {
      int a = labels[i], b = labels[j], cap = z%3; z /= 3;
      c[a][b] = c[b][a] = cap;
      for (int k = 0; k < cap; ++k)
        gh::Dinic.add_edge(a,b,1), gh::Dinic.add_edge(b,a,1);
    }
    gh::Dinic.maxflow(labels[0], labels[3]);
    for (int repeat = 0; repeat < 2; ++repeat) {
      gh::GomoryHu(4);
      vector<pair<int,int>> tree[4];
      for (int i = 1; i < 4; ++i) {
        check(0 <= gh::g[i] && gh::g[i] < 4, "GH parent range");
        tree[i].pb({gh::g[i],gh::gh_w[i]});
        tree[gh::g[i]].pb({i,gh::gh_w[i]});
      }
      for (int s = 0; s < 4; ++s) {
        vector<int> path(4,-1); path[s] = INT_MAX;
        auto dfs = [&](auto &&self, int u, int p) -> void {
          for (auto [v,w] : tree[u]) if (v != p) {
            check(path[v] == -1, "GH cycle");
            path[v] = min(path[u],w); self(self,v,u);
          }
        }; dfs(dfs,s,-1);
        for (int t = s+1; t < 4; ++t) {
          int want = INT_MAX;
          for (int mask = 0; mask < 16; ++mask)
            if ((mask>>s&1) && !(mask>>t&1)) {
              int cut = 0;
              for (int u = 0; u < 4; ++u) if (mask>>u&1)
                for (int v = 0; v < 4; ++v)
                  if (!(mask>>v&1)) cut += c[u][v];
              want = min(want,cut);
            }
          check(path[t] == want, "GH exhaustive cut");
        }
      }
    }
  }
  for (int n : {0,1,0,1}) gh::Dinic.init(n), gh::GomoryHu(n);
}

void suffixes() {
  // Ternary LMS patterns and gaps in symbol IDs; direct read-only view.
  for (int n = 0, count = 1; n <= 10; ++n, count *= 3)
    for (int code = 0; code < count; ++code) {
      vector<int> s(n); int z = code;
      for (int &x : s) x = array{1,17,257}[z%3], z /= 3;
      vector<int> want(n); iota(ALL(want),0);
      sort(ALL(want), [&](int a, int b) {
        return lexicographical_compare(s.begin()+a,s.end(),
                                       s.begin()+b,s.end());
      });
      Suffix got(s.data(),n);
      check(got.sa == want, "SAIS ternary suffix order");
      for (int i = 0; i < n; ++i) {
        int h = 0;
        if (i) while (want[i]+h < n && want[i-1]+h < n &&
                      s[want[i]+h] == s[want[i-1]+h]) ++h;
        check(got.ra[want[i]] == i && got.hi[i] == h,
              "SAIS inverse and direct LCP");
      }
      s.pb(0); want.insert(want.begin(),n);
      check(sais(span<const int>(s)) == want, "SAIS span sentinel");
    }
  unsigned char bytes[] = {255,128,1,255,128,1};
  Suffix got(bytes,6);
  check(got.sa == vector<int>({5,2,4,1,3,0}), "SAIS unsigned bytes");
}

void automata() {
  vector<string> words;
  for (int n = 0; n <= 5; ++n) for (int code = 0; code < (1<<n); ++code) {
    string s(n,'a');
    for (int i = 0; i < n; ++i) s[i] += code>>i&1;
    words.pb(s);
  }
  sam::exSAM a;
  for (const string &x : words) for (const string &y : words) {
    // Duplicate one document, plus a prefix, changes occurrence counts
    // without necessarily creating trie nodes; reuse the same object.
    vector<string> docs{x,y,x,x.substr(0,x.size()/2),""};
    map<string,int> want; int total = 0;
    a.init();
    for (const string &s : docs) {
      a.insert(s); total += SZ(s);
      for (int i = 0; i < SZ(s); ++i)
        for (int j = i+1; j <= SZ(s); ++j) ++want[s.substr(i,j-i)];
    }
    a.build(); a.solve();
    check(a.cnt[0] == total, "exSAM reused root count");
    ll distinct = 0;
    for (int v = 1; v < a.tot; ++v)
      distinct += a.len[v]-a.len[a.link[v]];
    check(distinct == ll(want.size()), "exSAM distinct substrings");
    for (auto &[s,count] : want) {
      int v = 0;
      for (char c : s) v = a.next[v][c-'a'];
      check(v && a.cnt[v] == count, "exSAM direct multiplicity");
    }
  }
}

void integers() {
  vector<ll> values{0,1,2,3,LLONG_MAX/2,LLONG_MAX/2+1,
                    LLONG_MAX-2,LLONG_MAX-1,LLONG_MAX};
  for (ll a : values) for (ll b : values) {
    auto [x,y] = exgcd(a,b);
    check((__int128)a*x+(__int128)b*y == gcd(a,b), "full-width Bezout");
  }
  // Verify the printed normalization recipe with both signs of res.
  for (ll a = 0; a <= 40; ++a) for (ll b = 1; b <= 40; ++b)
    for (ll multiple = -40; multiple <= 40; ++multiple) {
      ll g = gcd(a,b), res = g*multiple;
      auto [x,y] = exgcd(a,b); x *= multiple; y *= multiple;
      ll nx = (x%(b/g)+b/g)%(b/g);
      ll ny = y-(nx-x)/(b/g)*(a/g);
      check(0 <= nx && nx < b/g && a*nx+b*ny == res,
            "Bezout normalized solution recipe");
    }
}

pair<ll,ll> heap_audit(node *p) {
  if (!p) return {0,0};
  auto [ln,ls] = heap_audit(p->l);
  auto [rn,rs] = heap_audit(p->r);
  check((!p->l || p->data >= p->l->data) &&
        (!p->r || p->data >= p->r->data), "leftist max order");
  check(V(p->l) >= V(p->r) && p->v == V(p->r)+1,
        "leftist null path rank");
  check(p->sz == ln+rn+1 && p->sum == ls+rs+p->data,
        "leftist aggregates");
  return {p->sz,p->sum};
}
void heaps() {
  for (int mode = 0; mode < 3; ++mode) {
    vector<node*> roots; vector<ll> expected;
    for (int i = 0; i < 2048; ++i) {
      ll x = mode == 0 ? 0 : mode == 1 ? i : (i%3-1)*1000000000000LL;
      roots.pb(new node(x)); expected.pb(x);
    }
    while (roots.size() > 1) {
      vector<node*> next;
      for (int i = 0; i < SZ(roots); i += 2) {
        // Vary which equally ranked heap is the first argument.
        node *p = i%4 ? merge(roots[i+1],roots[i])
                     : merge(roots[i],roots[i+1]);
        heap_audit(p); next.pb(p);
      }
      roots = move(next);
    }
    node *p = roots[0]; sort(ALL(expected),greater<ll>());
    for (ll x : expected) {
      check(p && p->data == x, "leftist tournament drain");
      pop(p); heap_audit(p);
    }
    check(!p && !merge(p,nullptr), "leftist empty merge");
  }
  for (ll x : {LLONG_MIN,LLONG_MAX}) {
    node *p = merge(nullptr,new node(x)); heap_audit(p); pop(p);
    check(!p, "leftist full-width singleton");
  }
}

void integration() {
  int calls = 0;
  auto zero_mid = make_simpson([&](double x) { ++calls; return x; });
  check(zero_mid.mix({-1,-1},{1,1},0).first == 0 && !calls,
        "Simpson explicitly supplied zero midpoint");
  check(zero_mid.eval({-1,-1},{1,1},0,1e-10) == 0 && calls == 2,
        "Simpson zero midpoint sample reuse");
  auto e = make_simpson([](double x) { return exp(x); });
  for (auto [l,r] : vector<pair<double,double>>{
         {-20,0},{0,-20},{-4,-4},{-1e-5,1e-5}})
    for (int k : {1,2,3,997})
      near(e.eval2(l,r,1e-9,k), expl(r)-expl(l), 1e-8,
           "Simpson analytic exponential signed interval");
  Simpson<decltype(e.f),long double> wide{e.f};
  near(wide.eval(-4.L,0.L,1e-12L),1-expl(-4),1e-11,
       "Simpson long-double arithmetic");
}

long double polygon_area(const vector<P> &ps) {
  long double ans = 0;
  for (int i = 0; i < SZ(ps); ++i) {
    P a = ps[i], b = ps[(i+1)%SZ(ps)];
    ans += (long double)a.x*b.y-(long double)a.y*b.x;
  }
  return abs(ans)/2;
}
void polygons() {
  mt19937 rng(14051);
  for (int shear : {-100,0,100}) for (int turn = 0; turn < 4; ++turn) {
    auto transform = [&](P p) {
      p = {4*p.x+shear*p.y+123,2*p.y-37};
      for (int i = 0; i < turn; ++i) p = {-p.y,p.x};
      return p;
    };
    vector<P> h;
    for (P p : vector<P>{{0,0},{2,0},{2,2},{0,2}})
      h.pb(transform(p));
    vector<P> points = h;
    for (P p : h) points.pb(p), points.pb(p);
    points.pb(transform({1,1}));
    for (int repeat = 0; repeat < 8; ++repeat) {
      shuffle(ALL(points),rng);
      auto hull = convex(points);
      check(hull.size() == 4 && polygon_area(hull) == 32,
            "convex duplicate transformed parallelogram");
      CH ch(hull);
      for (int x = -2; x <= 6; ++x) for (int y = -2; y <= 6; ++y) {
        P p = transform({x/2.,y/2.});
        bool inside = 0 <= x && x <= 4 && 0 <= y && y <= 4;
        check(ch.contain(p) == inside, "CH affine containment");
        int a,b;
        check(ch.get_tangent(p,a,b) == !inside, "CH affine tangent");
        if (!inside) for (P q : ch.p) {
          check(crx(p,ch.p[a],q) <= eps, "CH tangent a support");
          check(crx(p,ch.p[b],q) >= -eps, "CH tangent b support");
        }
      }
      vector<L> lines;
      for (int i = 0; i < 4; ++i) {
        P a = h[i], b = h[(i+1)%4], d = b-a;
        for (int k : {1,2,16}) lines.emplace_back(a,a+d*k);
        P outside = a+P(d.y,-d.x);
        lines.emplace_back(outside,outside+d);
        lines.emplace_back(a,a+h[(i+1)%4]-h[(i+3)%4]);
      }
      shuffle(ALL(lines),rng); auto clipped = HPI(lines);
      check(clipped.size() >= 4, "HPI transformed positive region");
      near(polygon_area(clipped),32,1e-7,"HPI affine exact area");
      for (P p : clipped) for (L l : lines)
        check(isfinite(p.x) && isfinite(p.y) &&
              ((l.pb-l.pa)^(p-l.pa)) >= -1e-7,
              "HPI redundant constraints and finite vertices");
    }
  }
  vector<L> empty;
  check(convex({}).empty() && HPI(empty).empty(),
        "empty hull and HPI");
  // Caller canonicalizes the atan2 branch cut: equal directions must
  // have exactly equal stored angles before the template deduplicates.
  vector<L> branch_cut{
    L({0,-1},{1,-1}), L({1,-1},{1,0}), L({0,0},{0,-1}),
    L({1,+0.},{0,-0.}), L({1,-0.},{0,+0.})};
  for (L &l : branch_cut) if (l.o == pi) l.o = -pi;
  auto square = HPI(branch_cut);
  check(square.size() == 4, "HPI canonical branch-cut duplicate");
  near(polygon_area(square),1,1e-12,"HPI canonical square area");
  for (int n : {1,2,100}) {
    auto h = convex(vector<P>(n,{3,4}));
    check(h.size() == 1 && h[0].x == 3 && h[0].y == 4,
          "convex all identical");
  }
}

void circles() {
  // Exact disk-in-triangle and triangle-in-disk areas, all permutations.
  for (auto [t,r,want] : vector<tuple<vector<P>,double,double>>{
        {{{-4,-2},{4,-2},{0,4}},1,pi},
        {{{0,0},{1,0},{0,1}},10,.5},
        {{{0,0},{0,0},{0,0}},1,0},
        {{{-4,0},{0,0},{4,0}},2,0}}) {
    array<int,3> order{0,1,2};
    do {
      vector<P> p{t[order[0]],t[order[1]],t[order[2]]};
      near(AreaOfCircleTriangle(p,r),want,1e-10,
           "circle triangle containment and degeneracy");
      near(AreaOfCircleTriangle(p,0),0,0,"zero disk triangle area");
    } while (next_permutation(ALL(order)));
  }
  // Pythagorean chords: roots known exactly, both segment directions.
  for (auto [r,y,x] : vector<array<int,3>>{{5,3,4},{13,5,12},{25,7,24}})
    for (P o : vector<P>{{0,0},{123,-456},{-1000,1000}})
      for (int flip : {-1,1}) {
        P a = o+P(-2*r*flip,y), b = o+P(2*r*flip,y);
        auto roots = CircleCrossLine(a,b,o,r);
        check(roots.size() == 2,"circle Pythagorean chord roots");
        near(roots[0].x,o.x-x*flip,1e-10,"circle ordered entry");
        near(roots[1].x,o.x+x*flip,1e-10,"circle ordered exit");
        for (P p : roots) near((p-o).abs(),r,1e-10,"circle root radius");
      }
  for (double r : {0.,1.,7.}) {
    C a({3,4},r);
    check(Intersect(a,a).empty(),"coincident finite-point convention");
    near(IntersectArea(a,a),pi*r*r,1e-10,"coincident disk area");
    auto p = CircleCrossLine({3+r,4},{3+r,4},{3,4},r);
    check(p.size() == 1,"circle point segment on boundary");
  }
  // Signed zeros at the angle branch cut; scales do not affect area.
  for (double zero : {0.,-0.}) for (double scale : {1e-100,1.,1e100}) {
    near(SectorArea({-scale,zero},{-scale,-zero},3),0,0,
         "sector signed-zero branch cut");
    near(SectorArea({-scale,zero},{scale,zero},3),9*pi/2,1e-13,
         "sector opposite vectors");
  }
}

int main() {
  cuts(); suffixes(); automata(); integers(); heaps(); integration();
  polygons(); circles();
  cout << "PASS round14 g05: ten-template adversarial audit\n";
}
