#include "../../codebook/1_Basic/Default_code.cpp"
namespace graph {
#include "../../codebook/2_Graph/Bridge.cpp"
}
namespace ztest {
constexpr int MAXn = 8192;
#include "../../codebook/5_String/Z-value.cpp"
}
namespace suffix {
#include "../../codebook/5_String/SAIS-C++20.cpp"
}
namespace factorial {
constexpr int MAXP = 262146;
#include "../../codebook/6_Math/fac_no_p.cpp"
}
namespace bezout {
#include "../../codebook/6_Math/ax+by=gcd.cpp"
}
namespace mobius {
int n, tot, mu[65539], flg[65539], p[65539];
#include "../../codebook/6_Math/getMu.cpp"
}
namespace mo {
#include "../../codebook/9_Else/Mos_Algorithm_With_modification.cpp"
}
namespace geometry {
const double eps = 1e-9, pi = acos(-1.);
double sq(double x) { return x*x; }
int sign(double x) { return (x > eps) - (x < -eps); }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/sectorArea.cpp"
#include "../../codebook/8_Geometry/circle.cpp"
}
void check(bool ok, const char *what) {
  if (!ok) { cerr << "FAIL " << what << '\n'; exit(1); }
}
mt19937_64 rng(170008);

void bridges() {
  for (int tc = 0; tc < 240; ++tc) {
    int n = tc % 65; vector<pii> es;
    auto add = [&](int u, int v) { es.pb({u,v}); };
    for (int v = 0; v < n; ++v) {
      add(v,v);
      if (v && v % 11) {
        add(v-1,v);
        if (tc & 1) add(v,v-1);
        if (v % 4 == 3) add(v-3,v);
      }
    }
    vector<int> perm(n); iota(ALL(perm),0); shuffle(ALL(perm),rng);
    for (auto &[u,v] : es) u = perm[u], v = perm[v];
    shuffle(ALL(es),rng); graph::ECC g(n);
    for (auto [u,v] : es) g.add_edge(u,v);
    auto components = [&](const vector<int> &removed) {
      vector<int> label(n,-1); int count = 0;
      for (int u = 0; u < n; ++u) if (label[u] < 0) {
        vector<int> q{u}; label[u] = count++;
        for (int at = 0; at < SZ(q); ++at)
          for (int e = 0; e < SZ(es); ++e) if (!removed[e]) {
            auto [a,b] = es[e]; if (b == q[at]) swap(a,b);
            if (a == q[at] && label[b] < 0)
              label[b] = label[u], q.pb(b);
          }
      }
      return pair{count,label};
    };
    vector<int> removed(SZ(es)), want(SZ(es));
    int original = components(removed).first;
    for (int e = 0; e < SZ(es); ++e) {
      removed[e] = 1;
      want[e] = components(removed).first > original;
      removed[e] = 0;
    }
    auto [count,label] = components(want); g.solve();
    check(g.is_bridge == want && g.necc == count && g.stk.empty(),
          "bridges: block chains, loops, reversed duplicates");
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
      check((g.bln[u] == g.bln[v]) == (label[u] == label[v]),
            "ECC labels after deleting bridges");
    auto saved = g.bln; g.solve();
    check(saved == g.bln && g.is_bridge == want, "ECC repeated solve");
  }
}

// Independent doubling oracle, including direct sentinel suffix.
vector<int> suffix_order(const vector<int> &s) {
  int n = SZ(s); vector<int> sa(n), rank = s, next(n);
  iota(ALL(sa),0);
  for (int h = 1; h < max(2,n); h *= 2) {
    auto key = [&](int i) { return pii{rank[i],
      i+h < n ? rank[i+h] : -1}; };
    sort(ALL(sa),[&](int i,int j) { return key(i) < key(j); });
    if (!n) break;
    next[sa[0]] = 0;
    for (int i = 1; i < n; ++i)
      next[sa[i]] = next[sa[i-1]] + (key(sa[i-1]) != key(sa[i]));
    rank = next;
    if (rank[sa.back()] == n-1) break;
  }
  return sa;
}
void suffix_case(vector<int> s) {
  auto want = suffix_order(s); suffix::Suffix got(s.data(),SZ(s));
  check(got.sa == want, "SAIS vs doubling");
  for (int i = 0; i < SZ(s); ++i) {
    check(got.ra[want[i]] == i, "SAIS inverse ranks");
    int h = 0;
    if (i) while (max(want[i],want[i-1])+h < SZ(s) &&
                  s[want[i]+h] == s[want[i-1]+h]) ++h;
    check(got.hi[i] == h, "SAIS direct LCP");
  }
  s.pb(0);
  check(suffix::sais(span<const int>(s)) == suffix_order(s),
        "direct SAIS through immutable span");
}
void strings() {
  for (int n : {0,1,2,31,127,511,1023,2049,4097}) {
    for (int shape = 0; shape < 6; ++shape) {
      vector<int> s(n);
      for (int i = 0; i < n; ++i)
        s[i] = shape == 0 ? 1 : shape == 1 ? 1+i%2 :
          shape == 2 ? 1+popcount(unsigned(i))%2 :
          shape == 3 ? 1+(i%31 == 0) :
          shape == 4 ? 1+(i%7 == 0 ? i%29 : 0) : 1+rng()%257;
      suffix_case(s); reverse(ALL(s)); suffix_case(s);
    }
  }
  for (int tc = 0; tc < 400; ++tc) {
    int run = 1+rng()%60; vector<int> s;
    for (int b = 0; b < 30; ++b) {
      s.pb(257); s.insert(s.end(),run+(b%3),1);
      s.pb(b%7 == 0 ? 17 : 2);
    }
    s[rng()%SZ(s)] = 128; suffix_case(s);
  }
  for (int n : {8192,1,0,8191,2,4097,0}) {
    string s(n,'\0');
    for (int i = 0; i < n; ++i) s[i] = char(i%19 ? 255 : 128);
    fill_n(ztest::z,ztest::MAXn,INT_MIN); ztest::make_z(s);
    check(ztest::z[0] == INT_MIN, "Z leaves zero untouched");
    for (int i = 1; i < n; ++i) {
      int h = 0; while (i+h < n && s[h] == s[i+h]) ++h;
      check(ztest::z[i] == h, "Z poisoned byte/periodic capacity");
    }
    for (int i = max(1,n); i < ztest::MAXn; ++i)
      check(ztest::z[i] == INT_MIN, "Z leaves unused tail untouched");
  }
}

void numeric_audit() {
  // Exact local ratio identity U(n)/U(n-1) = n with p factors removed;
  // this checks the recurrence at full-width n without iterating to n.
  for (auto [p,pk] : vector<pll>{{2,2},{2,4},{2,8},{2,131072},
        {3,177147},{5,78125},{7,117649},{257,66049}}) {
    for (ll n : {1LL,p,pk-1,pk,pk+1,pk*p,LLONG_MAX-1,LLONG_MAX}) {
      ll u = factorial::fac_no_p(n-1,p,pk), x = n;
      while (x%p == 0) x /= p;
      check(factorial::fac_no_p(n,p,pk) == (__int128)u*x%pk,
            "stripped factorial adjacent full-width ratio");
    }
    check(factorial::fac_no_p(0,p,pk) == 1, "stripped zero factorial");
  }
  vector<ll> f{0,1};
  while (f.back() <= LLONG_MAX-f[SZ(f)-2])
    f.pb(f.back()+f[SZ(f)-2]);
  for (int i = 2; i < SZ(f); ++i) for (ll d : {-1LL,0LL,1LL}) {
    ll a = f[i]+d, b = f[i-1];
    for (auto [x,y] : vector<pll>{{a,b},{b,a},{a,0},{0,a}}) {
      auto [u,v] = bezout::exgcd(x,y);
      check((__int128)x*u+(__int128)y*v == gcd(x,y),
            "Bezout Fibonacci worst depth and neighbors");
    }
  }
  auto zero = bezout::exgcd(0,0);
  check(zero == pll(1,0), "Bezout zero convention");
  for (int size : {1,2,127,65537,65536,729,1}) {
    using namespace mobius;
    n = size; tot = 0; fill_n(flg,65539,0);
    fill_n(mu,65539,12345); fill_n(p,65539,-1); getMu();
    vector<int> primes;
    for (int i = 1; i <= n; ++i) {
      int x = i, expected = 1; bool square = false;
      for (int q = 2; q*q <= x; ++q) if (x%q == 0) {
        x /= q; expected = -expected;
        if (x%q == 0) square = true;
        while (x%q == 0) x /= q;
      }
      if (x > 1) expected = -expected;
      if (square) expected = 0;
      check(mu[i] == expected, "Mobius independent trial factors");
      bool prime = i > 1 && x == i;
      check(bool(flg[i]) == (i > 1 && !prime),
            "Mobius composite flags");
      if (prime) primes.pb(i);
    }
    check(tot == SZ(primes) && equal(ALL(primes),p+1),
          "Mobius prime output");
    check(mu[0] == 12345 && mu[n+1] == 12345 && p[tot+1] == -1,
          "Mobius guards and unused prime tail");
  }
}

void modifications() {
  auto unused = [](auto...) { check(false,"empty Mo callback"); };
  mo::solve({},unused,unused,unused,unused,unused);
  for (int n : {1,2,33,257}) for (int updates : {0,1,64})
    for (int block : {1,7,1000,INT_MAX}) {
      vector<int> initial(n), a, pos(updates+1), old(updates+1);
      vector<int> value(updates+1);
      for (int &x : initial) x = int(rng()%7)-3;
      a = initial;
      for (int t = 1; t <= updates; ++t) {
        pos[t] = t%3 ? 0 : n-1; old[t] = a[pos[t]];
        value[t] = t%4 ? int(rng()%7)-3 : old[t];
        a[pos[t]] = value[t];
      }
      vector<mo::Query> qs; vector<vector<int>> want;
      for (int t = 0; t <= updates; ++t) {
        vector<int> replay = initial;
        for (int k = 1; k <= t; ++k) replay[pos[k]] = value[k];
        for (int i = 0; i < 12; ++i) {
          int l = i%3 ? rng()%n : 0, r = i%3 ? l : n-1;
          qs.emplace_back(l,r,t,block,SZ(qs)); vector<int> freq(7);
          for (int p = l; p <= r; ++p) ++freq[replay[p]+3];
          want.pb(freq);
        }
      }
      shuffle(ALL(qs),rng); a = initial;
      vector<int> active(n), freq(7), seen(SZ(qs)); int time = 0;
      int forwards = 0, backwards = 0;
      auto change = [&](int t,int l,int r,bool forward) {
        check(t == time+forward, "Mo chronological apply/undo");
        for (int p = 0; p < n; ++p)
          check(active[p] == (l <= p && p <= r), "Mo current range");
        int p = pos[t], next = forward ? value[t] : old[t];
        check(a[p] == (forward ? old[t] : value[t]),
              "Mo repeated overwrite/no-op source");
        if (active[p]) --freq[a[p]+3], ++freq[next+3];
        a[p] = next; time += forward ? 1 : -1;
        if (forward) ++forwards; else ++backwards;
      };
      mo::solve(qs,
        [&](int t,int l,int r) { change(t,l,r,true); },
        [&](int t,int l,int r) { change(t,l,r,false); },
        [&](int p) {
          check(!active.at(p),"Mo add inactive");
          active[p] = 1; ++freq[a[p]+3];
        },
        [&](int p) {
          check(active.at(p),"Mo remove active");
          active[p] = 0; --freq[a[p]+3];
        },
        [&](const mo::Query &q) {
          check(time == q.T && freq == want[q.id],"Mo full frequency");
          check(++seen[q.id] == 1,"Mo duplicate-key query IDs");
        });
      check(all_of(ALL(seen),[](int x) { return x == 1; }),
            "Mo every query answered");
      if (updates && n > 1 && block == 1)
        check(forwards && backwards,"Mo actually exercises rollback");
    }
}

// Independent signed edge integral, splitting at quadratic roots.
long double edge_area(geometry::P a, geometry::P b, long double r) {
  long double x = a.x, y = a.y, dx = b.x-x, dy = b.y-y;
  long double A = dx*dx+dy*dy, B = 2*(x*dx+y*dy);
  if (!A || !r) return 0;
  long double C = x*x+y*y-r*r, D = B*B-4*A*C;
  vector<long double> cuts{0,1};
  if (D >= 0) for (int sign : {-1,1}) {
    long double t = (-B+sign*sqrtl(D))/(2*A);
    if (0 < t && t < 1) cuts.pb(t);
  }
  sort(ALL(cuts)); long double sum = 0;
  for (int i = 1; i < SZ(cuts); ++i) {
    auto l = cuts[i-1], h = cuts[i], m = (l+h)/2;
    long double ax = x+dx*l, ay = y+dy*l;
    long double bx = x+dx*h, by = y+dy*h, cross = ax*by-ay*bx;
    long double mx = x+dx*m, my = y+dy*m;
    if (mx*mx+my*my < r*r) sum += cross/2;
    else sum += r*r*atan2l(cross,ax*bx+ay*by)/2;
  }
  return sum;
}
void circles() {
  using namespace geometry;
  for (int tc = 0; tc < 4000; ++tc) {
    double r = rng()%1025/8., s = rng()%1025/8., d = rng()%3073/8.;
    C a({17,-31},r), b({17+d*.6,-31+d*.8},s);
    auto roots = Intersect(a,b);
    int count = d == 0 || d > r+s || d < abs(r-s) ? 0 :
      d == r+s || d == abs(r-s) ? 1 : 2;
    check(SZ(roots) == count,"circle pair topology with zero radii");
    for (P p : roots)
      check(abs((p-a.c).abs()-r) < 1e-8 &&
            abs((p-b.c).abs()-s) < 1e-8,"circle pair root membership");
    long double R = r, S = s, D = d, want;
    if (d >= r+s) want = 0;
    else if (d <= abs(r-s)) want = pi*min(R*R,S*S);
    else {
      long double alpha = acosl((D*D+R*R-S*S)/(2*D*R));
      long double beta = acosl((D*D+S*S-R*R)/(2*D*S));
      long double area = sqrtl((-D+R+S)*(D+R-S)*(D-R+S)*(D+R+S))/2;
      want = R*R*alpha+S*S*beta-area;
    }
    check(abs(IntersectArea(a,b)-want) < 2e-8*max(1.L,want),
          "circle lens independent cosine/Heron oracle");
    check(abs(IntersectArea(a,b)-IntersectArea(b,a)) < 1e-8,
          "circle lens symmetry");
  }
  for (int tc = 0; tc < 20000; ++tc) {
    vector<P> t(3);
    for (P &p : t) p = {double(int(rng()%41)-20),
                        double(int(rng()%41)-20)};
    double r = tc%11 ? 1+tc%7 : 0; long double want = 0;
    for (int i = 0; i < 3; ++i) want += edge_area(t[i],t[(i+1)%3],r);
    double got = AreaOfCircleTriangle(t,r);
    check(abs(got-abs(want)) < 2e-8,"circle triangle split integral");
    reverse(ALL(t));
    check(abs(got-AreaOfCircleTriangle(t,r)) < 2e-8,
          "circle triangle orientation");
  }
  for (double scale : {1.,16.,1024.}) {
    // Diameter, tangent, boundary points, wholly inside/outside segments.
    P o(17*scale,-31*scale); double r = 5*scale;
    for (int y : {0,3,5,6}) {
      auto roots = CircleCrossLine(o+P(-10*scale,y*scale),
                                   o+P(10*scale,y*scale),o,r);
      check(SZ(roots) == (y < 5 ? 2 : y == 5 ? 1 : 0),
            "circle segment exact root counts");
      for (P p : roots)
        check(abs((p-o).abs()-r) < 1e-8,"circle segment membership");
    }
    check(CircleCrossLine(o+P(-scale,0),o+P(scale,0),o,r).empty(),
          "inside segment has no boundary roots");
    for (P p : vector<P>{o,o+P(r,0),o+P(2*r,0)})
      check(SZ(CircleCrossLine(p,p,o,r)) == (p.x == o.x+r),
            "zero-length point on/off circle");
  }
  for (double a : {1e-100,1.,1e100})
    for (double b : {1e-100,1.,1e100}) {
      check(SectorArea({a,0},{0,b},2) == pi,
            "sector independent vector scales");
      check(SectorArea({-a,+0.},{-b,-0.},2) == 0,
            "sector signed-zero branch wrap");
      check(SectorArea({a,0},{-b,0},0) == 0,"sector zero radius");
    }
}
int main() {
  bridges(); strings(); numeric_audit(); modifications(); circles();
  cout << "PASS round17 g08: all nine templates adversarial audit\n";
}
