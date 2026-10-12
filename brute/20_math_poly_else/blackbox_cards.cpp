// Runs every concrete example printed in the "Black-box Cards" section.
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
#define X first
#define Y second
#define SZ(a) int((a).size())
#define ALL(v) (v).begin(), (v).end()
#define pb push_back
#define CHECK(c) do { if (!(c)) { \
  cerr << "FAIL line " << __LINE__ << ": " #c "\n"; exit(1); } } while (0)

namespace vt {
const int N = 10; vector<int> adj[N]; int dfn[N], dep[N], par[N], T;
void dfs(int u, int p) { dfn[u] = T++, par[u] = p;
  for (int v : adj[u]) if (v != p) dep[v] = dep[u] + 1, dfs(v, u); }
int LCA(int a, int b) { while (a != b) { if (dep[a] < dep[b]) swap(a, b);
  a = par[a]; } return a; }
#include "../../codebook/2_Graph/Virtual_Tree.cpp"
}
namespace steiner { const int N = 8, T = 4, INF = 1e9;
#include "../../codebook/2_Graph/MinimumSteinerTree.cpp"
}
namespace dom { const int N = 10;
#include "../../codebook/2_Graph/Dominator_Tree.cpp"
}
namespace fwtns {
#include "../../codebook/7_Polynomial/Fast_Walsh_Transform.cpp"
}
namespace ccover { const int N = 6; using fwtns::fwt;
#include "../../codebook/2_Graph/Minimum_Clique_Cover.cpp"
}
namespace bkn { const int N = 64;
#include "../../codebook/2_Graph/NumberofMaximalClique.cpp"
}
#include "../../codebook/3_Data_Structure/IntervalContainer.cpp"
namespace seg2 {
#include "../../codebook/3_Data_Structure/Dynamic_2D_Segment_Tree.cpp"
const int N = 10;
#include "../../codebook/3_Data_Structure/Heavy_light_Decomposition.cpp"
}
namespace cent { const int N = 10;
#include "../../codebook/3_Data_Structure/Centroid_Decomposition.cpp"
}
namespace lichao { const ll INF = 4e18;
#include "../../codebook/3_Data_Structure/LiChaoST.cpp"
}
namespace mcmf_ns { const int N = 10; const ll INF = 1e18;
#include "../../codebook/4_Flow_Matching/MincostMaxflow.cpp"
}
namespace gm {
#include "../../codebook/4_Flow_Matching/Maximum_Simple_Graph_Matching.cpp"
}
namespace swc {
#include "../../codebook/4_Flow_Matching/SW-mincut.cpp"
}
namespace gh { const int MAXN = 10, INF = 1e9;
#include "../../codebook/4_Flow_Matching/Dinic.cpp"
#include "../../codebook/4_Flow_Matching/Gomory_Hu_tree.cpp"
}
namespace circ { const int N = 10; const ll INF = 1e18;
#include "../../codebook/4_Flow_Matching/MinCostCirculation.cpp"
}
namespace sais_ns {
#include "../../codebook/5_String/SAIS-C++20.cpp"
}
namespace ac_ns { const int len = 100, sigma = 26;
#include "../../codebook/5_String/Aho-Corasick_Automatan.cpp"
}
namespace rot {
#include "../../codebook/5_String/Smallest_Rotation.cpp"
}
namespace dbn {
#include "../../codebook/5_String/De_Bruijn_sequence.cpp"
}
namespace exsam { const int N = 100, CNUM = 26;
#include "../../codebook/5_String/exSAM.cpp"
}
namespace pal {
#include "../../codebook/5_String/PalTree.cpp"
}
namespace ml { const int kN = 50;
#include "../../codebook/5_String/MainLorentz.cpp"
}
namespace ggcd {
#include "../../codebook/6_Math/Gaussian_gcd.cpp"
}
namespace simeq { const int MAXN = 4;
#include "../../codebook/6_Math/Fraction.cpp"
#include "../../codebook/6_Math/Simultaneous_Equations.cpp"
}
namespace qr {
#include "../../codebook/6_Math/QuadraticResidue.cpp"
}
namespace det { const int MAXN = 4; const ll P = 1000000007;
#include "../../codebook/6_Math/Determinant.cpp"
}
namespace modp { const int MOD = 1000000007;
struct mint { int v; mint(ll x = 0) : v(int((x % MOD + MOD) % MOD)) {}
  mint operator+(mint b) const { return v + b.v; }
  mint operator-(mint b) const { return v - b.v; }
  mint operator*(mint b) const { return ll(v) * b.v; }
  mint operator-() const { return -v; }
  mint &operator+=(mint b) { return *this = *this + b; }
  mint &operator*=(mint b) { return *this = *this * b; }
  mint pw(ll e) const { mint r = 1, b = *this;
    for (; e; e >>= 1, b = b * b) if (e & 1) r = r * b; return r; }
  mint operator/(mint b) const { return *this * b.pw(MOD - 2); }
  bool operator==(mint b) const { return v == b.v; }
};
mint ifac[10], inegfac[10];
#include "../../codebook/6_Math/Berlekamp-Massey.cpp"
#include "../../codebook/7_Polynomial/Value_Poly.cpp"
}
#include "../../codebook/8_Geometry/Triangulation.cpp"
namespace geo { const double eps = 1e-9, pi = acos(-1);
int sign(double x) { return x < -eps ? -1 : x > eps; }
#include "../../codebook/8_Geometry/_basic.cpp"
#include "../../codebook/8_Geometry/sectorArea.cpp"
#include "../../codebook/8_Geometry/halfPlaneIntersect.cpp"
struct Point { double x, y; Point(double x = 0, double y = 0) : x(x), y(y) {}
  Point operator-(Point q) const { return {x - q.x, y - q.y}; } };
double Cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }
#include "../../codebook/8_Geometry/center.cpp"
}
namespace rsl {
#include "../../codebook/8_Geometry/rotatingSweepLine.cpp"
}
#include "../../codebook/9_Else/CDQ_Dominance.cpp"
#include "../../codebook/9_Else/HilbertCurve.cpp"
#include "../../codebook/9_Else/DynamicConvexTrick.cpp"
#include "../../codebook/9_Else/All_LCS.cpp"
#include "../../codebook/9_Else/AdaptiveSimpson.cpp"
#include "../../codebook/9_Else/simulated_annealing.cpp"
#include "../../codebook/9_Else/BinarySearchOnFraction.cpp"
#include "../../codebook/9_Else/min_plus_convolution.cpp"
namespace mo_upd {
#include "../../codebook/9_Else/Mos_Algorithm_With_modification.cpp"
}
namespace mo_tree {
#include "../../codebook/9_Else/Mos_Algorithm_On_Tree.cpp"
}

int main() {
  { using namespace vt; // tree 1-2, 2-3, 2-4 rooted at 1
    adj[1] = {2}; adj[2] = {1, 3, 4}; adj[3] = {2}; adj[4] = {2};
    dfs(1, 0); vector<int> keys = {4, 3};
    int r = build(keys); CHECK(r == 2);
    sort(ALL(vG[2])); CHECK(vG[2] == vector<int>({3, 4})); reset(r); }
  { using namespace steiner; static SteinerTree st; st.init(4);
    for (int v : {1, 2, 3}) st.add_edge(0, v, 1), st.add_edge(v, 0, 1);
    CHECK(st.solve({1, 2, 3}) == 3); }
  { using namespace dom; static dominator_tree dt; dt.init(4);
    dt.add_edge(1, 2), dt.add_edge(1, 3), dt.add_edge(2, 4);
    dt.add_edge(3, 4); dt.tarjan(1);
    auto c = dt.tree[1]; sort(ALL(c)); CHECK(c == vector<int>({2, 3, 4})); }
  { using namespace ccover; static Clique_Cover cc; cc.init(3);
    cc.add_edge(0, 1), cc.add_edge(1, 2); CHECK(cc.solve() == 2); }
  { using namespace bkn; static BronKerbosch bk; bk.init(4);
    bk.add_edge(1, 2), bk.add_edge(2, 3), bk.add_edge(1, 3);
    bk.add_edge(3, 4); CHECK(bk.solve() == 2); }
  { set<pii> s; addInterval(s, 1, 3); addInterval(s, 3, 5);
    CHECK(s == set<pii>({{1, 5}})); removeInterval(s, 2, 3);
    CHECK(s == set<pii>({{1, 2}, {3, 5}})); }
  { using namespace seg2; Seg2D st(4, 4); st.update(0, 2, 0, 2, 5);
    CHECK(st.query(1, 4, 1, 4) == 5 && st.query(0, 4, 0, 4) == 20); }
  { using namespace seg2; static Heavy_light_Decomposition h; h.init(3);
    h.add_edge(1, 2), h.add_edge(2, 3);
    for (int u = 1; u <= 3; ++u) h.val[u] = u;
    h.build(); CHECK(h.query(1, 3) == 6);
    h.seg.update(h.pl[2], h.pl[2] + 1, 10); CHECK(h.query(3, 1) == 16); }
  { using namespace cent; static Cent_Dec cd; cd.init(3);
    cd.add_edge(1, 2, 3), cd.add_edge(2, 3, 4); cd.build();
    cd.modify(1), cd.modify(3); CHECK(cd.query(2) == 7);
    CHECK(cd.query(1) == 7); }
  { using namespace lichao; LiChao lc(5); CHECK(lc.query(0) == -INF);
    lc.insert(L(1, 0, 0)), lc.insert(L(-1, 4, 1));
    CHECK(lc.query(0) == 4 && lc.query(3) == 3); }
  { using namespace mcmf_ns; static MinCostMaxFlow m; m.init(3);
    m.add_edge(0, 1, 2, 1), m.add_edge(1, 2, 1, 1), m.add_edge(0, 2, 1, 5);
    ll f, c; m.solve(0, 2, f, c); CHECK(f == 2 && c == 7); }
  { using namespace gm; Matching M(5);
    for (int i = 0; i < 5; ++i) M.add_edge(i, (i + 1) % 5);
    CHECK(M.solve() == 2); int un = 0;
    for (int i = 0; i < 5; ++i) un += M.match[i] == 5; CHECK(un == 1); }
  { using namespace swc; sw.init(4);
    for (int i = 0; i < 4; ++i) sw.addEdge(i, (i + 1) % 4, 1);
    CHECK(sw.solve(4) == 2); }
  { using namespace gh; Dinic.init(3);
    Dinic.add_edge(0, 1, 3), Dinic.add_edge(1, 0, 3);
    Dinic.add_edge(1, 2, 5), Dinic.add_edge(2, 1, 5); GomoryHu(3);
    CHECK(g[1] == 0 && gh_w[1] == 3 && g[2] == 1 && gh_w[2] == 5); }
  { using namespace circ; mcmf.init(2);
    mcmf.add_edge(0, 1, 2, -3), mcmf.add_edge(1, 0, 5, 1); mcmf.solve(2);
    ll cost = 0;
    for (int u = 0; u < 2; ++u) for (auto &e : mcmf.G[u])
      if (e.fcap > 0) cost += e.flow * e.cost;
    CHECK(cost == -4); }
  { using namespace sais_ns; string s = "banana";
    vector<unsigned char> a(ALL(s)); Suffix sf(a.data(), SZ(a));
    CHECK(sf.sa == vector<int>({5, 3, 1, 0, 4, 2}));
    CHECK(sf.hi == vector<int>({0, 1, 3, 0, 0, 2}));
    CHECK(sf.ra[0] == 3); }
  { using namespace ac_ns; ac.init(); vector<string> pats = {"AB", "BAB"};
    vector<int> ends; for (auto &p : pats) ends.pb(ac.input(p));
    ac.make_fl(); int x = 1;
    for (char c : string("ABABAB")) x = ac.rnx[x][c - 'A'], ++ac.cnt[x];
    ac.solve(); CHECK(ac.cnt[ends[0]] == 3 && ac.cnt[ends[1]] == 2); }
  CHECK(rot::mcp("bca") == "abc");
  { using namespace dbn; int out[5]; dbs.solve(2, 2, 4, out);
    set<int> w; for (int i = 0; i + 2 <= 5; ++i) w.insert(out[i]*2+out[i+1]);
    CHECK(SZ(w) == 4);
    cerr << "debruijn:"; for (int v : out) cerr << ' ' << v; cerr << '\n'; }
  { using namespace exsam; static exSAM ex; ex.init();
    ex.insert("ab"), ex.insert("b"); ex.build(); ex.solve();
    ll distinct = 0;
    for (int v = 1; v < ex.tot; ++v) distinct += ex.len[v] - ex.len[ex.link[v]];
    CHECK(distinct == 3);
    int vb = ex.next[0]['b' - 'a']; CHECK(ex.cnt[vb] == 2);
    int va = ex.next[0]['a' - 'a']; CHECK(ex.cnt[va] == 1); }
  { using namespace pal; palindromic_tree t;
    for (char c : string("aaa")) t.add(c);
    t.count(); CHECK(t.size() == 3); int tot = 0;
    for (int i = 2; i < SZ(t.St); ++i) tot += t.St[i].cnt; CHECK(tot == 6); }
  { using namespace ml; main_lorentz("aabab");
    CHECK(rep[1] == vector<pii>({{0, 0}}));
    CHECK(rep[2] == vector<pii>({{1, 1}})); }
  { using namespace ggcd; auto g = gaussian_gcd({4, 2}, {2, 0});
    // gcd(4+2i, 2) = 2 up to a unit; 4+2i = 2(2+i)
    CHECK(norm(complex<double>(g.real(), g.imag())) == 4); }
  { using namespace simeq; matrix eq{}; eq.n = eq.m = 2;
    eq.M[0][0] = 1, eq.M[0][1] = 1, eq.M[0][2] = 3;
    eq.M[1][0] = 2, eq.M[1][1] = -1, eq.M[1][2] = 0;
    CHECK(eq.solve() == 2);
    CHECK(eq.sol[0].n == 1 && eq.sol[0].d == 1);
    CHECK(eq.sol[1].n == 2 && eq.sol[1].d == 1); }
  { using namespace qr; int r = QuadraticResidue(10, 13);
    CHECK(r == 6 || r == 7); CHECK(QuadraticResidue(5, 13) == -1);
    CHECK(Jacobi(10, 13) == 1 && Jacobi(5, 13) == -1); }
  { using namespace det; Matrix a; a.n = a.m = 2;
    a.M[0][0] = 1, a.M[0][1] = 2, a.M[1][0] = 3, a.M[1][1] = 4;
    CHECK(a.det() == P - 2); }
  { using namespace modp;
    auto c = BerlekampMassey(vector<mint>{0, 1, 1, 2, 3, 5, 8});
    CHECK(SZ(c) == 2 && c[0] == 1 && c[1] == 1);
    ifac[0] = inegfac[0] = 1;
    for (int i = 1; i < 10; ++i) ifac[i] = ifac[i-1] / i,
      inegfac[i] = inegfac[i-1] / (-i);
    Poly f(0, 0); f.poly = {0, 1}; CHECK(f.get_val(100).v == 100);
    f.raise(); CHECK(f.get_val(100).v == 5050); }
  { using namespace delaunay;
    auto t = Triangulate({{0, 0}, {4, 0}, {0, 4}, {1, 1}});
    CHECK(SZ(t) == 3);
    for (auto &tr : t) CHECK(((tr[1] - tr[0]) ^ (tr[2] - tr[0])) > 0); }
  { using namespace geo;
    CHECK(abs(SectorArea(P(1, 0), P(0, 1), 2) - pi) < 1e-9);
    vector<L> hs = {L({0,0},{1,0}), L({1,0},{1,1}), L({1,1},{0,1}),
                    L({0,1},{0,0}), L({0,.5},{1,.5})}; // and y >= .5
    auto poly = HPI(hs); CHECK(SZ(poly) == 4);
    double area = 0;
    for (int i = 0; i < SZ(poly); ++i)
      area += poly[i] ^ poly[(i + 1) % SZ(poly)];
    CHECK(abs(area / 2 - 0.5) < 1e-9);
    vector<Point> sq = {{0, 0}, {2, 0}, {2, 2}, {0, 2}};
    auto c = BaryCenter(sq, 4);
    CHECK(abs(c.x - 1) < 1e-9 && abs(c.y - 1) < 1e-9); }
  { auto o = rsl::rotatingSweepLineOrder({{0, 0}, {2, 0}, {1, 1}});
    CHECK(SZ(o) == 3);
    cerr << "sweep:"; for (int v : o) cerr << ' ' << v; cerr << '\n'; }
  CHECK(dominance({{1, 1}, {2, 2}, {2, 0}}) == vector<ll>({0, 2, 0}));
  CHECK(hilbert(2, 0, 0) == 0 && hilbert(2, 0, 1) == 1 &&
        hilbert(2, 1, 1) == 2 && hilbert(2, 1, 0) == 3);
  { DynamicHull h; h.addline(2, 3), h.addline(-1, 10);
    CHECK(h.query(1) == 9 && h.query(5) == 13); }
  CHECK(all_lcs("abc", "acb") == vector<string>({"ab", "ac"}));
  { auto f = make_simpson([](double x) { return sin(x); });
    CHECK(abs(f.eval(0, acos(-1.0), 1e-10) - 2) < 1e-8); }
  { mt19937_64 rng(7122);
    int best = simulated_annealing(0, [](int x) { return abs(x - 3); },
      [](int x, auto &) { return x < 3 ? x + 1 : x - 1; }, rng);
    CHECK(best == 3); }
  { Q x = frac_bs(100, [](Q v) { return v.q == 0 || 3*v.p >= 2*v.q; });
    CHECK(x.p == 2 && x.q == 3); }
  CHECK(min_plus_convolution(vector<int>{0, 1, 4}, vector<int>{3, 0})
        == vector<int>({3, 0, 1, 4}));
  { // Mo with updates: a = {1,2,1}; distinct count queries.
    using namespace mo_upd; vector<int> a = {1, 2, 1}, cnt(10);
    int distinct = 0; vector<pii> upd = {{0, 0}, {1, 1}}; // t=1: a[1]=1
    vector<Query> qs; qs.emplace_back(0, 2, 0, 1, 0);
    qs.emplace_back(0, 2, 1, 1, 1); vector<int> ans(2);
    auto add = [&](int i) { distinct += cnt[a[i]]++ == 0; };
    auto sub = [&](int i) { distinct -= --cnt[a[i]] == 0; };
    auto swap_val = [&](int t, int L, int R) {
      auto &[pos, val] = upd[t]; bool in = L <= pos && pos <= R;
      if (in) sub(pos); swap(a[pos], val); if (in) add(pos); };
    solve(qs, swap_val, swap_val, add, sub,
          [&](const Query &q) { ans[q.id] = distinct; });
    CHECK(ans[0] == 2 && ans[1] == 1); }
  { // Mo on tree: path 0-1-2, star at 1 with 3; colors.
    using namespace mo_tree; int n = 4;
    vector<vector<int>> g(n); auto ae = [&](int u, int v) {
      g[u].pb(v), g[v].pb(u); };
    ae(0, 1), ae(1, 2), ae(1, 3);
    vector<int> in(n), out(n), ord, dep(n), par(n), col = {5, 7, 5, 9};
    auto dfs = [&](auto &&self, int u, int p) -> void {
      in[u] = SZ(ord), ord.pb(u), par[u] = p;
      for (int v : g[u]) if (v != p) dep[v] = dep[u] + 1, self(self, v, u);
      out[u] = SZ(ord), ord.pb(u); };
    dfs(dfs, 0, 0);
    auto lca = [&](int a, int b) { while (a != b) {
      if (dep[a] < dep[b]) swap(a, b); a = par[a]; } return a; };
    vector<int> on(n), cnt(10); int distinct = 0;
    auto flip = [&](int v) { if ((on[v] ^= 1)) distinct += cnt[col[v]]++ == 0;
      else distinct -= --cnt[col[v]] == 0; };
    vector<Query> qs; qs.emplace_back(2, 3, 2, in, out, lca, 0);
    qs.emplace_back(0, 2, 2, in, out, lca, 1); vector<int> ans(2);
    solve(qs, ord, flip, [&](const Query &q) { ans[q.id] = distinct; });
    CHECK(ans[0] == 3 && ans[1] == 2); }
  cout << "blackbox_cards: PASS\n";
}
