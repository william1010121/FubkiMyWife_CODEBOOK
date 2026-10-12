#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define SZ(a) int((a).size())
#define pb push_back
const double eps = 1e-9;
int sign(double x) { return (x > eps) - (x < -eps); }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/halfPlaneIntersect.cpp"
struct Point {
  double x, y;
  Point(double x = 0, double y = 0) : x(x), y(y) {}
  Point operator+(Point p) const { return {x+p.x, y+p.y}; }
  Point operator-(Point p) const { return {x-p.x, y-p.y}; }
  Point operator*(double t) const { return {x*t, y*t}; }
  Point operator/(double t) const { return {x/t, y/t}; }
};
double Cross(Point a, Point b) { return a.x*b.y-a.y*b.x; }
double len(Point p) { return hypot(p.x, p.y); }
#include "../../codebook/8_Geometry/triangleCenter.cpp"
#include "../../codebook/9_Else/DynamicConvexTrick.cpp"
#include "../../codebook/9_Else/HilbertCurve.cpp"
namespace mo {
#include "../../codebook/9_Else/Mos_Algorithm_With_modification.cpp"
}
namespace matching {
constexpr int N = 32;
#include "../../codebook/4_Flow_Matching/Bipartite_Matching.cpp"
}
namespace covering {
constexpr int N = 12;
void fwt(int *a, int n, int) {
  for (int s = 1; s < n; s *= 2)
    for (int base = 0; base < n; base += 2*s)
      for (int j = 0; j < s; ++j) a[base+s+j] += a[base+j];
}
#include "../../codebook/2_Graph/Minimum_Clique_Cover.cpp"
}
namespace zvalue {
constexpr int MAXn = 1024;
#include "../../codebook/5_String/Z-value.cpp"
}
void require(bool ok, const char *s) {
  if (!ok) throw runtime_error(s);
}
void near(double got, long double want, long double tolerance) {
  require(isfinite(got) && abs(got-want) <= tolerance,
          "triangle analytic center");
}

void geometry() {
  mt19937 rng(1105);
  // Independent convex hull supplies exact area and supporting constraints.
  for (int mask = 0; mask < 512; ++mask) {
    vector<P> points;
    for (int i = 0; i < 9; ++i) if (mask >> i & 1)
      points.push_back({double(i/3-1), double(i%3-1)});
    if (points.size() < 3) continue;
    vector<P> h;
    auto append = [&](P p, int lower) {
      while (SZ(h) > lower &&
             ((h.back()-h[SZ(h)-2]) ^ (p-h.back())) <= 0)
        h.pop_back();
      h.push_back(p);
    };
    for (P p : points) append(p, 1);
    int lower = SZ(h);
    for (int i = SZ(points)-2; i >= 0; --i) append(points[i], lower);
    h.pop_back();
    if (h.size() < 3) continue;
    double want = 0;
    for (int i = 0; i < SZ(h); ++i) want += h[i] ^ h[(i+1)%SZ(h)];
    vector<L> lines;
    for (int i = 0; i < SZ(h); ++i) {
      P a = h[i], b = h[(i+1)%SZ(h)], d = b-a;
      lines.emplace_back(a, b);
      lines.emplace_back(a, a+d*8); // Same normalized offset.
      P outside = a + P{d.y, -d.x}*4;
      lines.emplace_back(outside, outside+d*16);
      // Redundant tangent through a vertex, not parallel to either edge.
      P tangent = b-h[(i+SZ(h)-1)%SZ(h)];
      lines.emplace_back(a, a+tangent);
    }
    for (int repeat = 0; repeat < 5; ++repeat) {
      shuffle(lines.begin(), lines.end(), rng);
      auto polygon = HPI(lines);
      double area = 0;
      require(polygon.size() >= 3, "HPI positive-area hull");
      for (int i = 0; i < SZ(polygon); ++i) {
        P p = polygon[i];
        require(isfinite(p.x) && isfinite(p.y), "HPI finite vertex");
        area += p ^ polygon[(i+1)%SZ(polygon)];
        for (L l : lines)
          require(((l.pb-l.pa) ^ (p-l.pa)) >= -eps,
                  "HPI redundant vertex constraints");
      }
      require(abs(area-want) < 1e-8, "HPI lattice polygon area");
    }
  }
  // Short-side anchor must work for every permutation and orientation.
  for (auto triangle : vector<array<Point,3>>{
      {{{0,0},{6,0},{0,8}}}, {{{0,0},{8,0},{2,1}}},
      {{{0,0},{1000000,1},{1000001,1}}},
      {{{0,0},{1,1000000},{1,1000001}}}}) {
    for (double scale : {0.125, 1., 1024.}) {
      array<int,3> order{0,1,2};
      do {
        Point a = triangle[order[0]]*scale;
        Point b = triangle[order[1]]*scale;
        Point c = triangle[order[2]]*scale;
        long double ux = b.x-a.x, uy = b.y-a.y;
        long double vx = c.x-a.x, vy = c.y-a.y;
        long double det = 2*(ux*vy-uy*vx);
        long double ox = a.x + (vy*(ux*ux+uy*uy)-
                                        uy*(vx*vx+vy*vy))/det;
        long double oy = a.y + (ux*(vx*vx+vy*vy)-
                                        vx*(ux*ux+uy*uy))/det;
        auto o = TriangleCircumCenter(a,b,c);
        near(o.x, ox, 1e-10L*max(1.L,abs(ox)));
        near(o.y, oy, 1e-10L*max(1.L,abs(oy)));
        auto g = TriangleMassCenter(a,b,c);
        near(g.x, ((long double)a.x+b.x+c.x)/3, 1e-6);
        near(g.y, ((long double)a.y+b.y+c.y)/3, 1e-6);
        auto t = TriangleOrthoCenter(a,b,c);
        near(t.x, (long double)a.x+b.x+c.x-2*ox,
             1e-10L*max(1.L,abs(2*ox)));
        near(t.y, (long double)a.y+b.y+c.y-2*oy,
             1e-10L*max(1.L,abs(2*oy)));
        long double la = hypotl(b.x-c.x,b.y-c.y);
        long double lb = hypotl(a.x-c.x,a.y-c.y);
        long double lc = hypotl(a.x-b.x,a.y-b.y);
        auto in = TriangleInnerCenter(a,b,c);
        near(in.x, (a.x*la+b.x*lb+c.x*lc)/(la+lb+lc), 1e-6);
        near(in.y, (a.y*la+b.y*lb+c.y*lc)/(la+lb+lc), 1e-6);
      } while (next_permutation(order.begin(), order.end()));
    }
  }
}

void graphs_and_z() {
  matching::Bipartite_Matching bm;
  covering::Clique_Cover cc;
  // Exhaust partitions of n into clique components; reuse poisoned scratch.
  for (int n = 0; n <= covering::N; ++n)
    for (int cuts = 0; cuts < (1 << max(0,n-1)); ++cuts) {
      cc.init(n); bm.init(n,n);
      vector<int> group(n); int want = n != 0;
      for (int i = 1; i < n; ++i)
        group[i] = group[i-1] + (cuts >> (i-1) & 1),
        want += cuts >> (i-1) & 1;
      for (int u = 0; u < n; ++u) {
        bm.add_edge(u,u); bm.add_edge(u,u);
        for (int v = u+1; v < n; ++v) if (group[u] == group[v])
          cc.add_edge(u,v), cc.add_edge(v,u);
      }
      for (int repeat = 0; repeat < 2; ++repeat) {
        fill(begin(cc.co), end(cc.co), INT_MIN);
        fill(begin(cc.dp), end(cc.dp), UINT_MAX);
        require(cc.solve() == want, "clique partition poisoned scratch");
        require(bm.matching() == n, "matching duplicate diagonal");
        for (int u = 0; u < n; ++u)
          require(bm.mp[u] == u && bm.mq[u] == u,
                  "matching inverse maps");
      }
    }
  // Capacity boundary, shrinking to zero, then growing again.
  for (int n : {32,0,1,32,0}) {
    bm.init(n,n);
    for (int u = 0; u < n; ++u) bm.add_edge(u,n-u-1);
    require(bm.matching() == n, "matching capacity reuse");
    for (int u = 0; u < n; ++u)
      require(bm.mq[bm.mp[u]] == u, "matching inverse capacity maps");
  }
  mt19937 rng(11051);
  for (int repeat = 0; repeat < 256; ++repeat) {
    string s(zvalue::MAXn, '\0');
    for (char &c : s) c = char(rng());
    for (int n : {1024,1,0,1023,31}) {
      s.resize(n);
      fill(begin(zvalue::z),end(zvalue::z),INT_MAX);
      zvalue::make_z(s);
      require(zvalue::z[0] == INT_MAX, "Z poisoned index zero");
      for (int i = 1; i < n; ++i) {
        int want = 0;
        while (i+want < n && s[want] == s[i+want]) ++want;
        require(zvalue::z[i] == want, "Z all-byte capacity oracle");
      }
      for (int i = max(1,n); i < zvalue::MAXn; ++i)
        require(zvalue::z[i] == INT_MAX, "Z untouched array suffix");
    }
  }
}

pair<int,int> decode_hilbert(int n, ll d) {
  ll x = 0, y = 0;
  for (ll s = 1; s < n; s *= 2) {
    int rx = (d/2)&1, ry = (d^rx)&1;
    if (!ry) {
      if (rx) x = s-1-x, y = s-1-y;
      swap(x,y);
    }
    x += s*rx; y += s*ry; d /= 4;
  }
  return {int(x),int(y)};
}
void integer_algorithms() {
  mt19937_64 rng(110511);
  for (int k = 0; k <= 30; ++k) {
    int n = 1 << k; ll area = ll(n)*n;
    for (int i = 0; i < 2000; ++i) {
      ll d = rng()%area; auto [x,y] = decode_hilbert(n,d);
      require(hilbert(n,x,y) == d, "Hilbert independent inverse");
      if (d+1 < area) {
        auto [a,b] = decode_hilbert(n,d+1);
        require(abs(a-x)+abs(b-y) == 1, "Hilbert path adjacency");
      }
    }
  }
  for (ll intercept : {LLONG_MIN+2,LLONG_MAX-2}) {
    // Full signed range with equal slopes; no differences can overflow.
    DynamicHull h;
    for (ll delta : {0LL,-1LL,1LL,0LL}) h.addline(0,intercept+delta);
    require(h.query(-DynamicHull::kInf+1) == intercept+1 &&
            h.query(DynamicHull::kInf) == intercept+1,
            "hull equal slopes near signed intercept limits");
  }
  vector<pair<ll,ll>> lines{{-3,0},{0,4},{3,0},{0,4},{0,-4}};
  sort(lines.begin(),lines.end());
  do {
    DynamicHull h; vector<pair<ll,ll>> prefix;
    for (auto [a,b] : lines) {
      prefix.push_back({a,b}); h.addline(a,b);
      for (ll x : {-DynamicHull::kInf+1,-2LL,-1LL,0LL,1LL,2LL,
                    DynamicHull::kInf}) {
        __int128 want = LLONG_MIN;
        for (auto [s,t] : prefix) want = max(want,(__int128)s*x+t);
        require(h.query(x) == want, "hull endpoint and equal crossing");
      }
    }
  } while (next_permutation(lines.begin(),lines.end()));
}

void mo_queries() {
  auto unused = [](auto...) { throw runtime_error("empty Mo callback"); };
  mo::solve({},unused,unused,unused,unused,unused);
  mt19937 rng(1105111);
  for (int n : {1,2,9}) for (int block : {1,2,INT_MAX}) {
    vector<int> current(n), before(25), after(25), pos(25);
    for (int t = 1; t <= 24; ++t) {
      pos[t] = rng()%n; before[t] = current[pos[t]];
      after[t] = int(rng()%7)-3; current[pos[t]] = after[t];
    }
    fill(current.begin(),current.end(),0);
    vector<mo::Query> queries;
    // Every time and interval, including duplicates of every singleton.
    for (int t = 0; t <= 24; ++t)
      for (int l = 0; l < n; ++l) for (int r = l; r < n; ++r) {
        queries.emplace_back(l,r,t,block,SZ(queries));
        if (l == r) queries.emplace_back(l,r,t,block,SZ(queries));
      }
    vector<int> active(n), seen(queries.size()); int time = 0, sum = 0;
    auto change = [&](int t, int l, int r, bool forward) {
      require(t == time+forward, "Mo sequential update number");
      for (int i = 0; i < n; ++i)
        require(active[i] == (l <= i && i <= r), "Mo update active range");
      int p = pos[t], value = forward ? after[t] : before[t];
      if (active[p]) sum += value-current[p];
      current[p] = value; time += forward ? 1 : -1;
    };
    shuffle(queries.begin(),queries.end(),rng);
    mo::solve(std::move(queries),
      [&](int t,int l,int r) { change(t,l,r,true); },
      [&](int t,int l,int r) { change(t,l,r,false); },
      [&](int p) {
        require(!active.at(p), "Mo duplicate add");
        active[p] = 1; sum += current[p];
      },
      [&](int p) {
        require(active.at(p), "Mo inactive sub");
        active[p] = 0; sum -= current[p];
      },
      [&](const mo::Query &q) {
        vector<int> want(n);
        for (int t = 1; t <= q.T; ++t) want[pos[t]] = after[t];
        require(current == want && time == q.T, "Mo full array rollback");
        require(sum == accumulate(want.begin()+q.L,want.begin()+q.R+1,0),
                "Mo direct range sum");
        require(++seen[q.id] == 1, "Mo repeated query id");
        for (int i = 0; i < n; ++i)
          require(active[i] == (q.L <= i && i <= q.R), "Mo answer range");
      });
    require(all_of(seen.begin(),seen.end(),[](int x) { return x == 1; }),
            "Mo missing answer");
  }
}
int main() {
  geometry(); graphs_and_z(); integer_algorithms(); mo_queries();
  cout << "PASS round11 g05: lattice HPI, skinny centers, graph reuse, "
          "byte Z, Hilbert inverse, extreme hull, Mo\n";
}
