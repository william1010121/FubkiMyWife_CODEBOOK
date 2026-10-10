#include "../../codebook/1_Basic/Default_code.cpp"
using u64 = unsigned long long;
namespace pal {
#include "../../codebook/5_String/PalTree.cpp"
}
namespace man {
constexpr int MAXN = 129;
#include "../../codebook/5_String/Manacher.cpp"
}
namespace xb {
#include "../../codebook/3_Data_Structure/XOR_Basis.cpp"
}
namespace det {
constexpr int MAXN = 24;
ll P;
#include "../../codebook/6_Math/Determinant.cpp"
}
namespace st {
constexpr int N = 3, T = 3, INF = INT_MAX;
#include "../../codebook/2_Graph/MinimumSteinerTree.cpp"
}
namespace geo {
constexpr double eps = 1e-9;
int sign(double x) { return (x > eps) - (x < -eps); }
#include "../../codebook/8_Geometry/_basic.cpp"
using pt = P;
double norm2(P p) { return p * p; }
struct circle { P c; double r; };
#include "../../codebook/8_Geometry/MinimumEnclosingCircle.cpp"
}
#include "../../codebook/9_Else/cyc_tsearch.cpp"
#include "../../codebook/9_Else/HilbertCurve.cpp"

void require(bool ok, const char *why) {
  if (!ok) { cerr << "FAIL round17 g09: " << why << '\n'; exit(1); }
}
mt19937_64 rng(1709);

bool palindrome(const string &s, int l, int r) {
  while (l < r) if (s[l++] != s[r--]) return false;
  return true;
}
void strings_audit() {
  pal::palindromic_tree tree;
  auto check = [&](string s) {
    tree.clear(); map<string, int> occ; vector<string> value(2);
    for (int r = 0; r < SZ(s); ++r) {
      int num = 0, longest = 0;
      for (int l = 0; l <= r; ++l) if (palindrome(s, l, r)) {
        ++occ[s.substr(l, r-l+1)], ++num;
        longest = max(longest, r-l+1);
      }
      tree.add(s[r]);
      require(tree.St[tree.last].num == num &&
              tree.St[tree.last].len == longest, "eertree suffixes");
      if (SZ(value) < SZ(tree.St))
        value.pb(s.substr(r-longest+1, longest));
    }
    tree.count(); require(tree.size() == SZ(occ), "distinct nodes");
    for (int v = 2; v < SZ(value); ++v)
      require(tree.St[v].cnt == occ.at(value[v]), "occurrences");
    require(tree.St[0].cnt == SZ(s) && tree.St[1].cnt == SZ(s),
            "root counts after reuse");
    fill(begin(man::z), end(man::z), -77); man::Manacher(s);
    int n = SZ(s);
    for (int c = 0; c <= 2*n; ++c) {
      int want = 1;
      while (c-want >= 0 && c+want <= 2*n) {
        if ((c-want)%2 && s[(c-want)/2] != s[(c+want)/2]) break;
        ++want;
      }
      require(man::z[c] == want, "Manacher direct radii");
    }
    for (int i = 2*n+1; i < man::MAXN; ++i)
      require(man::z[i] == -77, "inactive radii untouched");
  };
  // All three-letter words, then repeatedly switch alphabet and length.
  for (int n = 0, total = 1; n <= 9; ++n, total *= 3)
    for (int code = 0; code < total; ++code) {
      int x = code; string s(n, 'a');
      for (char &c : s) c = "amz"[x%3], x /= 3;
      check(s);
    }
  for (int t = 0; t < 1000; ++t) {
    string s(rng()%65, 'a');
    for (char &c : s) c += rng()%26;
    if (t%3 == 0) for (int i = 0; i < SZ(s)/2; ++i)
      s[SZ(s)-1-i] = s[i];
    check(s);
  }
  check(string(64, 'z')); check("");
  // Every possible byte is a legal Manacher input, including separator.
  for (int byte = 0; byte < 256; ++byte) {
    man::Manacher(string(64, char(byte)));
    for (int i = 0; i <= 128; ++i)
      require(man::z[i] == min(i, 128-i)+1, "equal byte radii");
  }
}

void determinant_audit() {
  // Exhaustive 2x2 matrices over rings, including zero divisors.
  for (ll p = 1; p <= 12; ++p) {
    det::P = p;
    for (ll a = 0; a < p; ++a) for (ll b = 0; b < p; ++b)
      for (ll c = 0; c < p; ++c) for (ll d = 0; d < p; ++d) {
        det::Matrix m{}; m.n = m.m = 2;
        m.M[0][0] = a, m.M[0][1] = b;
        m.M[1][0] = c, m.M[1][1] = d;
        require(m.det() == ((a*d-b*c)%p+p)%p, "2x2 ring oracle");
        for (int i = 0; i < 2; ++i) for (int j = 0; j < 2; ++j)
          require(m.M[i][j] >= 0 && m.M[i][j] < p,
                  "canonical destructive output");
      }
  }
  for (ll p : {1LL, 8LL, 2147483646LL, 2147483647LL,
               2147483648LL, LLONG_MAX}) {
    det::P = p;
    for (int n = 0; n <= det::MAXN; ++n) for (int t = 0; t < 20; ++t) {
      det::Matrix m{}; m.n = m.m = n; ll want = 1%p;
      for (int i = 0; i < n; ++i) for (int j = i; j < n; ++j)
        m.M[i][j] = t%2 ? (p-1) : ll(rng()%p);
      for (int i = 0; i < n; ++i)
        want = (__int128)want*m.M[i][i]%p;
      for (int i = 0; i < n; ++i) {
        int j = rng()%n;
        auto before = m;
        require(m.row_swap(i, j) == (i != j), "swap return parity");
        for (int k = 0; k < n; ++k)
          require(m.M[i][k] == before.M[j][k] &&
                  m.M[j][k] == before.M[i][k], "full row swap");
        if (i != j && want) want = p-want;
      }
      require(m.det() == want, "permuted triangular determinant");
    }
  }
}

void steiner_audit() {
  static st::SteinerTree tree;
  array<pii, 6> arcs{{{0,1},{0,2},{1,0},{1,2},{2,0},{2,1}}};
  int choices[] = {0, 1, st::INF-1};
  // All 3-vertex digraphs with absent/zero/near-INF edges and all costs.
  for (int code = 0; code < 729; ++code)
    for (int cost = 0; cost < 27; ++cost) {
      tree.init(3); int x = cost, y = code;
      for (int i = 0; i < 3; ++i, x /= 3)
        tree.vcst[i] = choices[x%3];
      vector<tuple<int,int,int>> edges;
      for (auto [u,v] : arcs) {
        int c = y%3; y /= 3;
        if (c) edges.emplace_back(u,v,c == 1 ? 0 : st::INF-1);
      }
      int expected[8][3];
      for (auto &row : expected) fill(begin(row), end(row), st::INF);
      // Selected-edge-subset reachability, independent of subset DP.
      for (int subset = 0; subset < (1<<SZ(edges)); ++subset) {
        int reach[3] = {1,2,4}, used = 0; ll edgecost = 0;
        for (int i = 0; i < SZ(edges); ++i) if (subset>>i&1) {
          auto [u,v,w] = edges[i];
          reach[u] |= 1<<v, used |= (1<<u)|(1<<v), edgecost += w;
        }
        for (int k = 0; k < 3; ++k) for (int i = 0; i < 3; ++i)
          if (reach[i]>>k&1) reach[i] |= reach[k];
        for (int mask = 0; mask < 8; ++mask)
          for (int sink = 0; sink < 3; ++sink) {
            bool ok = true; ll sum = edgecost;
            for (int i = 0; i < 3; ++i) {
              if (mask>>i&1) ok &= bool(reach[i]>>sink&1);
              if ((used|mask|(1<<sink))>>i&1) sum += tree.vcst[i];
            }
            if (ok) expected[mask][sink] = min<ll>(
                expected[mask][sink], sum);
          }
      }
      for (auto [u,v,w] : edges) tree.add_edge(u,v,w);
      int got = tree.solve({2,0,1,2,1,0});
      for (int mask = 0; mask < 8; ++mask)
        for (int sink = 0; sink < 3; ++sink)
          require(tree.dp[mask][sink] == expected[mask][sink],
                  "all Steiner DP states vs edge subsets");
      require(got == *min_element(expected[7], expected[7]+3),
              "Steiner optimum");
    }
  tree.init(0); require(tree.solve({}) == 0, "empty Steiner reuse");
  tree.init(1); tree.vcst[0] = st::INF-1;
  tree.add_edge(0,0,st::INF-1);
  require(tree.solve({0,0}) == st::INF-1, "singleton self loop");
}

void basis_audit() {
  for (int t = 0; t < 1000; ++t) {
    fill(begin(xb::basis), end(xb::basis), 0);
    vector<u64> generators(t%9); set<u64> span{0};
    for (auto &x : generators) {
      x = t%2 ? rng() : (1ULL<<(rng()%64));
      auto old = span;
      for (u64 y : old) span.insert(x^y);
      xb::ins(x);
    }
    array<u64,64> before;
    copy(begin(xb::basis), end(xb::basis), before.begin());
    for (u64 x : span) {
      xb::ins(x);
      require(equal(ALL(before), begin(xb::basis)),
              "every span member is discarded");
    }
    for (int i = 0; i < 64; ++i)
      require(!xb::basis[i] || __lg(xb::basis[i]) == i,
              "highest-bit pivots");
    int rank = count_if(ALL(before), [](u64 x) { return x != 0; });
    require(span.size() == (1ULL<<rank), "independent span size");
  }
}

void cyclic_audit() {
  for (int n = 1; n <= 9; ++n) {
    vector<int> a(n); iota(ALL(a), 0);
    do {
      int changes = 0;
      for (int i = 0; i < n; ++i)
        changes += (a[i] < a[(i+1)%n]) !=
                   (a[(i+1)%n] < a[(i+2)%n]);
      if (n > 1 && changes != 2) continue;
      int calls = 0;
      int got = cyc_tsearch(n, [&](int u, int v) {
        require(0 <= u && u < n && 0 <= v && v < n,
                "cyclic in-range comparisons");
        ++calls; return a[u] < a[v];
      });
      require(a[got] == 0, "all legal cyclic permutations");
      require(n > 1 || calls == 0, "singleton comparator unused");
    } while (next_permutation(ALL(a)));
  }
}

pii inverse_hilbert(int n, ll d) {
  int x = 0, y = 0;
  for (int s = 1; s < n; s *= 2, d /= 4) {
    int rx = d/2&1, ry = (d^rx)&1;
    if (!ry) {
      if (rx) x = s-1-x, y = s-1-y;
      swap(x,y);
    }
    x += s*rx, y += s*ry;
  }
  return {x,y};
}
void hilbert_audit() {
  for (int k = 0; k <= 30; ++k) {
    int n = 1<<k; ll total = 1LL*n*n;
    for (int t = 0; t < 2000; ++t) {
      ll d = rng()%total; auto [x,y] = inverse_hilbert(n,d);
      require(hilbert(n,x,y) == d, "random full-width round trip");
      if (d+1 < total) {
        auto [a,b] = inverse_hilbert(n,d+1);
        require(abs(x-a)+abs(y-b) == 1, "random adjacency");
      }
    }
  }
}

void circle_check(vector<geo::P> points) {
  using LD = long double;
  LD best = numeric_limits<LD>::infinity();
  auto candidate = [&](LD x, LD y) {
    LD r2 = 0;
    for (auto p : points)
      r2 = max(r2, (p.x-x)*(p.x-x)+(p.y-y)*(p.y-y));
    best = min(best,r2);
  };
  if (points.empty()) best = 0;
  for (auto p : points) candidate(p.x,p.y);
  for (int i = 0; i < SZ(points); ++i)
    for (int j = 0; j < i; ++j) {
      candidate((LD(points[i].x)+points[j].x)/2,
                (LD(points[i].y)+points[j].y)/2);
      for (int k = 0; k < j; ++k) {
        LD x = LD(points[j].x)-points[i].x;
        LD y = LD(points[j].y)-points[i].y;
        LD u = LD(points[k].x)-points[i].x;
        LD v = LD(points[k].y)-points[i].y, d = 2*(x*v-y*u);
        if (d) candidate(points[i].x+((x*x+y*y)*v-(u*u+v*v)*y)/d,
                         points[i].y+((u*u+v*v)*x-(x*x+y*y)*u)/d);
      }
    }
  auto c = geo::min_enclosing(points);
  LD got = LD(c.r)*c.r;
  require(isfinite(c.r) && isfinite(c.c.x) && isfinite(c.c.y),
          "finite degenerate MEC");
  require(abs(got-best) <= max(best,LD(1e-200))*1e-10,
          "MEC minimum support oracle");
  for (auto p : points)
    require(geo::norm2(p-c.c) <= got*(1+1e-10), "MEC containment");
}
void circle_audit() {
  vector<geo::P> grid;
  for (int x = -1; x <= 1; ++x) for (int y = -1; y <= 1; ++y)
    grid.pb({double(x),double(y)});
  vector<geo::P> p;
  auto enumerate = [&](auto &&self, int first) -> void {
    for (int e : {-100,0,100}) {
      auto a = p;
      for (auto &q : a) q = (q+geo::P(64,-64))*ldexp(1.,e);
      circle_check(a);
    }
    if (p.size() == 5) return;
    for (int i = first; i < SZ(grid); ++i) {
      p.pb(grid[i]); self(self,i); p.pop_back();
    }
  };
  enumerate(enumerate,0);
}

void primes_audit() {
  // Independent trial division for every listed value below 2^31.
  ifstream in("codebook/6_Math/Primes.cpp");
  string text((istreambuf_iterator<char>(in)), {});
  vector<u64> values;
  for (size_t i = 0; i < text.size();) {
    if (!isdigit((unsigned char)text[i])) { ++i; continue; }
    __uint128_t x = 0;
    while (i < text.size() && isdigit((unsigned char)text[i]))
      x = x*10 + text[i++]-'0';
    require(x <= ULLONG_MAX, "prime list unsigned range");
    values.pb(u64(x));
  }
  require(values.size() == 28 && values.back() > u64(LLONG_MAX),
          "full list with unsigned last constant");
  for (u64 x : values) if (x <= INT_MAX)
    for (u64 d = 2; d*d <= x; ++d)
      require(x%d != 0, "trial division of listed primes");
}

int main() {
  strings_audit(); determinant_audit(); steiner_audit(); basis_audit();
  cyclic_audit(); hilbert_audit(); circle_audit(); primes_audit();
  cout << "PASS round17 g09: all nine assigned templates\n";
}
