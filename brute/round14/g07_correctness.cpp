#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define ALL(v) (v).begin(), (v).end()
#define SZ(v) int((v).size())
#define pb push_back

namespace treap {
#include "../../codebook/3_Data_Structure/Treap.cpp"
void audit() {
  // Destructive root deletion after every alias split, including ties.
  for (int code = 0; code < 4096; ++code) {
    vector<int> want;
    node *root = nullptr;
    int x = code;
    for (int i = 0; i < 6; ++i, x /= 4) {
      int key = array{INT_MIN, -1, 0, INT_MAX}[x % 4];
      insert(root, key); want.pb(key);
    }
    sort(ALL(want));
    for (int k = 0; k <= 6; ++k) {
      node *a;
      split2(root, a, root, k);
      assert(sz(a) == k && sz(root) == 6 - k);
      root = merge(a, root);
      interval(root, k + 1, 6 - k);
      for (int j = 0; j < 6; ++j)
        assert(kth(root, j + 1)->data == want[j]);
    }
    while (root) {
      int key = root->data;
      auto it = lower_bound(ALL(want), key);
      assert(Rank(root, key) == it - want.begin());
      want.erase(it); assert(erase(root, key));
      assert(sz(root) == SZ(want));
      for (int j = 0; j < SZ(want); ++j)
        assert(kth(root, j + 1)->data == want[j]);
    }
    assert(!erase(root, INT_MIN) && Rank(root, INT_MAX) == 0);
  }
}
}
namespace floors {
#include "../../codebook/6_Math/floor_sum.cpp"
void audit() {
  // Independent direct wide sum. Products in these calls fit ll.
  for (ll n = 0; n <= 24; ++n)
    for (ll m : {1LL, 2LL, 97LL, 1000000007LL})
      for (ll a : {0LL, 1LL, m - 1, m, m + 1, 9999999999LL})
        for (ll b : {0LL, 1LL, m - 1, m, m + 1, 9999999999LL}) {
          __int128 want = 0;
          for (ll i = 0; i < n; ++i) want += ((__int128)a*i+b)/m;
          assert(floor_sum(n, m, a, b) == want);
        }
  for (ll n : {0LL, 1LL, LLONG_MAX}) {
    assert(floor_sum(n, LLONG_MAX, 0, LLONG_MAX - 1) == 0);
    assert(floor_sum(n, 1, 0, 1) == n);
  }
}
}
namespace strings {
const int kN = 64;
#include "../../codebook/5_String/MainLorentz.cpp"
void audit() {
  // Binary words at extreme legal offsets; count reports, not just sets.
  for (int n = 0; n <= 13; ++n) for (int code = 0; code < (1<<n); ++code) {
    string s(n, '\0');
    for (int i = 0; i < n; ++i) s[i] = code >> i & 1 ? char(255) : '\0';
    int offset = code & 1 ? INT_MIN : INT_MAX - max(0, n - 1);
    main_lorentz(s, offset);
    vector<vector<int>> count(kN, vector<int>(n));
    for (int l = 0; l < kN; ++l) for (auto [L, R] : rep[l]) {
      assert(l > 0 && L <= R);
      for (ll start = L; start <= R; ++start) {
        ll p = start - offset;
        assert(0 <= p && p + 2*l <= n);
        ++count[l][p];
      }
    }
    for (int l = 1; l < kN; ++l) for (int p = 0; p < n; ++p) {
      bool square = p + 2*l <= n &&
        s.compare(p, l, s, p + l, l) == 0;
      assert(count[l][p] == square);
    }
    // Appending the same source must append identical intervals.
    vector<vector<pair<int,int>>> saved(begin(rep), end(rep));
    main_lorentz_impl(s, offset);
    for (int l = 0; l < kN; ++l) {
      assert(rep[l].size() == 2*saved[l].size());
      assert(equal(ALL(saved[l]), rep[l].begin()));
      assert(equal(ALL(saved[l]), rep[l].begin() + saved[l].size()));
    }
  }
}
}
namespace geometry {
const double eps = 1e-9, pi = acos(-1.);
double sq(double x) { return x*x; }
int sign(double x) { return x < -eps ? -1 : x > eps; }
#include "../../codebook/8_Geometry/_basic.cpp"
struct C { P c; double r; };
vector<P> p;
double dist(P a, P b) { return hypot(a.x-b.x, a.y-b.y); }
#include "../../codebook/8_Geometry/ClosestPair.cpp"
#include "../../codebook/8_Geometry/circleOrArea.cpp"
void audit() {
  mt19937 rng(1407);
  for (int tc = 0; tc < 5000; ++tc) {
    p.clear(); int n = rng() % 24;
    for (int i = 0; i < n; ++i)
      p.emplace_back(int(rng()%5), int(rng()%21)-10);
    sort(ALL(p), [](P a, P b) { return a.x < b.x; });
    auto saved = p;
    for (int l = 0; l <= n; ++l) for (int r = l-1; r < n; ++r) {
      double want = l >= r ? 1e9 : INFINITY;
      for (int i = l; i <= r; ++i) for (int j = i+1; j <= r; ++j)
        want = min(want, hypot(p[i].x-p[j].x, p[i].y-p[j].y));
      assert(closest_pair(l, r) == want);
    }
    for (int i = 0; i < n; ++i)
      assert(p[i].x == saved[i].x && p[i].y == saved[i].y);
  }
  // Three equal disks with pairwise lenses, but no triple intersection.
  // Inclusion-exclusion gives an independent analytic union oracle.
  for (double d : {1.75, 1.8, 1.9, 2.0, 2.1})
    for (double scale : {.1, 1., 100.}) {
      double lens = d >= 2 ? 0 : 2*acos(d/2)-d*sqrt(4-d*d)/2;
      double want = (3*pi - 3*lens)*scale*scale;
      for (int rotation = 0; rotation < 12; ++rotation) {
        vector<C> c{{{0,0},scale}, {{d*scale,0},scale},
                    {{d*scale/2,d*sqrt(3.)*scale/2},scale}};
        for (auto &v : c)
          v.c = v.c.rot(rotation*pi/6) + P(10000,-10000);
        for (int first = 0; first < 3; ++first) {
          assert(abs(CircleUnionArea(c)-want) < 1e-8*max(1.,want));
          rotate(c.begin(), c.begin()+1, c.end());
        }
      }
    }
  // Cover segments independently checked by sampling boundary points.
  for (int r = 1; r <= 4; ++r) for (int s = 0; s <= 4; ++s)
    for (int x = -6; x <= 6; ++x) for (int y = -6; y <= 6; ++y) {
      if (!x && !y && r == s) continue;
      C a{{0,0},double(r)}, b{{double(x),double(y)},double(s)};
      auto arcs = CoverSegment(a,b);
      vector<pair<double,double>> appended{{-7,-6}};
      AppendCoverSegment(a,b,appended);
      assert(appended.front() == make_pair(-7.,-6.));
      assert(vector(appended.begin()+1,appended.end()) == arcs);
      for (int k = 0; k < 71; ++k) {
        double t = (k+.314159)*2*pi/71;
        P boundary(r*cos(t), r*sin(t));
        double gap = dist(boundary,b.c)-s;
        if (abs(gap) < 1e-7) continue; // exclude sampled arc endpoints
        bool covered = false;
        for (auto [l,h] : arcs) covered |= l <= t && t <= h;
        assert(covered == (gap < 0));
      }
    }
}
}
namespace convolution {
#include "../../codebook/9_Else/min_plus_convolution.cpp"
void audit() {
  mt19937 rng(1408);
  assert(min_plus_convolution(vector<ll>{}, vector<ll>{1}).empty());
  for (int tc = 0; tc < 10000; ++tc) {
    int n = 1+rng()%30, m = 1+rng()%30;
    vector<ll> a(n), b(m), want(n+m-1, LLONG_MAX);
    // Offsets near both ll extremes; every pair sum is exact and fits.
    a[0] = tc & 1 ? LLONG_MIN + 100000 : LLONG_MAX - 100000;
    ll slope = -100;
    for (int i = 1; i < n; ++i) {
      slope += rng()%9; a[i] = a[i-1]+slope;
    }
    for (auto &v : b) v = int(rng()%2001)-1000;
    for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) {
      __int128 sum = (__int128)a[i]+b[j];
      assert(LLONG_MIN <= sum && sum <= LLONG_MAX);
      want[i+j] = min(want[i+j], ll(sum));
    }
    assert(min_plus_convolution(a,b) == want);
  }
}
}
namespace fractions {
#include "../../codebook/9_Else/BinarySearchOnFraction.cpp"
void audit() {
  // Irrational thresholds and strict square thresholds; exhaustive oracle.
  for (ll n = 1; n <= 40; ++n) for (ll k = 0; k <= 100; ++k)
    for (bool strict : {false,true}) {
      auto pred = [&](Q x) {
        if (!x.q) return true;
        ll diff = x.p*x.p-k*x.q*x.q;
        return strict ? diff > 0 : diff >= 0;
      };
      Q want{1,0};
      for (ll p = 0; p <= n; ++p) for (ll q = 1; q <= n; ++q)
        if (pred({p,q}) && (!want.q || p*want.q < want.p*q))
          want = {p,q};
      Q got = frac_bs(n,pred);
      assert(gcd(got.p,got.q) == 1 && pred(got));
      assert(0 <= got.p && got.p <= n && 0 <= got.q && got.q <= n);
      assert(bool(got.q) == bool(want.q));
      assert(got.p*want.q == want.p*got.q);
    }
}
}
namespace bounded {
const int MAXN = 16, INF = 0x3f3f3f3f;
#include "../../codebook/4_Flow_Matching/Dinic.cpp"
#include "../../codebook/4_Flow_Matching/BoundedFlow.cpp"
void audit() {
  BoundedFlow bf;
  for (int lo : {0, 1, INF/2}) {
    bf.init(4);
    bf.add_edge(0,1,lo,INF); bf.add_edge(1,3,lo,INF);
    bf.add_edge(0,2,lo,INF); bf.add_edge(2,3,lo,INF);
    bf.add_edge(1,1,INF,INF); // no demand, no augmentable self-loop
    assert(bf.solve(0,3) == 2*INF); // exceeds INF, fits int
    assert(bf.G[4].empty() && bf.G[5].empty() && bf.G[6].empty());
    vector<ll> balance(4);
    for (int u = 0; u < 4; ++u) for (int id = 0; id < SZ(bf.G[u]); ++id) {
      auto e = bf.G[u][id]; auto r = bf.G[e.to][e.rev];
      assert(r.to == u && r.rev == id);
      if (e.cap) {
        assert(0 <= e.flow && e.flow <= e.cap);
        balance[u] += e.flow; balance[e.to] -= e.flow;
      }
    }
    assert(balance == vector<ll>({2LL*INF,0,0,-2LL*INF}));
  }
  // Forced cycle at INF and forced negative source value.
  bf.init(2); bf.add_edge(0,1,INF,INF); bf.add_edge(1,0,INF,INF);
  assert(bf.solve());
  bf.init(2); bf.add_edge(1,0,1,1); assert(bf.solve(0,1) == -1);
  bf.init(0); assert(bf.solve());
}
}
namespace steiner {
const int N = 6, T = 4, INF = INT_MAX;
#include "../../codebook/2_Graph/MinimumSteinerTree.cpp"
struct Edge { int u,v,w; };
int oracle(int n, const vector<Edge> &es, const vector<int> &vc,
           const vector<int> &term, int mask, int sink) {
  ll best = INF;
  for (int subset = 0; subset < (1<<SZ(es)); ++subset) {
    vector<vector<int>> g(n); vector<bool> used(n);
    used[sink] = true; ll cost = 0;
    for (int i = 0; i < SZ(term); ++i)
      if (mask >> i & 1) used[term[i]] = true;
    for (int i = 0; i < SZ(es); ++i) if (subset >> i & 1) {
      auto [u,v,w] = es[i]; used[u] = used[v] = true;
      g[u].pb(v); cost += w;
    }
    for (int i = 0; i < n; ++i) if (used[i]) cost += vc[i];
    if (cost >= best) continue;
    bool ok = true;
    for (int i = 0; i < SZ(term); ++i) if (mask >> i & 1) {
      vector<bool> seen(n); vector<int> q{term[i]}; seen[term[i]] = true;
      for (int j = 0; j < SZ(q); ++j) for (int v : g[q[j]])
        if (!seen[v]) seen[v] = true, q.pb(v);
      ok &= seen[sink];
    }
    if (ok) best = cost;
  }
  return best;
}
void audit() {
  static SteinerTree st;
  mt19937 rng(1409);
  const vector<int> costs{0,1,7,INF/2,INF-1};
  for (int tc = 0; tc < 500; ++tc) {
    int n = 1+rng()%N;
    vector<int> vc(n), term; vector<Edge> es;
    st.init(n);
    for (int i = 0; i < n; ++i)
      st.vcst[i] = vc[i] = costs[rng()%SZ(costs)];
    for (int i = 0; i < 7; ++i) {
      Edge e{int(rng()%n),int(rng()%n),costs[rng()%SZ(costs)]};
      es.pb(e); st.add_edge(e.u,e.v,e.w);
    }
    for (int i = 0; i < min(n,T); ++i) term.pb(i);
    auto duplicates = term;
    duplicates.insert(duplicates.end(), ALL(term));
    int got = st.solve(duplicates), want = INF;
    for (int mask = 0; mask < (1<<SZ(term)); ++mask)
      for (int sink = 0; sink < n; ++sink) {
        int expected = oracle(n,es,vc,term,mask,sink);
        assert(st.dp[mask][sink] == expected);
        if (mask == (1<<SZ(term))-1) want = min(want,expected);
      }
    assert(got == want);
  }
  st.init(0); assert(st.solve({}) == 0);
}
}
int main() {
  treap::audit(); floors::audit(); strings::audit(); geometry::audit();
  convolution::audit(); fractions::audit(); bounded::audit();
  steiner::audit();
  cout << "PASS round14 g07: nine independent edge-case audits\n";
}
