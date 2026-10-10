#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/9_Else/HilbertCurve.cpp"
#include "../../codebook/9_Else/cyc_tsearch.cpp"
#include "../../codebook/9_Else/tree_hash.cpp"
#include "../../codebook/2_Graph/BCC_Vertex.cpp"
#include "../../codebook/2_Graph/Bridge.cpp"
namespace factorial {
constexpr int MAXP = 1000001;
#include "../../codebook/6_Math/fac_no_p.cpp"
}
namespace palindrome {
constexpr int MAXN = 4097;
#include "../../codebook/5_String/Manacher.cpp"
}
struct Point {
  double x, y;
  Point operator-(Point b) const { return {x-b.x, y-b.y}; }
};
double Cross(Point a, Point b) { return a.x*b.y-a.y*b.x; }
#include "../../codebook/8_Geometry/maxTriangleOfConvex.cpp"
#include "../../codebook/8_Geometry/Triangulation.cpp"

void require(bool ok, const char *label) {
  if (!ok) { cerr << "FAIL " << label << '\n'; exit(1); }
}
mt19937_64 rng(140006);

// Independent inverse Hilbert construction, using nonnegative coordinates.
pii inverse_hilbert(int n, ll d) {
  int x = 0, y = 0;
  for (int s = 1; s < n; s *= 2, d /= 4) {
    int rx = (d/2)&1, ry = (d^rx)&1;
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
    set<ll> indices{0,total-1};
    // Exercise carries at every base-four digit in the index.
    for (ll block = 1; block < total; block *= 4)
      for (int t = 1; t <= 4; ++t)
        for (int delta : {-1,0,1}) {
          ll d = t*block+delta;
          if (0 <= d && d < total) indices.insert(d);
        }
    for (ll d : indices) {
      auto [x,y] = inverse_hilbert(n,d);
      require(hilbert(n,x,y) == d, "Hilbert bit-carry round trip");
      if (d+1 < total) {
        auto [a,b] = inverse_hilbert(n,d+1);
        require(abs(x-a)+abs(y-b) == 1, "Hilbert bit-carry adjacency");
      }
    }
  }
}
void cyclic_audit() {
  // Every cyclic valley/peak arrangement of ranks up to 12, not just
  // equal-slope U functions. Split all intermediate ranks between arms.
  for (int n = 2; n <= 12; ++n)
    for (int mask = 0; mask < (1<<(n-2)); ++mask) {
      vector<ll> a{LLONG_MIN};
      for (int i = 1; i < n-1; ++i)
        if (mask>>(i-1)&1) a.pb(i);
      a.pb(LLONG_MAX);
      for (int i = n-2; i >= 1; --i)
        if (!(mask>>(i-1)&1)) a.pb(i);
      for (int shift = 0; shift < n; ++shift) {
        int calls = 0;
        int got = cyc_tsearch(n,[&](int u,int v) {
          require(0 <= u && u < n && 0 <= v && v < n,
                  "cyclic comparator indices");
          ++calls; return a[u] < a[v];
        });
        require(a[got] == LLONG_MIN, "all cyclic bitonic rank splits");
        require(calls <= 2+2*int(bit_width(unsigned(n))),
                "cyclic logarithmic comparisons");
        rotate(a.begin(),a.begin()+1,a.end());
      }
    }
  require(cyc_tsearch(1,[](int,int) { abort(); return false; }) == 0,
          "cyclic singleton needs no comparison");
}
ll mod_power(ll x, ll e, ll m) {
  ll r = 1;
  for (; e; e /= 2, x = (__int128)x*x%m)
    if (e&1) r = (__int128)r*x%m;
  return r;
}
void factorial_audit() {
  for (auto [p,pk] : vector<pll>{{2,524288},{3,531441},
        {5,390625},{7,823543},{997,994009},{999983,999983},
        {2,2},{2,4},{2,8},{3,3},{5,25}}) {
    vector<ll> unit(pk+1,1);
    for (int i = 1; i <= pk; ++i)
      unit[i] = i%p ? (__int128)unit[i-1]*i%pk : unit[i-1];
    for (ll n : {0LL,1LL,p-1,p,p+1,pk-1,pk,pk+1,
                 2*pk-1,2*pk,2*pk+1,LLONG_MAX}) {
      ll want = 1;
      for (ll m = n; m; m /= p)
        want = (__int128)want*mod_power(unit[pk],m/pk,pk)%pk,
        want = (__int128)want*unit[m%pk]%pk;
      factorial::prod[pk] = -1;
      require(factorial::fac_no_p(n,p,pk) == want,
              "large prime-power and full-width factorial");
      require(equal(ALL(unit),factorial::prod),
              "factorial prefix table regenerated, including n=0");
      if (n <= 2*pk+1) {
        ll direct = 1;
        for (ll i = 1; i <= n; ++i) {
          ll x = i; while (x%p == 0) x /= p;
          direct = (__int128)direct*x%pk;
        }
        require(direct == want, "independent stripped factorial");
      }
    }
  }
}

// Vertex connectivity of an induced subset, with optional removals.
int components(int mask, const vector<pii> &es, int skip = -1) {
  int count = 0, visited = 0;
  for (int root = 0; root < 8; ++root)
    if ((mask>>root&1) && !(visited>>root&1)) {
      ++count; vector<int> q{root}; visited |= 1<<root;
      for (int at = 0; at < SZ(q); ++at)
        for (int e = 0; e < SZ(es); ++e) if (e != skip) {
          auto [u,v] = es[e];
          if (v == q[at]) swap(u,v);
          if (u == q[at] && (mask>>v&1) && !(visited>>v&1))
            visited |= 1<<v, q.pb(v);
        }
    }
  return count;
}
void graph_case(int n, vector<pii> es) {
  shuffle(ALL(es),rng); BCC b(n); ECC e(n);
  for (auto [u,v] : es) b.add_edge(u,v), e.add_edge(u,v);
  b.solve(); e.solve();
  int all = (1<<n)-1, original = components(all,es);
  vector<int> ap(n), bridge(SZ(es));
  for (int u = 0; u < n; ++u)
    ap[u] = components(all^(1<<u),es) > original;
  for (int i = 0; i < SZ(es); ++i)
    bridge[i] = components(all,es,i) > original;
  require(b.is_ap == ap && e.is_bridge == bridge,
          "vertex/edge-removal connectivity");
  set<int> candidates, expected, actual;
  for (int mask = 1; mask <= all; ++mask) {
    if (components(mask,es) != 1) continue;
    bool block = true;
    for (int u = 0; u < n; ++u) if (mask>>u&1)
      block &= components(mask^(1<<u),es) <= 1;
    if (block) candidates.insert(mask);
  }
  for (int mask : candidates) {
    bool maximal = true;
    for (int larger : candidates)
      if (larger != mask && (mask&larger) == mask) maximal = false;
    if (maximal) expected.insert(mask);
  }
  for (auto block : b.bcc) {
    int mask = 0;
    for (int u : block) {
      require(!(mask>>u&1), "no repeated block vertices");
      mask |= 1<<u;
    }
    require(actual.insert(mask).second, "no repeated blocks");
  }
  require(actual == expected, "maximal induced biconnected subsets");
  vector<pii> remaining;
  for (int i = 0; i < SZ(es); ++i)
    if (!bridge[i]) remaining.pb(es[i]);
  for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v) {
    // Reachability via a temporary extra edge reduces component count.
    auto joined = remaining; joined.pb({u,v});
    bool same = components(all,remaining) == components(all,joined);
    require((e.bln[u] == e.bln[v]) == same, "bridge-deleted components");
  }
  auto blocks = b.bcc; auto labels = e.bln;
  b.solve(); e.solve();
  require(b.bcc == blocks && e.bln == labels && e.is_bridge == bridge,
          "repeated solve before block-cut construction");
  b.block_cut_tree();
  require(b.nbcc <= max(n,2*n-2), "block-cut forest capacity");
  int arcs = 0;
  for (int i = 0; i < SZ(b.bcc); ++i) {
    set<int> want;
    for (int u : b.bcc[i]) {
      if (ap[u]) want.insert(b.bln[u]);
      else require(b.bln[u] == i, "ordinary vertex block label");
    }
    require(set<int>(ALL(b.nG[i])) == want, "block-cut incidence");
  }
  for (int u = 0; u < b.nbcc; ++u) for (int v : b.nG[u]) {
    require(count(ALL(b.nG[v]),u) == 1, "undirected block-cut arcs");
    ++arcs;
  }
  require(arcs/2 == b.nbcc-original, "block-cut forest edge count");
}
void graph_audit() {
  graph_case(0,{});
  for (int tc = 0; tc < 800; ++tc) {
    int n = 1+rng()%8; vector<pii> es;
    for (int u = 0; u < n; ++u) for (int v = u; v < n; ++v)
      for (int copies = rng()%4; copies; --copies) {
        if (tc%3 == 0 && u != v && v != u+1) continue;
        if (rng()&1) es.pb({u,v}); else es.pb({v,u});
      }
    graph_case(n,es);
  }
}
void tree_audit() {
  // Cayley/Pruefer enumeration covers every labelled 6-vertex tree.
  for (int n = 1; n <= 6; ++n) {
    int codes = n <= 2 ? 1 : int(pow(n,n-2));
    for (int code = 0; code < codes; ++code) {
      vector<int> degree(n,1), sequence;
      for (int i = 0, x = code; i < n-2; ++i, x /= n)
        sequence.pb(x%n), ++degree[x%n];
      vector<vector<int>> g(n);
      auto add = [&](int u,int v) { g[u].pb(v), g[v].pb(u); };
      for (int v : sequence) {
        int u = find(ALL(degree),1)-degree.begin();
        add(u,v); --degree[u], --degree[v];
      }
      if (n > 1) {
        int u = find(ALL(degree),1)-degree.begin(); degree[u] = 0;
        int v = find(ALL(degree),1)-degree.begin(); add(u,v);
      }
      for (int root = 0; root < n; ++root)
        for (auto seed : {0ULL,1ULL,ULLONG_MAX}) {
          auto s = seed ? seed : tree_hash_default_seed;
          vector<int> order{root}, parent(n,-1); parent[root] = root;
          for (int i = 0; i < SZ(order); ++i)
            for (int v : g[order[i]]) if (v != parent[order[i]])
              parent[v] = order[i], order.pb(v);
          vector<tree_hash_u64> h(n,s);
          for (int i = n-1; i > 0; --i) {
            auto x = h[order[i]];
            x ^= x<<13; x ^= x>>7; x ^= x<<17;
            h[parent[order[i]]] += x;
          }
          require(rooted_tree_hash(g,root,seed) == h[root],
                  "every small labelled tree/root/seed");
        }
    }
  }
}
void palindrome_audit() {
  // Exhaust short strings where '%' is data, mixed with NUL/high bytes.
  const string alphabet{char(0),'%',char(128),char(255)};
  for (int n = 0; n <= 7; ++n) for (int code = 0; code < (1<<(2*n));
                                  ++code) {
    string s;
    for (int i = 0, x = code; i < n; ++i, x >>= 2)
      s += alphabet[x&3];
    palindrome::Manacher(s);
    for (int i = 0; i < n; ++i) {
      int r = 0;
      while (i-r >= 0 && i+r < n && s[i-r] == s[i+r]) ++r;
      require(palindrome::z[2*i+1]/2 == r, "odd byte palindrome");
      r = 0;
      while (i-r >= 0 && i+1+r < n && s[i-r] == s[i+1+r]) ++r;
      require(palindrome::z[2*i+2]/2 == r, "even byte palindrome");
    }
  }
  string s(2048,'%'); palindrome::Manacher(s);
  require(palindrome::z[2048] == 2049, "exact Manacher capacity");
  palindrome::Manacher("");
  require(palindrome::z[0] == 1, "reuse after full-capacity palindrome");
}
void triangle_audit() {
  vector<Point> base{{0,0},{1,0},{7,0},{8,2},{8,5},
                     {4,9},{0,9},{0,6},{0,1}};
  for (int tc = 0; tc < 120; ++tc) {
    vector<Point> p;
    double scale = tc%3 == 0 ? 0x1p-400 : tc%3 == 1 ? 1 : 0x1p400;
    double shift = tc%2 ? 0x1p40*scale : 0;
    for (auto q : base) for (int copy = 1+rng()%5; copy; --copy)
      p.pb({q.x*scale+shift,q.y*scale-shift});
    if (tc%2) p.pb(p[0]);
    // Exercise res[] indirection and both cyclic orientations.
    vector<int> ids(SZ(p)); iota(ALL(ids),0); shuffle(ALL(ids),rng);
    vector<Point> storage(SZ(p));
    for (int i = 0; i < SZ(p); ++i) storage[ids[i]] = p[i];
    double want = 0;
    for (int i = 0; i < SZ(p); ++i)
      for (int j = i+1; j < SZ(p); ++j)
        for (int k = j+1; k < SZ(p); ++k)
          want = max(want,fabs(Cross(p[j]-p[i],p[k]-p[i]))/2);
    for (int dir = 0; dir < 2; ++dir) {
      for (int i = 0; i < SZ(p); ++i) {
        require(ConvexHullMaxTriangleArea(storage.data(),ids.data(),
                SZ(p)) == want, "triangle scales/plateaus/index mapping");
        rotate(ids.begin(),ids.begin()+1,ids.end());
      }
      reverse(ALL(ids));
    }
  }
  require(ConvexHullMaxTriangleArea(nullptr,nullptr,0) == 0,
          "empty hull does not dereference pointers");
}
// Lifted determinant oracle uses a permutation expansion rather than the
// translated squared-distance formula of the template.
int circle_sign(array<delaunay::Point,4> p) {
  array<array<__int128,4>,4> a;
  for (int i = 0; i < 4; ++i) {
    __int128 x = p[i].x, y = p[i].y;
    a[i] = {x,y,x*x+y*y,1};
  }
  array<int,4> perm{0,1,2,3}; __int128 det = 0;
  do {
    __int128 term = 1; int inversions = 0;
    for (int i = 0; i < 4; ++i) {
      term *= a[i][perm[i]];
      for (int j = 0; j < i; ++j) inversions += perm[j] > perm[i];
    }
    det += inversions%2 ? -term : term;
  } while (next_permutation(ALL(perm)));
  return (det > 0)-(det < 0);
}
void delaunay_case(vector<delaunay::Point> p) {
  auto saved = p;
  auto triangles = delaunay::Triangulate(p);
  require(p == saved, "Delaunay preserves input");
  require(delaunay::edges.empty(), "Delaunay empty arena reclaimed");
  if (SZ(p) < 3) {
    require(triangles.empty(), "Delaunay small inputs"); return;
  }
  auto turn = [](auto a,auto b,auto c) { return (b-a)^(c-a); };
  bool collinear = true;
  for (auto q : p) collinear &= turn(p[0],p[1],q) == 0;
  if (collinear) {
    require(triangles.empty(), "Delaunay vertical/oblique collinearity");
    return;
  }
  sort(ALL(p)); vector<delaunay::Point> hull;
  for (int pass = 0; pass < 2; ++pass) {
    int start = SZ(hull);
    for (auto q : p) {
      while (SZ(hull) >= start+2 &&
             turn(hull[SZ(hull)-2],hull.back(),q) <= 0) hull.pop_back();
      hull.pb(q);
    }
    hull.pop_back(); reverse(ALL(p));
  }
  __int128 area = 0, total = 0;
  int boundary = 0;
  for (int i = 1; i+1 < SZ(hull); ++i)
    area += turn(hull[0],hull[i],hull[i+1]);
  for (auto q : p) {
    bool on = false;
    for (int i = 0; i < SZ(hull); ++i) {
      auto a = hull[i], b = hull[(i+1)%SZ(hull)];
      on |= turn(a,b,q) == 0 && min(a.x,b.x) <= q.x &&
        q.x <= max(a.x,b.x) && min(a.y,b.y) <= q.y && q.y <= max(a.y,b.y);
    }
    boundary += on;
  }
  require(SZ(triangles) == 2*SZ(p)-2-boundary, "Delaunay Euler count");
  set<delaunay::Point> used;
  map<pair<delaunay::Point,delaunay::Point>,int> incidence;
  for (auto t : triangles) {
    ll a = turn(t[0],t[1],t[2]); total += a;
    require(a > 0, "Delaunay triangle CCW");
    used.insert(ALL(t));
    for (auto q : p)
      require(circle_sign({t[0],t[1],t[2],q}) <= 0,
              "Delaunay exact lifted empty circle");
    for (int i = 0; i < 3; ++i) {
      auto u = t[i], v = t[(i+1)%3];
      if (v < u) swap(u,v);
      require(++incidence[{u,v}] <= 2, "Delaunay edge incidence");
    }
  }
  require(total == area && SZ(used) == SZ(p),
          "Delaunay area and boundary vertex coverage");
  int rim = 0;
  for (auto [edge,count] : incidence) rim += count == 1;
  require(rim == boundary, "Delaunay full boundary subdivision");
  for (auto [ab,count] : incidence) for (auto [cd,count2] : incidence) {
    auto [a,b] = ab; auto [c,d] = cd;
    auto opposite = [](ll x,ll y) { return (x<0 && y>0)||(x>0 && y<0); };
    require(!(opposite(turn(a,b,c),turn(a,b,d)) &&
              opposite(turn(c,d,a),turn(c,d,b))),
            "Delaunay no crossing edges");
  }
}
void delaunay_audit() {
  constexpr ll M = 500000000;
  for (auto p : vector<vector<delaunay::Point>>{
      {},{{0,0}},{{M,-M},{-M,M}},
      {{M,-M},{M,0},{M,M},{M,1}},
      {{-M,-M},{0,0},{M,M},{M-1,M-1}},
      {{-M,-M},{-M+1,-M},{M,M-1},{M,M}}}) delaunay_case(p);
  // Determinant-one leaves near both extremes; every input permutation.
  vector<delaunay::Point> tiny{{M-2,M-1},{M-1,M},{M,M}};
  sort(ALL(tiny));
  do { delaunay_case(tiny); } while (next_permutation(ALL(tiny)));
  for (int tc = 0; tc < 200; ++tc) {
    vector<delaunay::Point> p;
    ll step = tc%2 ? 250000000 : 1;
    ll shift = tc%2 ? 0 : M-2;
    for (int x = -2; x <= 2; ++x) for (int y = -2; y <= 2; ++y)
      if (rng()%3) p.pb({shift+x*step,shift+y*step});
    shuffle(ALL(p),rng); delaunay_case(p);
  }
}
int main() {
  hilbert_audit(); cyclic_audit(); factorial_audit(); graph_audit();
  tree_audit(); palindrome_audit(); triangle_audit(); delaunay_audit();
  cout << "PASS round14 Hilbert/cyclic/factorial/lowlink/tree/"
          "Manacher/triangle/Delaunay adversarial audit\n";
}
