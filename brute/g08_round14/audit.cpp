#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll, ll>;
#define X first
#define Y second
#define SZ(a) int((a).size())
#define ALL(a) (a).begin(), (a).end()
#define pb push_back

namespace geo {
const double eps = 1e-9;
int sign(double x) { return abs(x) < eps ? 0 : x > 0 ? 1 : -1; }
#include "../../codebook/8_Geometry/_basic.cpp"
using pt = P;
double norm2(P p) { return p * p; }
struct circle { P c; double r; };
#include "../../codebook/8_Geometry/MinimumEnclosingCircle.cpp"
}
namespace li {
const ll INF = LLONG_MAX;
#include "../../codebook/3_Data_Structure/LiChaoST.cpp"
}
namespace km {
const int N = 8;
const ll INF = 1LL << 60;
#include "../../codebook/4_Flow_Matching/Kuhn_Munkres.cpp"
}
namespace qr {
#include "../../codebook/6_Math/QuadraticResidue.cpp"
}
namespace lc {
#include "../../codebook/6_Math/ax+by=gcd.cpp"
#include "../../codebook/6_Math/Linear_Congruence.cpp"
}
namespace cdq {
#include "../../codebook/9_Else/CDQ_Dominance.cpp"
}
namespace hld {
const int N = 128;
#include "../../codebook/3_Data_Structure/Dynamic_2D_Segment_Tree.cpp"
#include "../../codebook/3_Data_Structure/Heavy_light_Decomposition.cpp"
}
namespace cir {
const int N = 8;
const ll INF = 1LL << 60;
#include "../../codebook/4_Flow_Matching/MinCostCirculation.cpp"
}
namespace db {
#include "../../codebook/5_String/De_Bruijn_sequence.cpp"
}

void require(bool ok, const char *why) {
  if (!ok) throw runtime_error(why);
}
mt19937 rng(1408);

// Independent long-double enumeration of all possible MEC supports.
void check_circle(vector<geo::P> a) {
  using LD = long double;
  LD best = numeric_limits<LD>::infinity();
  auto candidate = [&](LD x, LD y) {
    LD r2 = 0;
    for (auto p : a) r2 = max(r2, (p.x-x)*(p.x-x)+(p.y-y)*(p.y-y));
    best = min(best, r2);
  };
  if (a.empty()) best = 0;
  for (auto p : a) candidate(p.x, p.y);
  for (int i = 0; i < SZ(a); ++i) for (int j = 0; j < i; ++j) {
    candidate((LD(a[i].x)+a[j].x)/2, (LD(a[i].y)+a[j].y)/2);
    for (int k = 0; k < j; ++k) {
      LD x = LD(a[j].x)-a[i].x, y = LD(a[j].y)-a[i].y;
      LD u = LD(a[k].x)-a[i].x, v = LD(a[k].y)-a[i].y;
      LD det = 2*(x*v-y*u); if (!det) continue;
      candidate(a[i].x+((x*x+y*y)*v-(u*u+v*v)*y)/det,
                a[i].y+((u*u+v*v)*x-(x*x+y*y)*u)/det);
    }
  }
  auto c = geo::min_enclosing(a);
  LD r2 = LD(c.r)*c.r;
  require(isfinite(c.r) && c.r >= 0, "MEC finite radius");
  require(abs(r2-best) <= max(best, LD(1e-200))*1e-9,
          "MEC support oracle");
  for (auto p : a)
    require(geo::norm2(p-c.c) <= double(r2*(1+1e-9)),
            "MEC containment");
  if (a.empty()) require(c.c.x == 0 && c.c.y == 0, "MEC empty");
}
void audit_circle() {
  for (int t = 0; t < 1000; ++t) {
    vector<geo::P> a(t%10);
    for (auto &p : a) p = {double(int(rng()%201)-100),
                           double(int(rng()%201)-100)};
    if (t%4 == 0) for (auto &p : a) p.y = 3*p.x+7;
    if (a.size() > 1 && t%3 == 0) a.back() = a.front();
    for (int e : {-100, 0, 100}) {
      auto b = a;
      for (auto &p : b) p = p*ldexp(1., e);
      check_circle(b);
    }
  }
  check_circle({{0,0}, {65536,1}, {131073,2}});
  check_circle({{0,0}, {65536,0}, {32768,65536}});
  cout << "PASS MEC 3002 scaled/degenerate support-oracle cases\n";
}
void audit_li() {
  for (int n = 1; n <= 17; ++n) for (int t = 0; t < 100; ++t) {
    li::LiChao tree(n); vector<li::L> lines;
    for (int i = 0; i < 40; ++i) {
      ll m = int(rng()%41)-20, k = int(rng()%101)-50;
      if (i >= 30 && i%5 == 0) m = 0, k = i%2 ? li::INF : -li::INF;
      lines.emplace_back(m, k, i%2 ? LLONG_MIN : i);
      tree.insert(lines.back());
      for (int x = 0; x < n; ++x) {
        ll want = -li::INF;
        for (auto l : lines) want = max(want, l.at(x));
        require(tree.query(x) == want, "Li Chao prefix oracle");
      }
    }
  }
  for (int n : {0, -1, INT_MIN}) {
    li::LiChao tree(n); tree.insert(li::L(1, 2, 0));
    for (ll x : {LLONG_MIN, 0LL, LLONG_MAX})
      require(tree.query(x) == -li::INF, "Li Chao empty domain");
  }
  li::LiChao tree(2);
  tree.insert(li::L(LLONG_MAX/2, -LLONG_MAX/2, 0));
  tree.insert(li::L(-LLONG_MAX/2, LLONG_MAX/2, LLONG_MIN));
  require(tree.query(0) == LLONG_MAX/2 && tree.query(1) == 0,
          "Li Chao wide crossing");
  cout << "PASS Li Chao extreme sentinels/IDs and prefix oracles\n";
}
void audit_km() {
  km::KM m; m.init(0); require(m.solve() == 0, "KM empty");
  ll b = (km::INF-1)/6;
  ll options[] = {-km::INF, -b, 0, b};
  for (int mask = 0; mask < (1<<18); ++mask) {
    ll w[3][3]; int code = mask; m.init(3);
    for (auto &row : w) for (auto &x : row)
      x = options[code%4], code /= 4;
    vector<int> p{0,1,2}; ll best = LLONG_MIN;
    do {
      ll sum = 0; bool ok = true;
      for (int i = 0; i < 3; ++i)
        if (w[i][p[i]] == -km::INF) ok = false;
        else sum += w[i][p[i]];
      if (ok) best = max(best, sum);
    } while (next_permutation(ALL(p)));
    if (best == LLONG_MIN) continue; // Perfect matching precondition.
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j)
      if (w[i][j] != -km::INF) m.add_edge(i, j, w[i][j]);
    for (int rep = 0; rep < 2; ++rep) {
      require(m.solve() == best, "KM exhaustive sentinel oracle");
      for (int i = 0; i < 3; ++i)
        require(m.fr[m.fl[i]] == i && w[i][m.fl[i]] != -km::INF,
                "KM reciprocal real matching");
    }
  }
  cout << "PASS KM all 262144 3x3 four-valued matrices\n";
}
ll modpow(ll a, int e, int m) {
  ll r = 1;
  for (; e; e >>= 1, a = a*a%m) if (e&1) r = r*a%m;
  return r;
}
int jacobi_oracle(int a, int m) {
  ll x = (ll(a)%m+m)%m; int result = 1;
  for (int p = 3; 1LL*p*p <= m; p += 2) if (m%p == 0) {
    int e = 0; do m /= p, ++e; while (m%p == 0);
    ll leg = x%p ? modpow(x%p, (p-1)/2, p) : 0;
    if (!leg) return 0;
    if ((e&1) && leg == p-1) result = -result;
  }
  if (m > 1) {
    ll leg = x%m ? modpow(x%m, (m-1)/2, m) : 0;
    if (!leg) return 0;
    if (leg == m-1) result = -result;
  }
  return result;
}
void audit_qr() {
  for (int t = 0; t < 2000; ++t) {
    int m = int(rng()&INT_MAX)|1;
    int a = t%3 == 0 ? INT_MIN : t%3 == 1 ? INT_MAX : int(rng());
    require(qr::Jacobi(a, m) == jacobi_oracle(a, m),
            "Jacobi independent factorization");
  }
  srand(1408);
  for (int p : {2,3,5,17,97,65537,998244353,1000000007,INT_MAX})
    for (int t = 0; t < 2000; ++t) {
      int a = t%4 == 0 ? INT_MIN : t%4 == 1 ? INT_MAX : int(rng());
      int x = (ll(a)%p+p)%p, root = qr::QuadraticResidue(a, p);
      if (p > 2 && x && modpow(x, (p-1)/2, p) == p-1)
        require(root == -1, "Cipolla nonresidue");
      else require(root >= 0 && root < p && 1LL*root*root%p == x,
                   "Cipolla independent Euler oracle");
    }
  cout << "PASS Jacobi factorization and 18000 Cipolla signed cases\n";
}
void audit_lc() {
  for (ll n = 1; n <= 80; ++n) for (ll a = -2*n; a <= 2*n; ++a)
    for (ll b : {LLONG_MIN, -n, -1LL, 0LL, 1LL, n, LLONG_MAX}) {
      vector<ll> want;
      for (ll x = 0; x < n; ++x)
        if (((__int128)a*x-b)%n == 0) want.pb(x);
      require(lc::mod_leq(a, b, n) == want,
              "Linear congruence exhaustive signed oracle");
    }
  for (ll n : {7540113804746346429LL, LLONG_MAX})
    for (ll a : {LLONG_MIN, -1LL, 1LL, n-1})
      for (ll b : {LLONG_MIN, -1LL, 0LL, LLONG_MAX}) {
        auto x = lc::mod_leq(a, b, n);
        ll normalized = a%n; if (normalized < 0) normalized += n;
        ll g = gcd(normalized, n);
        require(SZ(x) == g, "Linear congruence wide count");
        for (ll v : x) require(v >= 0 && v < n &&
          ((__int128)a*v-b)%n == 0, "Linear congruence wide residue");
      }
  cout << "PASS linear congruence signed exhaustive/wide moduli\n";
}
void audit_cdq() {
  vector<vector<int>> cube;
  for (int i = 0; i < 16; ++i) {
    vector<int> p(4);
    for (int d = 0; d < 4; ++d) p[d] = i>>d&1 ? INT_MAX : INT_MIN;
    cube.pb(p);
  }
  for (int mask = 0; mask < (1<<16); ++mask) {
    vector<vector<int>> a; vector<int> ids;
    for (int i = 0; i < 16; ++i) if (mask>>i&1)
      for (int r = 0; r <= i%3; ++r) a.pb(cube[i]), ids.pb(i);
    vector<ll> want(a.size(), -1);
    for (int i = 0; i < SZ(a); ++i) for (int j : ids)
      want[i] += (j&ids[i]) == j;
    require(cdq::dominance(a) == want, "CDQ duplicate Boolean lattice");
  }
  for (int n : {0,1,2,100})
    require(cdq::dominance(vector<vector<int>>(n)) == vector<ll>(n,n-1),
            "CDQ zero dimensions");
  cout << "PASS CDQ all 65536 duplicate 4D Boolean sublattices\n";
}
void free_seg(hld::Seg::Node *p) {
  if (p) free_seg(p->l), free_seg(p->r), delete p;
}
void audit_hld() {
  hld::Heavy_light_Decomposition h;
  for (int t = 0; t < 100; ++t) {
    int n = 1+t%127; h.init(n);
    for (int i = 1; i <= n; ++i) h.val[i] = i%2 ? 7 : -7;
    for (int i = 2; i <= n; ++i) h.add_edge(i, 1+rng()%(i-1));
    h.build(); vector<int> live(n+1);
    for (int i = 1; i <= n; ++i) live[i] = h.val[i];
    for (int step = 0; step < 12; ++step) {
      int l = rng()%(n+1), r = rng()%(n+1);
      if (l > r) swap(l, r);
      int delta = int(rng()%15)-7; h.seg.update(l, r, delta);
      for (int i = 1; i <= n; ++i)
        if (l <= h.pl[i] && h.pl[i] < r) live[i] += delta;
      for (int s = 1; s <= n; ++s) {
        auto dfs = [&](auto &&self, int u, int p, int sum) -> void {
          sum += live[u];
          require(h.query(s, u) == sum, "HLD independent tree paths");
          for (int v : h.G[u]) if (v != p) self(self, v, u, sum);
        };
        dfs(dfs, s, 0, 0);
      }
    }
    free_seg(h.seg.root); h.seg.root = nullptr;
  }
  cout << "PASS HLD mutable paths, zero ranges and reused trees\n";
}
struct Arc { int u, v, cap, cost; };
void audit_cir() {
  auto &m = cir::mcmf;
  for (int t = 0; t < 2500; ++t) {
    int n = 1+rng()%4; vector<Arc> a(rng()%8);
    for (auto &e : a) e = {int(rng()%n), int(rng()%n),
                           int(rng()%3), int(rng()%21)-10};
    ll best = LLONG_MAX; vector<int> bal(n);
    auto brute = [&](auto &&self, int i, ll cost) -> void {
      if (i == SZ(a)) {
        if (all_of(ALL(bal), [](int x) { return !x; }))
          best = min(best, cost);
        return;
      }
      auto e = a[i];
      for (int f = 0; f <= e.cap; ++f) {
        bal[e.u] += f, bal[e.v] -= f;
        self(self, i+1, cost+f*e.cost);
        bal[e.u] -= f, bal[e.v] += f;
      }
    };
    brute(brute, 0, 0);
    for (int rep = 0; rep < 3; ++rep) {
      shuffle(ALL(a), rng); m.init(n); vector<pair<int,int>> refs;
      for (auto e : a) {
        refs.pb({e.u, SZ(m.G[e.u])});
        m.add_edge(e.u, e.v, e.cap, e.cost);
      }
      int mx = 0; for (auto e : a) mx = max(mx, e.cap);
      m.solve(mx == 2 ? 1 : 0); ll cost = 0; fill(ALL(bal), 0);
      for (int i = 0; i < SZ(a); ++i) {
        auto &e = m.G[refs[i].X][refs[i].Y];
        auto &r = m.G[e.to][e.rev];
        require(e.cap == a[i].cap && e.flow >= 0 && e.flow <= e.cap,
                "Circulation capacity bounds");
        require(r.to == e.from && r.flow == -e.flow,
                "Circulation reverse edge");
        cost += e.flow*e.cost;
        bal[e.from] += e.flow, bal[e.to] -= e.flow;
      }
      require(cost == best && all_of(ALL(bal), [](int x){return !x;}),
              "Circulation exact flow-enumeration oracle");
    }
  }
  cout << "PASS circulation 7500 signed multigraph insertion orders\n";
}
void audit_db() {
  for (int c = 1; c <= 5; ++c) for (int n = 0; n <= 6; ++n) {
    int total = 1; for (int i = 0; i < n; ++i) total *= c;
    for (int k : {1, max(1,total/2), max(1,total-1), total}) {
      int len = n+k-1; vector<int> out(len+2, -77);
      fill_n(db::dbs.buf, n+1, INT_MAX);
      db::dbs.solve(c, n, k, out.data()+1);
      require(out.front() == -77 && out.back() == -77, "DB guards");
      for (int i = 1; i <= len; ++i)
        require(out[i] >= 0 && out[i] < c, "DB alphabet");
      set<vector<int>> windows;
      for (int i = 1; i <= k; ++i)
        windows.insert(vector<int>(out.begin()+i, out.begin()+i+n));
      require(SZ(windows) == k, "DB distinct windows");
    }
  }
  db::dbs.solve(1, 0, 1, nullptr);
  cout << "PASS DB partial/full necklace boundaries and dirty scratch\n";
}
int main() {
  audit_circle(); audit_li(); audit_km(); audit_qr(); audit_lc();
  audit_cdq(); audit_hld(); audit_cir(); audit_db();
}
