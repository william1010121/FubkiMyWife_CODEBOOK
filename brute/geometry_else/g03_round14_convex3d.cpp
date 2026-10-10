// Tests codebook/8_Geometry/convex3D.cpp with exact integer predicates.
#include "../../codebook/1_Basic/Default_code.cpp"
const double eps = 1e-9;
struct P {
  double x, y, z;
  P operator-(P b) const { return {x-b.x, y-b.y, z-b.z}; }
  P operator^(P b) const {
    return {y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x};
  }
  double operator*(P b) const { return x*b.x+y*b.y+z*b.z; }
};
double abs(P p) { return sqrt(p*p); }
#include "../../codebook/8_Geometry/convex3D.cpp"
void req(bool ok, const char *what) {
  if (!ok) { cerr << "FAIL " << what << '\n'; abort(); }
}
using Plane = array<ll,4>;
Plane plane(P a, P b, P c) {
  P v = (b-a)^(c-a);
  Plane key{llround(v.x),llround(v.y),llround(v.z),llround(v*a)};
  ll g = 0;
  for (ll x : key) g = gcd(g, abs(x));
  req(g, "nonzero plane normal");
  for (ll &x : key) x /= g;
  for (ll x : key) if (x) {
    if (x < 0) for (ll &y : key) y = -y;
    break;
  }
  return key;
}
// Exhaustive supporting planes, independent of the incremental hull.
void check_3d(convex3D &h) {
  set<Plane> want, got;
  for (int a = 0; a < h.n; ++a) for (int b = a+1; b < h.n; ++b)
    for (int c = b+1; c < h.n; ++c) {
      P v = (h.p[b]-h.p[a])^(h.p[c]-h.p[a]);
      bool pos = false, neg = false;
      for (int i = 0; i < h.n; ++i) {
        double d = v*(h.p[i]-h.p[a]);
        pos |= d > eps; neg |= d < -eps;
      }
      if (pos != neg) want.insert(plane(h.p[a],h.p[b],h.p[c]));
    }
  map<pii,int> edges; set<int> vertices;
  for (int i = 0; i < h.m; ++i) {
    auto t = h.f[i]; req(t.res, "active compacted triangle");
    got.insert(plane(h.p[t.a],h.p[t.b],h.p[t.c]));
    for (int j = 0; j < h.n; ++j)
      req(!h.on(h.f[i],h.p[j]), "outward supporting face");
    for (auto e : {pii{t.a,t.b},pii{t.b,t.c},pii{t.c,t.a}})
      ++edges[e], vertices.insert(e.first);
  }
  req(want == got, "exhaustive supporting planes");
  req(h.faces() == int(want.size()), "geometric facet count");
  for (auto [e,count] : edges)
    req(count == 1 && edges[{e.second,e.first}] == 1,
        "closed directed manifold");
  req(h.m == 2*int(vertices.size())-4, "Euler triangle count");
}
int main() {
  static convex3D h;
  vector<P> cube;
  for (int x : {-1,1}) for (int y : {-1,1}) for (int z : {-1,1})
    cube.push_back({double(1000 + x + y),double(-2000 + y + z),
                   double(3000 - z)});
  array<int,8> order{0,1,2,3,4,5,6,7};
  int cases = 0;
  do {
    h.n = 8; h.m = 0;
    for (int i = 0; i < 8; ++i) h.p[i] = cube[order[i]];
    h(); check_3d(h); req(h.faces() == 6, "cube six geometric facets");
    req(h.faces() == 6, "repeat facet query");
    for (int i = 0; i < h.m; ++i) {
      auto t = h.f[i];
      req(abs((h.p[t.b] - h.p[t.a]) ^ (h.p[t.c] - h.p[t.a])) > eps,
          "nondegenerate triangles");
      for (auto [a,b] : {pair{t.a,t.b},pair{t.b,t.c},pair{t.c,t.a}})
        req(h.id[a][b] == i, "directed adjacency after compaction");
    }
    ++cases;
  } while (next_permutation(order.begin(),order.end()));
  // Long collinear prefixes and plane-interior points precede extreme
  // vertices: initial selection must not mistake the prefix for the hull.
  mt19937 rng(140308);
  vector<P> p{{0,0,0},{1,0,0},{2,0,0},{3,0,0},{0,1,0},{1,1,0},
              {0,0,1},{4,0,0},{0,4,0},{0,0,4},{1,1,1},{2,1,1}};
  for (int tc = 0; tc < 256; ++tc) {
    h.n = p.size(); h.m = 0; copy(p.begin(),p.end(),h.p);
    h(); check_3d(h); req(h.faces() == 4, "tetrahedron with boundary");
    shuffle(p.begin(),p.end(),rng);
  }
  cout << "PASS convex3D: " << cases << " cube permutations, "
       << "256 collinear/coplanar-prefix orders\n";
}
