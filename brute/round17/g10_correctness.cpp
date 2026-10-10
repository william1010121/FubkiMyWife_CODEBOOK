#include "../../codebook/1_Basic/Default_code.cpp"
namespace li {
constexpr ll INF = LLONG_MAX;
#include "../../codebook/3_Data_Structure/LiChaoST.cpp"
}
namespace sam {
constexpr int N = 8192, CNUM = 26;
#include "../../codebook/5_String/exSAM.cpp"
}
namespace tiny_sam {
constexpr int N = 1, CNUM = 1;
#include "../../codebook/5_String/exSAM.cpp"
}
namespace scc {
#include "../../codebook/2_Graph/SCC.cpp"
}
namespace fraction {
#include "../../codebook/9_Else/BinarySearchOnFraction.cpp"
}
namespace logs {
int fpow(int a, int e, int m) {
  ll r = 1 % m;
  for (; e; e >>= 1, a = 1LL * a * a % m)
    if (e & 1) r = r * a % m;
  return r;
}
#include "../../codebook/6_Math/DiscreteLog.cpp"
}
namespace ranks {
constexpr int N = 65537;
#include "../../codebook/3_Data_Structure/BIT_kth.cpp"
}
namespace walsh {
#include "../../codebook/7_Polynomial/Fast_Walsh_Transform.cpp"
}
namespace crt {
#include "../../codebook/6_Math/ax+by=gcd.cpp"
#include "../../codebook/6_Math/chineseRemainder.cpp"
}
namespace triangle {
struct Point {
  double x, y;
  Point(double x = 0, double y = 0) : x(x), y(y) {}
  Point operator+(Point p) const { return {x+p.x, y+p.y}; }
  Point operator-(Point p) const { return {x-p.x, y-p.y}; }
  Point operator*(double k) const { return {x*k, y*k}; }
  Point operator/(double k) const { return {x/k, y/k}; }
};
double Cross(Point a, Point b) { return a.x*b.y-a.y*b.x; }
double len(Point p) { return hypot(p.x, p.y); }
#include "../../codebook/8_Geometry/triangleCenter.cpp"
}
mt19937_64 rng(170010);
void require(bool ok, const char *why) {
  if (!ok) { cerr << "FAIL round17 g10: " << why << '\n'; exit(1); }
}

void check_li() {
  for (int n : {1,2,3,7,8,9,31,32,33}) {
    ll unit = LLONG_MAX / (4*n);
    vector<li::L> lines;
    for (int m = -2; m <= 2; ++m)
      for (int k = -2; k <= 2; ++k)
        lines.emplace_back(m*unit, k*unit, LLONG_MIN);
    lines.emplace_back(0, -LLONG_MAX, LLONG_MAX);
    for (int pass = 0; pass < 12; ++pass) {
      shuffle(ALL(lines), rng); li::LiChao tree(n);
      vector<ll> want(n, -li::INF);
      for (int x = 0; x < n; ++x)
        require(tree.query(x) == want[x], "Li Chao empty hull");
      for (auto line : lines) {
        tree.insert(line); tree.insert(line);
        for (int x = 0; x < n; ++x) {
          __int128 value = (__int128)line.m*x+line.k;
          require(-li::INF <= value && value <= li::INF,
                  "Li Chao legal wide test values");
          want[x] = max(want[x], ll(value));
          require(tree.query(x) == want[x], "Li Chao wide crossings");
        }
      }
      tree.insert(li::L(0, LLONG_MAX, 0));
      for (int x = 0; x < n; ++x)
        require(tree.query(x) == LLONG_MAX, "Li Chao INF endpoint");
    }
  }
  for (int n : {0,-1,INT_MIN}) {
    li::LiChao tree(n); tree.insert(li::L(LLONG_MIN, 0, 0));
    require(tree.query(LLONG_MIN) == -li::INF, "Li Chao n<=0");
  }
}

void check_sam() {
  static sam::exSAM s;
  vector<vector<string>> collections{{}, {"", ""}, {"z", "z", ""},
    {"abcz", "bcz", "cz", "z", "abcz", "zcb", ""},
    {"azazaza", "zazazaz", "za", "az", "a", "z"}};
  for (int tc = 0; tc < 200; ++tc) {
    vector<string> docs;
    for (int d = 0; d < 1+tc%9; ++d) {
      string text(rng()%17, 'a');
      for (char &c : text) c += rng()%26;
      docs.pb(text);
      if (d%3 == 0) docs.pb(text), docs.pb(text.substr(text.size()/2));
    }
    collections.pb(docs);
  }
  for (auto docs : collections) {
    map<string,int> want; int total = 0;
    for (auto &text : docs) {
      total += text.size();
      for (int i = 0; i < SZ(text); ++i)
        for (int len = 1; i+len <= SZ(text); ++len)
          ++want[text.substr(i,len)];
    }
    for (int pass = 0; pass < 3; ++pass) {
      shuffle(ALL(docs), rng); s.init();
      for (auto &text : docs) s.insert(text);
      s.build(); s.solve(); ll count = 0;
      require(s.cnt[0] == total, "SAM root occurrence count");
      for (int v = 1; v < s.tot; ++v) {
        require(s.len[s.link[v]] < s.len[v], "SAM suffix-link order");
        count += s.len[v]-s.len[s.link[v]];
      }
      require(count == ll(want.size()), "SAM no extra substrings");
      for (auto &[text,multiplicity] : want) {
        int state = 0;
        for (char c : text) {
          state = s.next[state][c-'a'];
          require(state != 0, "SAM recognizes every substring");
        }
        require(s.cnt[state] == multiplicity, "SAM shuffled documents");
      }
    }
  }
  tiny_sam::exSAM tiny;
  for (int pass = 0; pass < 10; ++pass) {
    tiny.init(); tiny.insert("");
    for (int i = 0; i < pass; ++i) tiny.insert("a");
    tiny.build(); tiny.solve();
    require(tiny.tot == (pass ? 2 : 1), "SAM exact two-state capacity");
    require(tiny.cnt[0] == pass && (!pass || tiny.cnt[1] == pass),
            "SAM N=CNUM=1 reuse");
  }
}

void check_scc() {
  scc::SCC empty(0); empty.solve(); empty.solve();
  require(empty.nscc == 0 && empty.stk.empty(), "SCC empty");
  for (int tc = 0; tc < 300; ++tc) {
    int n = 1+tc%80; vector<pii> edges;
    vector<vector<bool>> reach(n, vector<bool>(n));
    for (int u = 0; u < n; ++u) reach[u][u] = true;
    for (int k = 0; k < n*(tc%5); ++k) {
      int u = rng()%n, v = rng()%n;
      edges.pb({u,v}); edges.pb({u,v}); reach[u][v] = true;
    }
    for (int k = 0; k < n; ++k) for (int u = 0; u < n; ++u)
      for (int v = 0; v < n; ++v)
        reach[u][v] = reach[u][v] || (reach[u][k] && reach[k][v]);
    for (bool transpose : {false,true}) {
      shuffle(ALL(edges), rng); scc::SCC s(n);
      for (auto [u,v] : edges) {
        if (transpose) swap(u,v); s.add_edge(u,v);
      }
      s.solve(); auto ids = s.bln; s.solve();
      require(ids == s.bln && s.stk.empty(), "SCC idempotent solve");
      set<int> components(ALL(s.bln));
      require(SZ(components) == s.nscc, "SCC contiguous component count");
      for (int u = 0; u < n; ++u) {
        require(!s.instack[u], "SCC clears active stack");
        for (int v = 0; v < n; ++v)
          require((s.bln[u] == s.bln[v]) == (reach[u][v] && reach[v][u]),
                  "SCC Floyd closure / transpose");
        for (int v : s.G[u])
          require(s.bln[u] >= s.bln[v], "SCC reverse topology");
      }
    }
  }
}

void check_fraction() {
  using namespace fraction;
  for (ll n = 1; n <= 15; ++n) {
    vector<Q> ordered{{1,0}};
    for (ll q = 1; q <= n; ++q) for (ll p = 0; p <= n; ++p)
      if (gcd(p,q) == 1) ordered.pb({p,q});
    sort(ALL(ordered), [](Q a,Q b) { return a.p*b.q < b.p*a.q; });
    for (int i = 0; i < SZ(ordered); ++i)
      for (bool strict : {false,true}) {
        Q threshold = ordered[i];
        if (strict && !threshold.q) continue;
        auto pred = [&](Q a) {
          if (!a.q) return true;
          ll diff = a.p*threshold.q-threshold.p*a.q;
          return strict ? diff > 0 : diff >= 0;
        };
        Q got = frac_bs(n,pred), want = ordered[i+strict];
        require(got.p == want.p && got.q == want.q,
                "fraction every bounded rational / strict successor");
      }
  }
  for (ll n : {1LL, 1000000000LL, (1LL<<59)-1}) {
    int calls = 0;
    auto pred = [&](Q a) { ++calls; return (__int128)a.p*n >= a.q; };
    Q got = frac_bs(n,pred);
    require(got.p == 1 && got.q == n && calls < 260,
            "fraction extreme accelerated run");
    auto positive = [](Q a) { return a.p > 0; };
    got = frac_bs(n,positive);
    require(got.p == 1 && got.q == n, "fraction strictly above zero");
    got = frac_bs(n,[](Q a) { return a.q == 0; });
    require(got.p == 1 && got.q == 0, "fraction only infinity");
  }
}

void check_logs() {
  // Full orbits are small even though the moduli approach INT_MAX.
  for (int k : {1,10,20,24}) for (int p : {3,7,31,101,127}) {
    int m = (1<<k)*p;
    for (int x : {2,6,2*p,m-1}) {
      x %= m; // Canonical bases, including zero for k=1.
      unordered_map<int,int> first; vector<int> orbit;
      for (int y = 1; !first.count(y); y = 1LL*y*x%m) {
        first[y] = SZ(orbit); orbit.pb(y);
      }
      vector<int> targets{0,1,m/2,m-1};
      for (int i : {k-1,k,k+1,99,100,101,SZ(orbit)-1})
        targets.pb(orbit[min(i,SZ(orbit)-1)]);
      for (int y : targets) {
        auto it = first.find(y);
        require(logs::DiscreteLog(x,y,m) ==
                (it == first.end() ? -1 : it->second),
                "DLog large mixed prime-power exact orbit");
      }
    }
  }
  for (int m = 1; m <= 150; ++m)
    for (int x = 0; x < m; ++x) {
      vector<int> first(m,-1); int value = 1%m, e = 0;
      while (first[value] < 0)
        first[value] = e++, value = 1LL*value*x%m;
      for (int y = 0; y < m; ++y)
        require(logs::DiscreteLog(x,y,m) == first[y],
                "DLog all small residues / least exponent");
    }
}

void check_bit() {
  using namespace ranks;
  vector<int> positions{1,2,3,31,32,33,32767,32768,32769,
                        65535,65536,65537};
  vector<int> a(N+1); fill_n(bit, N+1, 0);
  auto add = [&](int i,int value) {
    a[i] += value;
    for (; i <= N; i += i&-i) bit[i] += value;
  };
  add(N,INT_MAX);
  for (int tc = 0; tc < 1000; ++tc) {
    int from = positions[rng()%SZ(positions)];
    int to = positions[rng()%SZ(positions)];
    int mass = a[from] ? rng()%(1LL+a[from]) : 0;
    add(from,-mass); add(to,mass);
    ll sum = 0;
    for (int pos : positions) {
      if (a[pos]) {
        require(query_kth(sum+1) == pos, "BIT first rank of a mass");
        require(query_kth(sum+a[pos]) == pos, "BIT last rank of a mass");
      }
      sum += a[pos];
    }
    require(sum == INT_MAX, "BIT legal total at INT_MAX");
    require(query_kth(INT_MAX) <= N, "BIT endpoint capacity");
  }
}

void check_walsh() {
  using namespace walsh;
  for (int value : {INT_MIN,INT_MAX,-1,0,1}) {
    int got = value;
    fwt(&got,1,1); fwt(&got,1,-1);
    require(got == value, "FWT singleton full int range");
    int one = 1; subset_convolution(&got,&one,&got,0);
    require(got == value, "subset singleton full int range / alias");
  }
  for (int L : {8,1,7,0,5,2,0,8}) {
    int n = 1<<L; vector<int> buffer(2*n+3);
    for (int &x : buffer) x = int(rng()%7)-3;
    int *a = buffer.data()+1, *b = a+n/2, *c = a-1;
    vector<int> aa(a,a+n), bb(b,b+n), want(n);
    for (int mask = 0; mask < n; ++mask) {
      int sub = mask;
      do {
        want[mask] += aa[sub]*bb[mask^sub];
        sub = (sub-1)&mask;
      } while (sub != mask);
    }
    for (int i = 0; i <= L; ++i)
      fill_n(f[i],n,INT_MIN), fill_n(g[i],n,INT_MAX),
        fill_n(h[i],n,INT_MIN);
    subset_convolution(a,b,c,L);
    require(equal(ALL(want),c), "subset clears scratch / overlap");
    vector<int> transformed = aa;
    fwt(transformed.data(),n,1);
    for (int mask = 0; mask < n; ++mask) {
      int sum = 0;
      for (int sub = 0; sub < n; ++sub)
        if ((sub&mask) == sub) sum += aa[sub];
      require(transformed[mask] == sum, "FWT independent zeta sum");
    }
    fwt(transformed.data(),n,-1);
    require(transformed == aa, "FWT signed inverse");
  }
  vector<int> extreme{INT_MAX,-INT_MAX,0,0};
  auto saved = extreme; fwt(extreme.data(),4,1);
  fwt(extreme.data(),4,-1);
  require(extreme == saved, "FWT near int limits / cancellation");
}

ll crt_oracle(ll x,ll a,ll y,ll b) {
  // Independent iterative wide Bezout, normalizing residues first.
  __int128 r0 = a, r1 = b, s0 = 1, s1 = 0;
  while (r1) {
    __int128 q = r0/r1;
    auto r = r0-q*r1, s = s0-q*s1;
    r0 = r1, r1 = r, s0 = s1, s1 = s;
  }
  __int128 xx = x%a, yy = y%b;
  if (xx < 0) xx += a; if (yy < 0) yy += b;
  __int128 d = yy-xx, modulus = b/r0;
  if (d%r0) return -1;
  __int128 step = (d/r0*s0)%modulus;
  if (step < 0) step += modulus;
  __int128 period = (__int128)a*modulus;
  return (xx+a*step)%period;
}
void check_crt() {
  vector<pll> moduli{{1,1}, {1,LLONG_MAX}, {LLONG_MAX,1},
    {LLONG_MAX,LLONG_MAX}, {LLONG_MAX,7}, {7,LLONG_MAX},
    {3037000499LL,3037000493LL}, {1836311903LL,2971215073LL},
    {1LL<<61,1LL<<60}, {3*(1LL<<60),1LL<<61}};
  for (int tc = 0; tc < 10000; ++tc)
    moduli.pb({1+ll(rng()%3037000499ULL),
               1+ll(rng()%3037000499ULL)});
  ll extremes[] = {LLONG_MIN,LLONG_MIN+1,-1,0,1,LLONG_MAX};
  for (auto [a,b] : moduli) for (int i = 0; i < 6; ++i) {
    ll x = extremes[i], y = i%2 ? ll(rng()) : extremes[5-i];
    ll want = crt_oracle(x,a,y,b);
    require(crt::solve(x,a,y,b) == want && crt::solve(y,b,x,a) == want,
            "CRT full-width residues / wide coprime and dividing moduli");
    if (want >= 0) {
      require(((__int128)want-x)%a == 0 && ((__int128)want-y)%b == 0,
              "CRT both congruences");
      require(want < (__int128)a*(b/gcd(a,b)), "CRT least residue");
    }
  }
}

void check_triangle() {
  using namespace triangle;
  using LD = long double;
  auto close = [](double got,LD want) {
    return isfinite(got) && abs(LD(got)-want) <= 1e-9L*(1+abs(want));
  };
  vector<array<Point,3>> cases{{Point(0,0),Point(3,0),Point(0,4)},
    {Point(0,0),Point(2,0),Point(1,1)},
    {Point(0,0),Point(1000000,1),Point(1000001,1)},
    {Point(0,0),Point(1000000,1),Point(2000001,2)}};
  for (int tc = 0; tc < 300; ++tc) {
    array<Point,3> ps;
    for (auto &p : ps) p = {double(int(rng()%201)-100),
                            double(int(rng()%201)-100)};
    // Collinear and duplicate-vertex circumcenters are outside contract.
    if (Cross(ps[1]-ps[0],ps[2]-ps[0])) cases.pb(ps);
  }
  for (auto ps : cases) for (int exponent : {-100,0,100}) {
    double scale = ldexp(1.,exponent);
    auto scaled = ps; for (auto &p : scaled) p = p*scale;
    array<int,3> perm{0,1,2};
    do {
      Point a = scaled[perm[0]], b = scaled[perm[1]], c = scaled[perm[2]];
      LD ux = LD(b.x)-a.x, uy = LD(b.y)-a.y;
      LD vx = LD(c.x)-a.x, vy = LD(c.y)-a.y;
      LD x = ux*ux+uy*uy, y = vx*vx+vy*vy, d = 2*(ux*vy-uy*vx);
      LD ox = a.x+(vy*x-uy*y)/d, oy = a.y+(ux*y-vx*x)/d;
      Point o = TriangleCircumCenter(a,b,c);
      require(close(o.x/scale,ox/scale) && close(o.y/scale,oy/scale),
              "triangle circumcenter / skinny permutations");
      Point mass = TriangleMassCenter(a,b,c);
      require(close(mass.x/scale,(LD(a.x)+b.x+c.x)/(3*scale)) &&
              close(mass.y/scale,(LD(a.y)+b.y+c.y)/(3*scale)),
              "triangle centroid");
      Point ortho = TriangleOrthoCenter(a,b,c);
      require(close(ortho.x/scale,(LD(a.x)+b.x+c.x-2*ox)/scale) &&
              close(ortho.y/scale,(LD(a.y)+b.y+c.y-2*oy)/scale),
              "triangle orthocenter");
      LD la = hypotl(LD(b.x)-c.x,LD(b.y)-c.y);
      LD lb = hypotl(LD(a.x)-c.x,LD(a.y)-c.y);
      LD lc = hypotl(LD(a.x)-b.x,LD(a.y)-b.y);
      Point inner = TriangleInnerCenter(a,b,c);
      require(close(inner.x/scale,(a.x*la+b.x*lb+c.x*lc)/
                                  ((la+lb+lc)*scale)) &&
              close(inner.y/scale,(a.y*la+b.y*lb+c.y*lc)/
                                  ((la+lb+lc)*scale)),
              "triangle incenter independent long-double weights");
    } while (next_permutation(ALL(perm)));
  }
}

int main() {
  check_li(); check_sam(); check_scc(); check_fraction(); check_logs();
  check_bit(); check_walsh(); check_crt(); check_triangle();
  cout << "PASS round17 g10: nine adversarial template audits\n";
}
