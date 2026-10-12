#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define pb push_back
#define SZ(a) ((int)(a).size())

namespace centroid {
struct Point {
  double x, y;
  Point(double x = 0, double y = 0) : x(x), y(y) {}
  Point operator-(Point q) const { return {x - q.x, y - q.y}; }
};
double Cross(Point a, Point b) { return a.x*b.y - a.y*b.x; }
#include "codebook/8_Geometry/center.cpp"
void audit() {
  mt19937 rng(1401);
  for (int tc = 0; tc < 100; ++tc) {
    int n = 1 + rng()%12;
    vector<int> h(n);
    double area = 0, mx = 0, my = 0;
    for (int i = 0; i < n; ++i) {
      h[i] = 1 + rng()%20;
      area += h[i], mx += h[i]*(i + .5), my += h[i]*h[i]/2.;
    }
    vector<Point> p{{0, 0}, {double(n), 0}};
    for (int i = n - 1; i >= 0; --i) {
      if (i == n - 1 || h[i] != h[i + 1])
        p.push_back({double(i + 1), double(h[i])});
      p.push_back({double(i), double(h[i])});
    }
    // Rectangle moments, then a nonsingular affine transformation.
    int a, b, c, d;
    do {
      a = int(rng()%7)-3, b = int(rng()%7)-3;
      c = int(rng()%7)-3, d = int(rng()%7)-3;
    } while (a*d == b*c);
    double shift = tc%2 ? 1099511627776. : 0.;
    Point want(a*mx/area + b*my/area + shift,
               c*mx/area + d*my/area - shift);
    for (auto &q : p)
      q = {a*q.x + b*q.y + shift, c*q.x + d*q.y - shift};
    for (int dir = 0; dir < 2; ++dir) {
      for (int i = 0; i < SZ(p); ++i) {
        auto got = BaryCenter(p, INT_MIN);
        assert(abs(got.x - want.x) <= .001);
        assert(abs(got.y - want.y) <= .001);
        rotate(p.begin(), p.begin() + 1, p.end());
      }
      reverse(p.begin(), p.end());
    }
  }
}
}

namespace mo {
#include "codebook/9_Else/Mos_Algorithm_On_Tree.cpp"
void audit() {
  solve({}, {}, [](int) { assert(false); },
        [](const Query &) { assert(false); });
  mt19937 rng(1402);
  for (int n : {1, 2, 17, 64}) for (int shape = 0; shape < 3; ++shape) {
    vector<vector<int>> g(n);
    for (int v = 1; v < n; ++v) {
      int u = shape == 0 ? v - 1 : shape == 1 ? 0 : rng()%v;
      g[u].push_back(v), g[v].push_back(u);
    }
    int root = rng()%n;
    vector<int> par(n, -1), dep(n), in(n), out(n), ord;
    auto dfs = [&](auto &&self, int u) -> void {
      in[u] = SZ(ord), ord.push_back(u);
      for (int v : g[u]) if (v != par[u]) {
        par[v] = u, dep[v] = dep[u] + 1, self(self, v);
      }
      out[u] = SZ(ord), ord.push_back(u);
    };
    dfs(dfs, root);
    auto lca = [&](int u, int v) {
      while (dep[u] > dep[v]) u = par[u];
      while (dep[v] > dep[u]) v = par[v];
      while (u != v) u = par[u], v = par[v];
      return u;
    };
    for (int block : {1, 7, INT_MAX}) {
      vector<Query> qs;
      map<tuple<int, int, int>, vector<int>> want;
      for (int u = 0; u < n; ++u) {
        vector<int> prev(n, -1); queue<int> bfs;
        prev[u] = u, bfs.push(u);
        while (!bfs.empty()) {
          int v = bfs.front(); bfs.pop();
          for (int w : g[v]) if (prev[w] == -1)
            prev[w] = v, bfs.push(w);
        }
        for (int v = 0; v < n; ++v) {
          Query q(u, v, block, in, out, lca, INT_MIN);
          vector<int> mask(n); mask[u] = 1;
          for (int w = v; w != u; w = prev[w]) mask[w] = 1;
          want[{q.L, q.R, q.lca}] = mask;
          qs.push_back(q), qs.push_back(q);
        }
      }
      shuffle(qs.begin(), qs.end(), rng);
      vector<int> active(n); int answers = 0;
      solve(qs, ord, [&](int u) { active[u] ^= 1; },
        [&](const Query &q) {
          assert(q.id == INT_MIN);
          assert(active == want.at({q.L, q.R, q.lca}));
          ++answers;
        });
      assert(answers == 2*n*n);
      vector<int> last(n); auto sorted = qs;
      sort(sorted.begin(), sorted.end());
      for (int i = sorted.back().L; i <= sorted.back().R; ++i)
        last[ord[i]] ^= 1;
      assert(active == last); // Extra LCA is removed after answering.
    }
  }
}
}

namespace witness {
ll mul(ll a, ll b, ll n) { return (__int128)a*b%n; }
#include "codebook/6_Math/Miller_Rabin.cpp"
void audit() {
  for (ll p : {3LL, 5LL, 97LL, 65537LL, 2147483647LL}) {
    ll n = p*p;
    // A nonzero witness sharing a factor cannot be a strong liar.
    for (ll a : {p, 2*p, n - p, LLONG_MAX/ p * p})
      if (a%n) assert(!Miller_Rabin(a, n));
    for (ll a : {0LL, n, 2*n}) assert(Miller_Rabin(a, n));
  }
  for (ll n : {561LL, 1105LL, 1729LL, 2465LL, 6601LL, 41041LL})
    for (ll a = 1; a < n; ++a)
      if (gcd(a, n) > 1) assert(!Miller_Rabin(a, n));
  assert(Miller_Rabin(0, 4) && !Miller_Rabin(2, 4));
  for (ll n : {LLONG_MIN, -1LL, 0LL, 1LL})
    assert(!Miller_Rabin(LLONG_MAX, n));
}
}

namespace modmin {
#include "codebook/6_Math/ModMin.cpp"
ll singleton(ll a, ll m, ll l) {
  ll g = gcd(a, m);
  if (l%g) return -1;
  a /= g, m /= g, l /= g;
  if (m == 1) return 0;
  ll u = 1, v = 0, x = a, y = m;
  while (y) {
    ll q = x/y;
    tie(x, y) = pair{y, x - q*y};
    tie(u, v) = pair{v, u - q*v};
  }
  return (__int128)((u%m + m)%m)*l%m;
}
void audit() {
  assert(mod_min(1, LLONG_MAX, LLONG_MAX-2, LLONG_MAX-1)
         == LLONG_MAX-2);
  assert(mod_min(0, LLONG_MAX, 1, LLONG_MAX-1) == -1);
  mt19937_64 rng(1404);
  for (int tc = 0; tc < 20000; ++tc) {
    ll m = 1 + rng()%3000000000LL, a = rng()%m, l = rng()%m;
    ll r = min(m - 1, l + ll(rng()%6)), want = -1;
    for (ll t = l; t <= r; ++t) {
      ll k = singleton(a, m, t);
      if (k >= 0 && (want < 0 || k < want)) want = k;
    }
    assert(mod_min(a, m, l, r) == want);
  }
}
}

namespace mobius {
int n, tot; vector<int> p, mu, flg;
#include "codebook/6_Math/getMu.cpp"
void audit() {
  for (int size : {1, 2, 49, 65536, 257, 65537, 1}) {
    n = size, tot = 0;
    p.assign(n + 2, -777), mu.assign(n + 2, -777);
    flg.assign(n + 2, 0); flg[n + 1] = -777;
    getMu();
    vector<int> squarefree(n + 1, 1), sum(n + 1);
    squarefree[0] = 0;
    for (int d = 2; d*d <= n; ++d)
      for (int j = d*d; j <= n; j += d*d) squarefree[j] = 0;
    for (int d = 1; d <= n; ++d) {
      assert(abs(mu[d]) == squarefree[d]);
      for (int j = d; j <= n; j += d) sum[j] += mu[d];
    }
    assert(sum[1] == 1);
    for (int j = 2; j <= n; ++j) assert(sum[j] == 0);
    assert(mu[0] == -777 && mu[n + 1] == -777);
    assert(flg[n + 1] == -777 && p[tot + 1] == -777);
  }
}
}

namespace xorbasis {
#include "codebook/3_Data_Structure/XOR_Basis.cpp"
using U = unsigned long long;
U image(U x) { return x ^ (x << 1); }
U inverse(U x) {
  for (int s : {1, 2, 4, 8, 16, 32}) x ^= x << s;
  return x;
}
void audit() {
  mt19937_64 rng(1406);
  for (int rank = 0; rank <= 64; ++rank) {
    fill_n(basis, 64, 0);
    U mask = rank == 64 ? U(-1) : (U(1) << rank) - 1;
    vector<U> rows;
    for (int i = 0; i < rank; ++i) rows.push_back(image(U(1) << i));
    shuffle(rows.begin(), rows.end(), rng);
    for (U x : rows) ins(x);
    assert(count_if(begin(basis), end(basis),
                   [](U x) { return x != 0; }) == rank);
    array<U, 64> saved; copy_n(basis, 64, saved.begin());
    for (int tc = 0; tc < 100; ++tc) {
      U x = image(rng() & mask); assert(!(inverse(x) & ~mask));
      ins(x); assert(equal(saved.begin(), saved.end(), basis));
      x = rng(); ins(x);
      int added = bool(inverse(x) & ~mask);
      assert(count_if(begin(basis), end(basis),
                      [](U y) { return y != 0; }) == rank + added);
      if (!added) assert(equal(saved.begin(), saved.end(), basis));
      copy(saved.begin(), saved.end(), basis);
    }
  }
}
}

namespace rectangles {
#include "codebook/3_Data_Structure/Dynamic_2D_Segment_Tree.cpp"
void destroy(Seg::Node *p) {
  if (!p) return;
  destroy(p->l), destroy(p->r); delete p;
}
void push(Seg &s, Seg::Node *p, int l, int r) {
  if (!p) return;
  s.push(p, l, r);
  if (r - l > 1) {
    int m = l + (r - l)/2;
    push(s, p->l, l, m), push(s, p->r, m, r);
  }
}
void push(Seg2D::Node *p, int m) {
  if (!p) return;
  push(p->sum, p->sum.root, 0, m);
  push(p->lazy, p->lazy.root, 0, m);
  push(p->l, m), push(p->r, m);
}
void destroy(Seg2D::Node *p) {
  if (!p) return;
  destroy(p->l), destroy(p->r);
  destroy(p->sum.root), destroy(p->lazy.root); delete p;
}
void audit() {
  Seg2D atom(1, 1);
  atom.update(0, 1, 0, 1, INT_MIN);
  assert(atom.query(0, 1, 0, 1) == INT_MIN);
  atom.update(0, 1, 0, 1, INT_MAX);
  assert(atom.query(0, 1, 0, 1) == -1); destroy(atom.root);
  mt19937 rng(1407);
  for (int n = 0; n <= 6; ++n) for (int m = 0; m <= 6; ++m) {
    Seg2D s(n, m); vector<vector<int>> a(n, vector<int>(m));
    for (int tc = 0; tc < 20; ++tc) {
      int xl = rng()%(n + 1), xr = rng()%(n + 1);
      int yl = rng()%(m + 1), yr = rng()%(m + 1);
      if (xl > xr) swap(xl, xr);
      if (yl > yr) swap(yl, yr);
      int delta = tc%2 ? -1000000 : 1000000;
      if (tc%5 == 0) xl = yl = 0, xr = n, yr = m;
      s.update(xl, xr, yl, yr, delta);
      for (int x = xl; x < xr; ++x) for (int y = yl; y < yr; ++y)
        a[x][y] += delta;
      push(s.root, m); // Distribute every inner lazy, then update again.
      for (int x = 0; x <= n; ++x) for (int xx = x; xx <= n; ++xx)
        for (int y = 0; y <= m; ++y) for (int yy = y; yy <= m; ++yy) {
          int want = 0;
          for (int i = x; i < xx; ++i) for (int j = y; j < yy; ++j)
            want += a[i][j];
          assert(s.query(x, xx, y, yy) == want);
        }
    }
    destroy(s.root);
  }
}
}

namespace gaussian {
#include "codebook/6_Math/Gaussian_Eliminatin.cpp"
void audit() {
  for (double e : {0., 1e-13, -1e-13}) {
    vector<vector<double>> a{{e, 2, 4, -2}, {e, 1, 3, 7}};
    GAS(a);
    vector<vector<double>> want{{e/2, 1, 2, -1}, {e/2, 0, 1, 8}};
    assert(a == want); // Skipped prefix still participates in row ops.
  }
  for (double e : {1e-12, -1e-12, 0.5, -0.5, 1e100, -1e100}) {
    vector<vector<double>> a{{e, 2*e, -e}};
    GAS(a); assert((a == vector<vector<double>>{{1, 2, -1}}));
  }
  vector<vector<double>> a{{0, 0, 1}, {0, 0, -1}};
  auto saved = a; GAS(a); assert(a == saved);
  a.clear(); GAS(a); assert(a.empty());
}
}

namespace flow {
const int N = 12;
const ll INF = 32;
#include "codebook/4_Flow_Matching/MincostMaxflow.cpp"
struct EK {
  struct Edge { int u, v, cap; };
  int n; vector<Edge> e;
  void add(int u, int v, int cap) {
    e.push_back({u, v, cap}), e.push_back({v, u, 0});
  }
  int solve(int s, int t) {
    if (s == t) return 0;
    int flow = 0;
    for (;;) {
      vector<int> prev(n, -1); queue<int> q;
      prev[s] = -2, q.push(s);
      while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int i = 0; i < SZ(e); ++i)
          if (e[i].u == u && e[i].cap && prev[e[i].v] == -1)
            prev[e[i].v] = i, q.push(e[i].v);
      }
      if (prev[t] == -1) return flow;
      int delta = INT_MAX;
      for (int v = t; v != s; v = e[prev[v]].u)
        delta = min(delta, e[prev[v]].cap);
      for (int v = t; v != s; v = e[prev[v]].u)
        e[prev[v]].cap -= delta, e[prev[v]^1].cap += delta;
      flow += delta;
    }
  }
};
void audit() {
  mt19937 rng(1409);
  for (int tc = 0; tc < 500; ++tc) {
    int n = 1 + rng()%8;
    vector<int> h(n); for (int &x : h) x = int(rng()%7)-3;
    MinCostMaxFlow mf; mf.init(0); mf.init(n); EK oracle{n, {}};
    for (int step = 0; step < 12; ++step) {
      // Telescoping costs make every residual cycle exactly zero,
      // including after insertions. Cost follows independently from h.
      for (int j = 0; j < 3; ++j) {
        int u = rng()%n, v = rng()%n, cap = rng()%98;
        mf.add_edge(u, v, cap, h[v] - h[u]); oracle.add(u, v, cap);
      }
      int s = rng()%n, t = rng()%n; ll got, cost;
      mf.solve(s, t, got, cost, step%2);
      int want = oracle.solve(s, t);
      assert(got == want && cost == ll(want)*(h[t] - h[s]));
      mf.solve(s, t, got, cost, !(step%2));
      assert(got == 0 && cost == 0);
    }
  }
}
}

int main() {
  centroid::audit(); mo::audit(); witness::audit(); modmin::audit();
  mobius::audit(); xorbasis::audit(); rectangles::audit();
  gaussian::audit(); flow::audit();
  cout << "PASS round14: nine-template adversarial audit\n";
}
