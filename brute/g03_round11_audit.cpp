#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define SZ(a) ((int)(a).size())
#define ALL(a) (a).begin(), (a).end()
#define pb push_back

namespace modular {
#include "../codebook/6_Math/ModMin.cpp"
void audit() {
  // Full periods remain enumerable even with a 10^14-scale modulus.
  for (ll scale : {1LL, 1000000000039LL})
    for (ll p : {1, 2, 3, 17, 127, 251}) for (ll x = 0; x < p; ++x) {
      ll m = p * scale, a = x * scale;
      for (ll k = 0; k < p; ++k) for (int type = 0; type < 4; ++type) {
        ll l = k * scale, r = l;
        if (type == 1) r = min(m - 1, l + scale - 1);
        if (type == 2) l = min(m - 1, l + 1), r = l;
        if (type == 3) r = m - 1;
        ll want = -1;
        for (ll i = 0; i < p; ++i) {
          ll z = (__int128)a * i % m;
          if (l <= z && z <= r) { want = i; break; }
        }
        assert(mod_min(a, m, l, r) == want);
      }
    }
}
}
namespace rotation {
#include "../codebook/5_String/Smallest_Rotation.cpp"
bool less_bytes(const string &a, const string &b) {
  return lexicographical_compare(ALL(a), ALL(b),
    [](unsigned char x, unsigned char y) { return x < y; });
}
void audit() {
  assert(mcp("").empty());
  for (int a = 0; a < 256; ++a) for (int b = 0; b < 256; ++b) {
    // Former signed-byte bug, its reverse, and tied-prefix variants.
    for (string s : {string{char(a), char(b), char(a)},
                    string{char(b), char(a), char(b), char(a)}}) {
      string want = s;
      for (int k = 1; k < SZ(s); ++k) {
        string t = s.substr(k) + s.substr(0, k);
        if (less_bytes(t, want)) want = t;
      }
      assert(mcp(s) == want);
    }
  }
}
}
namespace equations {
const int MAXN = 8;
#include "../codebook/6_Math/Fraction.cpp"
#include "../codebook/6_Math/Simultaneous_Equations.cpp"
// Small minors are <1000003, so rank over this prime equals rational rank.
int rank_mod(vector<vector<ll>> a, int cols) {
  const ll p = 1000003; int rank = 0;
  auto power = [&](ll x) {
    ll r = 1;
    for (int e = p - 2; e; e >>= 1, x = x * x % p)
      if (e & 1) r = r * x % p;
    return r;
  };
  for (auto &row : a) for (ll &x : row) x = (x % p + p) % p;
  for (int c = 0; c < cols && rank < SZ(a); ++c) {
    int r = rank; while (r < SZ(a) && !a[r][c]) ++r;
    if (r == SZ(a)) continue;
    swap(a[r], a[rank]); ll inv = power(a[rank][c]);
    for (int j = c; j < cols; ++j) a[rank][j] = a[rank][j] * inv % p;
    for (int i = rank + 1; i < SZ(a); ++i) {
      ll t = a[i][c];
      for (int j = c; j < cols; ++j)
        a[i][j] = (a[i][j] - t * a[rank][j] % p + p) % p;
    }
    ++rank;
  }
  return rank;
}
void audit() {
  mt19937 rng(1103);
  for (int tc = 0; tc < 5000; ++tc) {
    matrix s; s.n = rng() % 6, s.m = rng() % 6;
    vector<vector<ll>> a(s.n, vector<ll>(s.m + 1));
    for (auto &row : a) for (ll &x : row) x = int(rng() % 5) - 2;
    for (int i = 0; i < s.n; ++i) for (int j = 0; j <= s.m; ++j)
      s.M[i][j] = fraction(a[i][j], i % 3 + 1);
    fill_n(s.sol, MAXN, fraction(42, 5));
    int rank = rank_mod(a, s.m);
    int want = rank_mod(a, s.m + 1) == rank ? rank : -1;
    assert(s.solve() == want);
    if (want >= 0) for (int i = 0; i < s.n; ++i) {
      fraction z;
      for (int j = 0; j < s.m; ++j)
        z = z + fraction(a[i][j]) * s.sol[j];
      assert((z - fraction(a[i][s.m])).n == 0);
    }
    for (int j = want < 0 ? 0 : s.m; j < MAXN; ++j)
      assert(s.sol[j].n == 42 && s.sol[j].d == 5);
    assert(s.solve() == want);
  }
}
}
namespace inverse {
void audit() {
  for (int mod : {2, 3, 97, 65537, INT_MAX}) {
    int N = min(mod - 1, 10000);
    vector<int> inv(N + 1, -37);
#include "../codebook/6_Math/Mod_Inverse.cpp"
    assert(inv[0] == -37);
    for (int i = 1; i <= N; ++i) {
      assert(0 < inv[i] && inv[i] < mod);
      assert(1LL * i * inv[i] % mod == 1);
    }
  }
}
}
namespace geometry {
const double eps = 1e-9, pi = acos(-1.);
double sq(double x) { return x * x; }
int sign(double x) { return abs(x) < eps ? 0 : x > 0 ? 1 : -1; }
#include "../codebook/8_Geometry/_basic.cpp"
struct C { P c; double r; };
#include "../codebook/8_Geometry/circleOrArea.cpp"
#include "../codebook/8_Geometry/rotatingSweepLine.cpp"
using ld = long double;
ld lens(C a, C b) {
  ld r = a.r, s = b.r;
  ld d = hypotl((ld)a.c.x - b.c.x, (ld)a.c.y - b.c.y);
  if (d >= r + s) return 0;
  if (d <= abs(r - s)) return acosl(-1.L) * min(r*r, s*s);
  ld x = (d*d + r*r - s*s) / (2*d*r);
  ld y = (d*d + s*s - r*r) / (2*d*s);
  ld z = (-d+r+s)*(d+r-s)*(d-r+s)*(d+r+s);
  // Rounded exact tangencies can put the analytic oracle outside [-1,1].
  return r*r*acosl(clamp(x,-1.L,1.L))
       + s*s*acosl(clamp(y,-1.L,1.L)) - sqrtl(max(0.L,z))/2;
}
void area_audit() {
  assert(CircleUnionArea({}) == 0);
  for (double scale : {.01, 1., 10000.}) {
    for (int r = 0; r <= 4; ++r) for (int s = 0; s <= 4; ++s)
      for (int x = -8; x <= 8; ++x) {
        if (!x && r == s) continue; // Excluded coincident circles.
        vector<C> c{{{0, 0}, r*scale}, {{x*scale, 0}, s*scale}};
        ld want = acosl(-1.L)*(sq(c[0].r)+sq(c[1].r))
                  - lens(c[0], c[1]);
        for (int k = 0; k < 4; ++k) {
          auto got = CircleUnionArea(c);
          if (!(abs(got - want) <= 2e-10L * max(1.L, want))) {
            cerr << setprecision(18) << "circle " << scale << ' '
                 << r << ' ' << s << ' ' << x << ' ' << k
                 << " got=" << got << " want=" << want << '\n';
            abort();
          }
          for (auto &a : c) a.c = a.c.rot(pi/2);
          reverse(ALL(c));
        }
      }
    // Multiple covering arcs, zero disks, internal/external tangency.
    vector<C> c{{{0,0},4*scale}, {{scale,0},3*scale},
      {{-scale,0},2*scale}, {{0,0},0}, {{10*scale,0},2*scale}};
    ld want = 20 * acosl(-1.L) * scale * scale;
    auto cmp = [](C a, C b) {
      return tie(a.c.x, a.c.y, a.r) < tie(b.c.x, b.c.y, b.r);
    };
    sort(ALL(c),cmp);
    do {
      assert(abs(CircleUnionArea(c) - want) <=
             2e-10L * max(1.L, want));
    } while (next_permutation(ALL(c),cmp));
  }
  C a{{0,0}, 2}, b{{1,0}, 2};
  vector<pair<double,double>> v{{-7,-6}};
  auto covered = CoverSegment(a,b); AppendCoverSegment(a,b,v);
  assert(v.front() == make_pair(-7.,-6.));
  assert(vector(v.begin()+1,v.end()) == covered);
}
void sweep_audit() {
  vector<pair<int,int>> grid;
  for (int x : {INT_MIN, -1, 0, 1, INT_MAX})
    for (int y : {INT_MIN, -1, 0, 1, INT_MAX}) grid.pb({x,y});
  mt19937 rng(1111); int cases = 0;
  for (int tc = 0; tc < 10000; ++tc) {
    shuffle(ALL(grid), rng);
    vector<pair<int,int>> p;
    int n = tc % 9;
    for (auto a : grid) {
      if (SZ(p) == n) break;
      bool valid = true;
      for (auto b : p) for (auto c : p) if (b != c)
        valid &= (__int128)(1LL*a.first-b.first)*(1LL*c.second-b.second)
               != (__int128)(1LL*a.second-b.second)*(1LL*c.first-b.first);
      if (valid) p.pb(a);
    }
    vector<int> want(SZ(p)); iota(ALL(want), 0);
    sort(ALL(want), [&](int a, int b) { return p[a] > p[b]; });
    assert(rotatingSweepLineOrder(p) == want);
    auto saved = p; rotatingSweepLine(p); assert(p == saved);
    ++cases;
  }
  cout << "extreme sweep cases=" << cases << '\n';
}
}
namespace dynamic {
#include "../codebook/3_Data_Structure/Dynamic_2D_Segment_Tree.cpp"
void free1(Seg::Node *p) {
  if (!p) return;
  free1(p->l), free1(p->r); delete p;
}
void free2(Seg2D::Node *p) {
  if (!p) return;
  free2(p->l), free2(p->r);
  free1(p->sum.root), free1(p->lazy.root); delete p;
}
void audit() {
  for (int n : {1, 2, 3, 1000000000, INT_MAX}) {
    Seg2D s(n,n); map<pair<int,int>,int> a;
    vector<int> at{0,n/2,n-1}; mt19937 rng(n);
    for (int tc = 0; tc < 400; ++tc) {
      int x = at[rng()%3], y = at[rng()%3], v = int(rng()%11)-5;
      s.update(x,x+1,y,y+1,v); a[{x,y}] += v;
      for (int k = 0; k < 30; ++k) {
        int l = at[rng()%3], r = at[rng()%3]+1;
        int b = at[rng()%3], t = at[rng()%3]+1;
        if (l > r) swap(l,r); if (b > t) swap(b,t);
        int want = 0;
        for (auto [p,z] : a)
          if (l <= p.first && p.first < r &&
              b <= p.second && p.second < t) want += z;
        assert(s.query(l,r,b,t) == want);
      }
    }
    free2(s.root);
  }
}
}
namespace trees {
const int N = 40;
int par[N], dep[N], dfn[N];
int LCA(int a, int b) {
  while (dep[a] > dep[b]) a = par[a];
  while (dep[b] > dep[a]) b = par[b];
  while (a != b) a = par[a], b = par[b];
  return a;
}
#include "../codebook/2_Graph/Virtual_Tree.cpp"
#include "../codebook/9_Else/tree_hash.cpp"
// A separate sorted bottom-up evaluation fixes exact hash semantics.
tree_hash_u64 hash_oracle(vector<vector<int>> g, int root,
                          tree_hash_u64 seed) {
  if (!seed) seed = tree_hash_default_seed;
  vector<int> order{root}, p(SZ(g),-1); p[root] = root;
  for (int i = 0; i < SZ(order); ++i) for (int v : g[order[i]])
    if (v != p[order[i]]) p[v] = order[i], order.pb(v);
  vector<tree_hash_u64> h(SZ(g),seed);
  reverse(ALL(order));
  for (int u : order) {
    vector<tree_hash_u64> children;
    for (int v : g[u]) if (p[v] == u && v != u) children.pb(h[v]);
    sort(ALL(children));
    for (auto x : children) {
      x ^= x << 13; x ^= x >> 7; x ^= x << 17; h[u] += x;
    }
  }
  return h[root];
}
void audit() {
  mt19937 rng(1133);
  for (int tc = 0; tc < 300; ++tc) {
    int n = 1 + rng()%N; vector<vector<int>> g(n);
    for (int i = 1; i < n; ++i) {
      int p = tc%3 == 0 ? 0 : tc%3 == 1 ? i-1 : rng()%i;
      g[i].pb(p), g[p].pb(i);
    }
    for (int root = 0; root < n; ++root) {
      int timer = 0;
      auto dfs = [&](auto &&self, int u, int p) -> void {
        par[u] = p; dfn[u] = timer++;
        for (int v : g[u]) if (v != p)
          dep[v] = dep[u]+1, self(self,v,u);
      };
      dep[root] = 0; dfs(dfs,root,root);
      vector<int> keys;
      for (int i = 0; i < n; ++i) if (rng()%2) keys.pb(i), keys.pb(i);
      shuffle(ALL(keys),rng);
      set<int> nodes(ALL(keys));
      for (int a : keys) for (int b : keys) nodes.insert(LCA(a,b));
      set<pair<int,int>> want, got;
      int expected = -1;
      for (int u : nodes) {
        int p = par[u];
        while (p != par[p] && !nodes.count(p)) p = par[p];
        if (u != p && nodes.count(p)) want.insert({p,u});
        else expected = u;
      }
      int r = build(keys); assert(r == expected);
      for (int u = 0; u < n; ++u) for (int v : vG[u])
        assert(got.insert({u,v}).second);
      assert(got == want);
      if (r != -1) reset(r);
      for (int u = 0; u < n; ++u) assert(vG[u].empty());
      for (tree_hash_u64 seed : {0ULL, 1ULL, ULLONG_MAX,
                                 tree_hash_default_seed}) {
        auto h = rooted_tree_hash(g,root,seed);
        assert(h == hash_oracle(g,root,seed));
        vector<int> id(n); iota(ALL(id),0); shuffle(ALL(id),rng);
        vector<vector<int>> q(n);
        for (int u = 0; u < n; ++u) for (int v : g[u])
          q[id[u]].pb(id[v]);
        for (auto &row : q) shuffle(ALL(row),rng);
        assert(rooted_tree_hash(q,id[root],seed) == h);
      }
    }
  }
}
}
namespace cyclic {
#include "../codebook/9_Else/cyc_tsearch.cpp"
void audit() {
  for (int n : {1,2,3,4,17,1024,1000000000,INT_MAX})
    for (ll valley : {0LL, (ll)n/2, (ll)n-1})
      for (ll shift : {0LL, 1LL%n, (ll)n/2, (ll)n-1}) {
        int calls = 0;
        auto value = [&](int i) {
          assert(0 <= i && i < n);
          ll p = (i + shift)%n;
          return p <= valley ? 2*(valley-p) : 2*(p-valley)-1;
        };
        int got = cyc_tsearch(n,[&](int a,int b) {
          ++calls; return value(a) < value(b);
        });
        assert(value(got) == 0);
        assert(calls <= 2 + 2*(int)bit_width((unsigned)n));
      }
}
}
int main() {
  modular::audit(); rotation::audit(); equations::audit();
  inverse::audit(); geometry::area_audit(); geometry::sweep_audit();
  dynamic::audit(); trees::audit(); cyclic::audit();
  cout << "g03 round11 adversarial audit PASS\n";
}
