#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll, ll>;
#define X first
#define Y second
#define ALL(v) (v).begin(), (v).end()
#define pb push_back
#define BLACK_MAGIC_NO_MAIN
#include "../../codebook/1_Basic/black_magic.cpp"
#include "../../codebook/6_Math/ax+by=gcd.cpp"
namespace rho {
ll mul(ll a, ll b, ll m) {
  assert(0 <= a && a < m && 0 <= b && b < m);
  return (__int128)a * b % m;
}
#include "../../codebook/6_Math/Miller_Rabin.cpp"
bool prime(ll n) {
  if (n < 2) return false;
  for (ll p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
    if (n == p) return true;
    if (n % p == 0) return false;
  }
  for (ll a : {2LL, 325LL, 9375LL, 28178LL, 450775LL,
              9780504LL, 1795265022LL})
    if (!Miller_Rabin(a, n)) return false;
  return true;
}
#include "../../codebook/6_Math/Pollard_Rho.cpp"
}
#include "../../codebook/8_Geometry/PointInConvex.cpp"
struct Point {
  double x, y;
  Point operator-(Point b) const { return {x-b.x, y-b.y}; }
};
double Cross(Point a, Point b) { return a.x*b.y-a.y*b.x; }
#include "../../codebook/8_Geometry/maxTriangleOfConvex.cpp"
const double eps = 1e-9, pi = acos(-1.);
double sq(double x) { return x*x; }
int sign(double x) { return abs(x) < eps ? 0 : (x > 0 ? 1 : -1); }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/sectorArea.cpp"
#include "../../codebook/8_Geometry/circle.cpp"
#include "../../codebook/9_Else/AdaptiveSimpson.cpp"
namespace queens {
#include "../../codebook/9_Else/NQueens.cpp"
}
namespace sam {
const int N = 128, CNUM = 2;
#include "../../codebook/5_String/exSAM.cpp"
}
namespace tiny_sam {
const int N = 1, CNUM = 1;
#include "../../codebook/5_String/exSAM.cpp"
}
void require(bool ok, const char *msg) {
  if (!ok) { cerr << "FAIL " << msg << '\n'; exit(1); }
}
mt19937_64 rng(110009);
void number_audit() {
  auto check = [](ll a, ll b) {
    auto [x,y] = exgcd(a,b);
    require((__int128)a*x + (__int128)b*y == gcd(a,b),
            "full-width Bezout identity");
  };
  check(0,0);
  vector<ll> fib{0,1};
  while ((__int128)fib.back()+fib[fib.size()-2] <= LLONG_MAX)
    fib.pb(fib.back()+fib[fib.size()-2]);
  for (int i = 1; i < (int)fib.size(); ++i) {
    check(fib[i],fib[i-1]); check(fib[i-1],fib[i]);
  }
  for (int i = 0; i < 20000; ++i) check(rng()>>1, rng()>>1);
  vector<ll> primes{3,5,7,97,1000000007,2147483647,3037000493};
  for (ll p : primes) {
    // Independently certify each input prime before using its known map.
    bool prime = true;
    for (ll d = 2; d <= p/d; ++d) if (p%d == 0) prime = false;
    require(prime, "test prime is prime");
    ll power = 1; int e = 0;
    while (power <= LLONG_MAX/p) {
      power *= p; ++e;
      rho::cnt.clear(); rho::PollardRho(power);
      require(rho::cnt == map<ll,int>{{p,e}}, "prime powers");
    }
  }
  rho::cnt.clear();
  for (ll p : primes) rho::PollardRho(p);
  auto saved = rho::cnt; rho::PollardRho(1);
  require(rho::cnt == saved, "rho unit preserves accumulated map");
  for (auto [p,q] : vector<pll>{{3037000493,3037000493},
                               {2147483647,3037000493}}) {
    rho::cnt.clear(); rho::PollardRho(p*q);
    map<ll,int> want; ++want[p]; ++want[q];
    require(rho::cnt == want, "near-limit semiprime");
  }
}
void sam_audit() {
  sam::exSAM s;
  vector<string> words{"","a","b","aa","ab","ba","bb"};
  for (int code = 0; code < 2187; ++code) {
    vector<string> docs; int z = code;
    for (auto w : words) {
      for (int i = 0; i < z%3; ++i) docs.pb(w);
      z /= 3;
    }
    for (int order = 0; order < 2; ++order) {
      shuffle(ALL(docs),rng); s.init();
      map<string,int> want; int total = 0;
      for (const auto &w : docs) {
        s.insert(w); total += w.size();
        for (int l = 0; l < (int)w.size(); ++l)
          for (int r = l+1; r <= (int)w.size(); ++r)
            ++want[w.substr(l,r-l)];
      }
      s.build(); s.solve();
      require(s.cnt[0] == total, "SAM empty/duplicate root counts");
      int distinct = 0;
      for (int v = 1; v < s.tot; ++v)
        distinct += s.len[v]-s.len[s.link[v]];
      require(distinct == (int)want.size(), "SAM document boundaries");
      for (auto [w,count] : want) {
        int v = 0;
        for (char c : w) { v = s.next[v][c-'a'];
          require(v != 0, "SAM substring transition"); }
        require(s.cnt[v] == count, "SAM repeated documents");
      }
    }
  }
  tiny_sam::exSAM t;
  t.init(); t.insert(""); t.build(); t.solve();
  require(t.tot == 1 && t.cnt[0] == 0, "minimum SAM empty capacity");
  t.init(); t.insert("a"); t.insert("a"); t.build(); t.solve();
  require(t.tot == 2 && t.cnt[0] == 2 && t.cnt[1] == 2,
          "minimum SAM exact capacity and duplicates");
}
__int128 wide_turn(IPoint a, IPoint b, IPoint c) {
  return ((__int128)b.x-a.x)*((__int128)c.y-a.y) -
         ((__int128)b.y-a.y)*((__int128)c.x-a.x);
}
int linear(const vector<IPoint> &h, IPoint p) {
  bool boundary = false;
  for (int i = 0; i < (int)h.size(); ++i) {
    auto z = wide_turn(h[i],h[(i+1)%h.size()],p);
    if (z < 0) return 0;
    boundary |= z == 0;
  }
  return boundary ? 1 : 2;
}
void convex_audit() {
  const ll B = 1000000000000000000LL;
  vector<vector<IPoint>> hulls{
    {{-B,-B},{B,-B},{B,B},{-B,B}},
    {{B-4,B-4},{B,B-4},{B,B},{B-4,B}},
    {{-B,-B},{B,B-1},{B,B}},
    {{0,0},{1000000000,1},{999999999,1}}
  };
  for (auto h : hulls) for (int shift = 0; shift < (int)h.size();
                           ++shift) {
    for (int i = 0; i < (int)h.size(); ++i)
      require(wide_turn(h[i],h[(i+1)%h.size()],h[(i+2)%h.size()]) > 0,
              "test polygon is strictly CCW");
    vector<IPoint> qs = h;
    for (int i = 0; i < (int)h.size(); ++i) {
      auto a = h[i], b = h[(i+1)%h.size()];
      qs.pb({(a.x+b.x)/2,(a.y+b.y)/2});
      for (ll dx : {-1,0,1}) for (ll dy : {-1,0,1}) {
        IPoint p{a.x+dx,a.y+dy};
        if (abs(p.x) <= B && abs(p.y) <= B) qs.pb(p);
      }
    }
    for (int i = 0; i < 100; ++i)
      qs.pb({ll(rng()%(2*B+1))-B,ll(rng()%(2*B+1))-B});
    for (auto p : qs) if (pointInConvex(h,p) != linear(h,p)) {
      cerr << "query " << p.x << ' ' << p.y << " got "
           << pointInConvex(h,p) << " want " << linear(h,p) << '\n';
      for (auto v : h) cerr << v.x << ' ' << v.y << '\n';
      require(false, "128-bit fan against linear half-plane oracle");
    }
    rotate(h.begin(),h.begin()+1,h.end());
  }
  vector<Point> base{{0,0},{3,0},{6,0},{6,2},{6,4},
                     {3,4},{0,4},{0,2}};
  for (int repeat = 1; repeat <= 4; ++repeat) {
    vector<Point> p;
    for (auto q : base) for (int k = 0; k < repeat; ++k) p.pb(q);
    p.pb(p[0]);
    for (int direction = 0; direction < 2; ++direction) {
      for (int shift = 0; shift < (int)p.size(); ++shift) {
        vector<int> ix(p.size()); iota(ALL(ix),0);
        double want = 0;
        for (int i = 0; i < (int)p.size(); ++i)
          for (int j = i+1; j < (int)p.size(); ++j)
            for (int k = j+1; k < (int)p.size(); ++k)
              want = max(want,abs(Cross(p[j]-p[i],p[k]-p[i]))/2);
        require(ConvexHullMaxTriangleArea(p.data(),ix.data(),p.size())
                == want, "triangle duplicate/collinear plateaus");
        rotate(p.begin(),p.begin()+1,p.end());
      }
      reverse(ALL(p));
    }
  }
  for (int n = 0; n <= 12; ++n) {
    vector<Point> p(n,{7,-4}); vector<int> ix(n); iota(ALL(ix),0);
    require(ConvexHullMaxTriangleArea(p.data(),ix.data(),n) == 0,
            "triangle empty/identical vertices");
  }
}
void circle_audit() {
  // Exact contact/containment classes; area oracle uses long-double acos.
  for (int r = 0; r <= 4; ++r) for (int q = 0; q <= 4; ++q)
    for (int d = 0; d <= 12; ++d) {
      C a({0,0},r), b({double(d),0},q);
      int count = !d ? 0 : (d == r+q || d == abs(r-q)) ? 1 :
                  (d > r+q || d < abs(r-q)) ? 0 : 2;
      auto got = Intersect(a,b);
      require((int)got.size() == count, "all circle contact classes");
      for (auto p : got)
        require(abs((p-a.c).abs()-r) < 1e-12 &&
                abs((p-b.c).abs()-q) < 1e-12,
                "circle intersection on both boundaries");
      long double want;
      if (d >= r+q) want = 0;
      else if (d <= abs(r-q)) want = acosl(-1.L)*sq(min(r,q));
      else {
        long double x = (r*r + d*d - q*q)/(2.L*d);
        long double y = sqrtl(r*r-x*x);
        want = r*r*acosl(x/r) + q*q*acosl((d-x)/q) - d*y;
      }
      require(abs(IntersectArea(a,b)-want) < 1e-12 &&
              abs(IntersectArea(b,a)-want) < 1e-12,
              "circle area independent oracle and swapping");
    }
  // Independent long-double quadratic roots at well-conditioned scales.
  for (int tc = 0; tc < 10000; ++tc) {
    auto coord = [] { return double(int(rng()%41)-20); };
    P a{coord(),coord()}, b{coord(),coord()}, o{coord(),coord()};
    double r = rng()%8;
    if (tc%10 == 0) b = a;
    auto got = CircleCrossLine(a,b,o,r);
    long double ux = b.x-a.x, uy = b.y-a.y,
      vx = a.x-o.x, vy = a.y-o.y,
      A = ux*ux+uy*uy, B = ux*vx+uy*vy, C = vx*vx+vy*vy-r*r;
    vector<P> want;
    if (!A) { if (!C) want.pb(a); }
    else {
      long double D = B*B-A*C;
      if (D >= 0) {
        for (long double t : {(-B-sqrtl(D))/A,(-B+sqrtl(D))/A})
          if (0 <= t && t <= 1 &&
              (want.empty() || D != 0))
            want.pb({double(a.x+t*ux),double(a.y+t*uy)});
      }
    }
    require(got.size() == want.size(), "segment zero/tangent/root count");
    for (int i = 0; i < (int)got.size(); ++i)
      require((got[i]-want[i]).abs() < 1e-10,
              "independent segment roots and order");
  }
  for (double r : {0.,1.,3.}) {
    vector<P> p{{0,0},{4*r,0},{0,4*r}};
    vector<int> ix{0,1,2};
    do {
      vector<P> tri{p[ix[0]],p[ix[1]],p[ix[2]]};
      require(abs(AreaOfCircleTriangle(tri,r)-pi*r*r/4) < 1e-10,
              "circle quarter-disk all triangle permutations");
    } while (next_permutation(ALL(ix)));
    for (auto tri : vector<vector<P>>{
          {{0,0},{0,0},{0,0}}, {{-10,0},{0,0},{10,0}},
          {{2,3},{2,3},{-7,4}}})
      require(AreaOfCircleTriangle(tri,r) < 1e-10,
              "zero-area triangles and zero-radius circle");
  }
}
void else_audit() {
  int calls = 0;
  auto f = make_simpson([&](double x) { ++calls; return x*x*x*x; });
  double want = .2;
  require(abs(f.eval(0.,1.,1e-12)-want) < 1e-12 && calls > 5,
          "Simpson true adaptive refinement quartic");
  for (int k : {1,2,7,997}) {
    auto wave = make_simpson([](double x) { return cos(3*x); });
    require(abs(wave.eval2(-.7,2.1,1e-10,k) -
                (sin(6.3)-sin(-2.1))/3) < 1e-9,
            "Simpson oscillatory independent primitive");
  }
  for (int n : {0,1,2,3,8,9,14,15,100001,100002,100003}) {
    vector<int> q{INT_MIN,INT_MAX}; queens::solve(q,n);
    int m = n == 2 || n == 3 ? 0 : n;
    require(q.size() == m+2 && q[0] == INT_MIN && q[1] == INT_MAX,
            "queens large append prefix");
    vector<bool> col(n+1), up(2*n+1), down(2*n+1);
    for (int row = 0; row < m; ++row) {
      int c = q[row+2];
      require(1 <= c && c <= n && !col[c] && !up[row+c] &&
              !down[row-c+n], "queens linear conflict oracle");
      col[c] = up[row+c] = down[row-c+n] = true;
    }
  }
}
void magic_audit() {
  mt19937 gen(11);
  for (auto [l,r] : vector<pair<int,int>>{
        {INT_MIN,INT_MIN},{INT_MAX,INT_MAX},{INT_MIN,INT_MAX}}) {
    uniform_int_distribution<int> dist(l,r);
    for (int i = 0; i < 1000; ++i) {
      int x = dist(gen);
      require(l <= x && x <= r, "random full int range");
    }
  }
  tree_set t; t.insert(INT_MIN); t.insert(INT_MAX);
  require(t.order_of_key(INT_MIN) == 0 &&
          t.order_of_key(INT_MAX) == 1 &&
          t.find_by_order(2) == t.end(), "PBDS extreme ranks");
  rope<char> a("abc"), b = a; b.erase(0,b.size());
  b.insert(0,"z");
  require(string(a.begin(),a.end()) == "abc" &&
          string(b.begin(),b.end()) == "z", "rope erase whole snapshot");
}
int main() {
  number_audit(); sam_audit(); convex_audit(); circle_audit();
  else_audit(); magic_audit();
  cout << "g09 round11 adversarial audit PASS\n";
}
