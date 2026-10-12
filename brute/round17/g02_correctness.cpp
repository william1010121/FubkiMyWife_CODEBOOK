#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define SZ(a) int((a).size())
#define ALL(a) (a).begin(), (a).end()
#define pb push_back
namespace transform {
#include "../../codebook/7_Polynomial/NTT_chrislaiisme.cpp"
}
#undef int
namespace rotation {
#include "../../codebook/5_String/Smallest_Rotation.cpp"
}
namespace fingerprint {
#include "../../codebook/9_Else/tree_hash.cpp"
}
namespace binomial {
#include "../../codebook/6_Math/Mod_Arithmetic.cpp"
#include "../../codebook/6_Math/Mod_Binomial.cpp"
}
#include "../../codebook/8_Geometry/Triangulation.cpp"
namespace leftist {
#include "../../codebook/3_Data_Structure/Leftist_Tree.cpp"
}
namespace dominator {
constexpr int N = 34;
#include "../../codebook/2_Graph/Dominator_Tree.cpp"
}
namespace treap {
#include "../../codebook/3_Data_Structure/Treap.cpp"
}
namespace rational {
#include "../../codebook/6_Math/Fraction.cpp"
}
namespace debruijn {
#include "../../codebook/5_String/De_Bruijn_sequence.cpp"
}
void check(bool ok, const char *why) {
  if (!ok) { cerr << "FAIL round17 " << why << '\n'; exit(1); }
}
mt19937_64 rng(1702);
void transform_audit() {
  using namespace transform;
  for (int n = 1; n <= 64; n *= 2) for (int rep = 0; rep < 20; ++rep) {
    vector<ll> a(n), expected(n);
    for (auto &x : a) x = rep%2 ? rng()%mod : mod-1;
    for (bool inverse : {false, true}) {
      ll w = pwr(g, (mod-1)/n);
      if (inverse) w = pwr(w, mod-2);
      for (int k = 0; k < n; ++k) {
        ll step = pwr(w,k), power = 1; expected[k] = 0;
        for (ll x : a) {
          expected[k] = (expected[k] + x*power)%mod;
          power = power*step%mod;
        }
        if (inverse) expected[k] = expected[k]*pwr(n,mod-2)%mod;
      }
      auto got = a; ntt(got,inverse);
      check(got == expected, "NTT independent DFT both directions");
    }
  }
  check(mult({}, {mod-1}).empty(), "NTT empty convolution");
  // Pascal powers cross convolution padding boundaries independently.
  vector<ll> a{1};
  for (int n = 0; n < 260; ++n) {
    vector<ll> want(n+2,1);
    for (int k = 1; k <= n; ++k) want[k] = (a[k-1]+a[k])%mod;
    a = mult(a,{1,1});
    check(a == want, "NTT binomial convolution padding boundaries");
  }
}
void rotation_audit() {
  using rotation::mcp;
  for (int a = 0; a < 256; ++a) for (int b = 0; b < 256; ++b) {
    string s{char(a),char(b)};
    string want{char(min(a,b)),char(max(a,b))};
    check(mcp(s) == want, "rotation every byte pair");
  }
  auto less_bytes = [](string a,string b) {
    return lexicographical_compare(ALL(a),ALL(b),[](auto x,auto y) {
      return (unsigned char)x < (unsigned char)y;
    });
  };
  for (int n = 0; n <= 150; ++n) {
    string s(n,char(255));
    for (int rep = 0; rep < 3; ++rep) {
      if (n) s[rng()%n] = char(rep);
      string want = s, shift = s;
      for (int i = 0; i < n; ++i) {
        if (less_bytes(shift,want)) want = shift;
        rotate(shift.begin(),shift.begin()+1,shift.end());
      }
      check(mcp(s) == want, "rotation long equality runs and NUL");
    }
  }
}
void hash_audit() {
  using namespace fingerprint;
  for (int n : {1,2,3,31,512}) {
    vector<vector<int>> g(n);
    for (int i = 1; i < n; ++i) {
      int p = rng()%i; g[p].pb(i); g[i].pb(p);
    }
    for (int root : {0,n-1}) for (auto seed : {0ULL,1ULL,ULLONG_MAX}) {
      vector<int> parent(n,-1), order{root};
      for (int i = 0; i < n; ++i) for (int v : g[order[i]])
        if (v != parent[order[i]]) parent[v] = order[i], order.pb(v);
      vector<tree_hash_u64> want(n,seed ? seed : tree_hash_default_seed);
      for (int i = n-1; i > 0; --i) {
        auto x = want[order[i]];
        x ^= x<<13; x ^= x>>7; x ^= x<<17;
        want[parent[order[i]]] += x;
      }
      for (int u = 0; u < n; ++u)
        check(tree_hash_dfs(g,u,parent[u],seed) == want[u],
              "hash public parent-subtree helper");
      for (auto &adj : g) shuffle(ALL(adj),rng);
      check(rooted_tree_hash(g,root,seed) == want[root],
            "hash unsigned wrapping and adjacency order");
    }
  }
}
void binomial_audit() {
  using namespace binomial;
  // Straddle the ll / __int128 query branch with known prime moduli.
  for (ll p : {2LL,3LL,97LL,2147483647LL,2147483659LL,
               9223372036854775783LL}) {
    int N = min(p-1,64LL); ModBinomial b(N,p), zero(0,p);
    check(zero.C(0,0) == 1, "binomial N=0");
    vector<ll> row{1};
    for (int n = 0; n <= N; ++n) {
      for (int k = 0; k <= n; ++k)
        check(b.C(n,k) == row[k], "binomial branch-boundary Pascal");
      vector<ll> next(n+2,1);
      for (int k = 1; k <= n; ++k)
        next[k] = (__int128(row[k-1])+row[k])%p;
      row = next;
    }
    check(b.C(INT_MIN,INT_MAX) == 0 && b.C(INT_MAX,-1) == 0,
          "binomial invalid guards precede table access");
  }
  ModBinomial binary(1,2);
  for (int i = 0; i < 10000; ++i) {
    ll n = rng()&LLONG_MAX, k = rng()&LLONG_MAX;
    check(binary.lucas(n,k) == ((k&n) == k),
          "Lucas full-width binary subset criterion");
  }
}
// Independent lifted determinant, using all 24 permutations.
int lifted_sign(array<delaunay::Point,4> p) {
  array<array<__int128,4>,4> a;
  for (int i = 0; i < 4; ++i) {
    __int128 x = p[i].x, y = p[i].y;
    a[i] = {x,y,x*x+y*y,1};
  }
  array<int,4> perm{0,1,2,3}; __int128 det = 0;
  do {
    int parity = 0; __int128 term = 1;
    for (int i = 0; i < 4; ++i) {
      term *= a[i][perm[i]];
      for (int j = 0; j < i; ++j) parity ^= perm[j] > perm[i];
    }
    det += parity ? -term : term;
  } while (next_permutation(ALL(perm)));
  return (det > 0)-(det < 0);
}
void geometry_case(vector<delaunay::Point> p) {
  using delaunay::Point;
  auto triangles = delaunay::Triangulate(p);
  check(delaunay::edges.empty(), "Delaunay arena reuse");
  if (p.size() < 3) {
    check(triangles.empty(), "Delaunay small input"); return;
  }
  auto cross = [](Point a,Point b,Point c) { return (b-a)^(c-a); };
  bool collinear = true;
  for (auto q : p) collinear &= cross(p[0],p[1],q) == 0;
  if (collinear) {
    check(triangles.empty(), "Delaunay collinear input"); return;
  }
  sort(ALL(p)); vector<Point> hull;
  for (int pass = 0; pass < 2; ++pass) {
    int start = SZ(hull);
    for (auto q : p) {
      while (SZ(hull) >= start+2 &&
        cross(hull[SZ(hull)-2],hull.back(),q) <= 0) hull.pop_back();
      hull.pb(q);
    }
    hull.pop_back(); reverse(ALL(p));
  }
  __int128 area = 0, total = 0; int boundary = 0;
  for (int i = 1; i+1 < SZ(hull); ++i)
    area += cross(hull[0],hull[i],hull[i+1]);
  for (auto q : p) for (int i = 0; i < SZ(hull); ++i) {
    auto a = hull[i], b = hull[(i+1)%SZ(hull)];
    if (!cross(a,b,q) && min(a.x,b.x) <= q.x && q.x <= max(a.x,b.x)
        && min(a.y,b.y) <= q.y && q.y <= max(a.y,b.y)) {
      ++boundary; break;
    }
  }
  check(SZ(triangles) == 2*SZ(p)-2-boundary, "Delaunay Euler count");
  set<Point> used; map<pair<Point,Point>,int> incidence;
  for (auto t : triangles) {
    ll a = cross(t[0],t[1],t[2]);
    check(a > 0, "Delaunay CCW"); total += a; used.insert(ALL(t));
    for (auto q : p)
      check(lifted_sign({t[0],t[1],t[2],q}) <= 0,
            "Delaunay independent empty circumcircle");
    for (int i = 0; i < 3; ++i) {
      auto a = t[i], b = t[(i+1)%3]; if (b < a) swap(a,b);
      check(++incidence[{a,b}] <= 2, "Delaunay edge incidence");
    }
  }
  check(area == total && used == set<Point>(ALL(p)),
        "Delaunay area and all input vertices");
}
void geometry_audit() {
  using delaunay::Point;
  geometry_case({}); geometry_case({{0,0}});
  vector<Point> ring;
  for (int x = -65; x <= 65; ++x) for (int y = -65; y <= 65; ++y)
    if (x*x+y*y == 65*65) ring.pb({x*7692307LL,y*7692307LL});
  for (int rep = 0; rep < 30; ++rep) {
    shuffle(ALL(ring),rng); geometry_case(ring);
    auto p = ring;
    for (ll x : {-1LL,0LL,1LL}) p.pb({x,0});
    geometry_case(p);
  }
  for (int rep = 0; rep < 300; ++rep) {
    vector<Point> p;
    for (int x = -2; x <= 2; ++x) for (int y = -2; y <= 2; ++y)
      if (rng()%3) p.pb({x*250000000LL,y*250000000LL});
    shuffle(ALL(p),rng); geometry_case(p);
  }
}
void leftist_audit() {
  using namespace leftist;
  auto audit = [&](auto &&self,node *p) -> array<ll,3> {
    if (!p) return {-1,0,0};
    auto a = self(self,p->l), b = self(self,p->r);
    check((!p->l || p->data >= p->l->data) &&
          (!p->r || p->data >= p->r->data), "leftist max order");
    check(a[0] >= b[0] && p->v == b[0]+1, "leftist null ranks");
    check(p->sz == a[1]+b[1]+1 && p->sum == a[2]+b[2]+p->data,
          "leftist aggregates");
    return {p->v,p->sz,p->sum};
  };
  for (int rep = 0; rep < 100; ++rep) {
    vector<node*> heaps; multiset<ll> want;
    for (int i = 0; i < 257; ++i) {
      ll key = rep%2 ? ll(rng()%2000000000001ULL)-1000000000000LL : 0;
      heaps.pb(new node(key)); want.insert(key);
    }
    while (heaps.size() > 1) {
      shuffle(ALL(heaps),rng); node *a = heaps.back(); heaps.pop_back();
      heaps.back() = merge(a,heaps.back()); audit(audit,heaps.back());
    }
    node *root = heaps[0];
    check(merge(root,nullptr) == root, "leftist empty merge");
    while (!want.empty()) {
      check(root->data == *want.rbegin(), "leftist descending pop");
      want.erase(prev(want.end())); pop(root); audit(audit,root);
    }
    check(!root, "leftist drained heap");
  }
  for (ll key : {LLONG_MIN,LLONG_MAX}) {
    node *p = new node(key); audit(audit,p); pop(p);
    check(!p, "leftist extreme singleton");
  }
}
void dominator_audit() {
  using namespace dominator;
  static dominator_tree d;
  // Reinitialize through shrinking and growing n, then change roots.
  for (int n : {33,1,17,2,33,3}) for (int rep = 0; rep < 10; ++rep) {
    d.init(n); vector<vector<int>> g(n+1);
    for (int i = 0; i < 3*n; ++i) {
      int u = 1+rng()%n, v = 1+rng()%n;
      d.add_edge(u,v); g[u].pb(v);
      if (i%3 == 0) d.add_edge(u,v), g[u].pb(v);
    }
    for (int root = 1; root <= n; ++root) {
      d.tarjan(root);
      auto reach = [&](int banned) {
        vector<bool> seen(n+1); vector<int> q;
        if (root != banned) seen[root] = true, q.pb(root);
        for (int i = 0; i < SZ(q); ++i) for (int v : g[q[i]])
          if (v != banned && !seen[v]) seen[v] = true, q.pb(v);
        return seen;
      };
      auto live = reach(0); vector<int> parent(n+1);
      for (int u = 1; u <= n; ++u) for (int v : d.tree[u]) {
        check(!parent[v] && live[u] && live[v], "dominator tree edges");
        parent[v] = u;
      }
      for (int banned = 1; banned <= n; ++banned) {
        auto seen = reach(banned);
        for (int v = 1; v <= n; ++v) if (live[v]) {
          bool ancestor = false;
          for (int u = v, count = 0; u; u = parent[u]) {
            check(++count <= n, "dominator acyclic parent tree");
            ancestor |= u == banned;
          }
          check(ancestor == !seen[v], "dominator deletion oracle");
          if (v != root)
            check(d.id[d.idom[d.dfn[v]]] == parent[v],
                  "dominator DFS-indexed idom");
        }
      }
    }
  }
}
void treap_audit() {
  using namespace treap;
  // Priority ties and both root aliases, checked by all legal size splits.
  for (int rep = 0; rep < 100; ++rep) {
    node *root = nullptr; vector<int> want;
    for (int i = 0; i < 40; ++i) {
      int key = array{INT_MIN,-1,0,INT_MAX}[rng()%4];
      node *p = new node(key); p->priority = rep%3 ? i%3 : 0;
      node *a, *b; split(root,a,b,key); root = merge(a,merge(p,b));
      want.pb(key);
    }
    sort(ALL(want));
    auto audit = [&](auto &&self,node *p) -> int {
      if (!p) return 0;
      check((!p->l || p->priority <= p->l->priority) &&
            (!p->r || p->priority <= p->r->priority), "treap heap");
      int size = 1+self(self,p->l)+self(self,p->r);
      check(p->sz == size, "treap sizes"); return size;
    };
    for (int k = 0; k <= 40; ++k) for (int side = 0; side < 2; ++side) {
      node *other;
      if (side) {
        split2(root,other,root,k);
        check(sz(other) == k && sz(root) == 40-k, "treap right alias");
        root = merge(other,root);
      } else {
        split2(root,root,other,k);
        check(sz(root) == k && sz(other) == 40-k, "treap left alias");
        root = merge(root,other);
      }
      interval(root,k+1,40-k); audit(audit,root);
      for (int i = 0; i < 40; ++i)
        check(kth(root,i+1)->data == want[i],
              "treap split restoration");
    }
    while (root) {
      int key = root->data;
      auto it = lower_bound(ALL(want),key);
      check(Rank(root,key) == it-want.begin(), "treap strict rank");
      want.erase(it); check(erase(root,key), "treap one-copy erase");
      check(audit(audit,root) == SZ(want), "treap after erase");
    }
    node *other; split(root,root,other,INT_MIN);
    split2(root,other,root,0); interval(root,1,0);
    check(!root && !other && !erase(root,0), "treap empty operations");
  }
}
void fraction_audit() {
  using rational::fraction;
  using wide = __int128;
  auto legal = [](wide x) {
    return -wide(LLONG_MAX) <= x && x <= LLONG_MAX;
  };
  auto normalize = [](wide n,wide d) {
    wide a = n < 0 ? -n : n, b = d < 0 ? -d : d;
    while (b) { wide r = a%b; a = b; b = r; }
    n /= a; d /= a; if (d < 0) n = -n, d = -d;
    return pair<wide,wide>{n,d};
  };
  vector<ll> values{0,1,-1,3037000499LL,-3037000499LL,
                   LLONG_MAX/2,-LLONG_MAX/2,LLONG_MAX,-LLONG_MAX};
  vector<fraction> fractions;
  for (ll n : values) for (ll d : values) if (d) {
    fraction f(n,d); auto want = normalize(n,d);
    check(f.n == want.first && f.d == want.second,
          "fraction full-width constructor signs and gcd");
    fractions.pb(f);
  }
  for (auto a : fractions) for (auto b : fractions)
    for (int op = 0; op < 4; ++op) {
      wide x = wide(a.n)*b.d, y = wide(b.n)*a.d;
      wide nn = op == 0 ? x+y : op == 1 ? x-y :
                op == 2 ? wide(a.n)*b.n : x;
      wide dd = op == 3 ? wide(a.d)*b.n : wide(a.d)*b.d;
      if (!dd || !legal(nn) || !legal(dd) ||
          (op < 2 && (!legal(x) || !legal(y)))) continue;
      auto want = normalize(nn,dd);
      fraction got = op == 0 ? a+b : op == 1 ? a-b : op == 2 ? a*b : a/b;
      check(got.n == want.first && got.d == want.second,
            "fraction legal raw products, sum and reduction");
    }
}
void debruijn_audit() {
  using namespace debruijn;
  // Every truncation length, reusing poisoned scratch between orders.
  for (int c = 1; c <= 4; ++c) for (int n = 0; n <= 5; ++n) {
    int total = 1; for (int i = 0; i < n; ++i) total *= c;
    for (int k = 1; k <= total; ++k) {
      int len = n+k-1; vector<int> out(len+2,-77);
      fill_n(dbs.buf,n+1,INT_MAX);
      dbs.solve(c,n,k,out.data()+1);
      check(out.front() == -77 && out.back() == -77, "DB out guards");
      set<vector<int>> windows;
      for (int i = 1; i <= len; ++i)
        check(0 <= out[i] && out[i] < c, "DB alphabet");
      for (int i = 1; i <= k; ++i)
        windows.insert(vector<int>(out.begin()+i,out.begin()+i+n));
      check(SZ(windows) == k, "DB every truncation and dirty scratch");
    }
  }
  dbs.solve(INT_MAX,0,1,nullptr);
  vector<int> out(100004,-77);
  dbs.solve(INT_MAX,100000,3,out.data()+1);
  check(out.front() == -77 && out.back() == -77, "DB large-order guards");
  set<vector<int>> windows;
  for (int i = 1; i <= 3; ++i)
    windows.insert(vector<int>(out.begin()+i,out.begin()+i+100000));
  check(windows.size() == 3,
        "DB large order, huge alphabet, short output");
}
int main() {
  transform_audit(); rotation_audit(); hash_audit(); binomial_audit();
  geometry_audit(); leftist_audit(); dominator_audit(); treap_audit();
  fraction_audit(); debruijn_audit();
  cout << "PASS round17 g02: ten adversarial correctness audits\n";
}
