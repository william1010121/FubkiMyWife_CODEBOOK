// Brute-force checks for Euler_Path.cpp and TSP.cpp.
#include "../../codebook/1_Basic/Default_code.cpp"
const ll INF = 1e18;
#include "../../codebook/2_Graph/Euler_Path.cpp"
#include "../../codebook/2_Graph/TSP.cpp"
#define CHECK(c) do { if (!(c)) { \
  cerr << "FAIL line " << __LINE__ << ": " #c "\n"; exit(1); } } while (0)
mt19937 rng(20261012);

// Exhaustive: does an Euler walk from s exist?
bool exists(int n, bool dir, const vector<pii> &es, int s) {
  int m = SZ(es); vector<int> used(m);
  auto go = [&](auto &&self, int u, int k) -> bool {
    if (k == m) return true;
    for (int i = 0; i < m; ++i) if (!used[i]) {
      auto [a, b] = es[i]; int v = -1;
      if (a == u) v = b; else if (!dir && b == u) v = a;
      if (v < 0) continue;
      used[i] = 1; bool ok = self(self, v, k + 1); used[i] = 0;
      if (ok) return true;
    }
    return false;
  };
  return go(go, s, 0);
}
bool valid(int n, bool dir, const vector<pii> &es, int s,
           const vector<int> &p) {
  int m = SZ(es); if (SZ(p) != m + 1 || p[0] != s) return false;
  multiset<pii> left;
  for (auto [a, b] : es)
    left.insert(dir ? pii(a, b) : pii(min(a, b), max(a, b)));
  for (int i = 0; i < m; ++i) {
    int a = p[i], b = p[i + 1];
    pii k = dir ? pii(a, b) : pii(min(a, b), max(a, b));
    auto it = left.find(k); if (it == left.end()) return false;
    left.erase(it);
  }
  return true;
}
int main() {
  for (int tc = 0; tc < 30000; ++tc) {
    int n = rng() % 5 + 1, m = rng() % 7; bool dir = rng() % 2;
    vector<pii> es(m);
    for (auto &[a, b] : es) a = rng() % n, b = rng() % n;
    Euler e(n, dir); for (auto [a, b] : es) e.add_edge(a, b);
    int s = rng() % n; auto p = e.solve(s);
    bool ex = exists(n, dir, es, s);
    CHECK(ex == !p.empty());
    if (ex) CHECK(valid(n, dir, es, s, p));
  }
  for (int tc = 0; tc < 3000; ++tc) {
    int n = rng() % 7 + 1; bool cyc = rng() % 2;
    vector<vector<ll>> d(n, vector<ll>(n));
    for (auto &r : d) for (ll &x : r)
      x = rng() % 5 ? ll(rng() % 100) : INF;
    auto [c, ord] = tsp(d, cyc);
    vector<int> p(n); iota(ALL(p), 0); ll best = INF;
    do {
      if (cyc && p[0]) continue;
      ll s = 0;
      for (int i = 0; i + 1 < n; ++i) s = min(INF, s + d[p[i]][p[i+1]]);
      if (cyc && n > 1) s = min(INF, s + d[p[n-1]][p[0]]);
      best = min(best, s);
    } while (next_permutation(ALL(p)));
    CHECK(c == best);
    if (c < INF) {
      CHECK(SZ(ord) == n && (!cyc || ord[0] == 0));
      vector<int> q = ord; sort(ALL(q));
      for (int i = 0; i < n; ++i) CHECK(q[i] == i);
      ll s = 0;
      for (int i = 0; i + 1 < n; ++i) s += d[ord[i]][ord[i+1]];
      if (cyc && n > 1) s += d[ord[n-1]][ord[0]];
      CHECK(s == c);
    } else CHECK(ord.empty());
  }
  { int n = 18; vector<vector<ll>> d(n, vector<ll>(n));
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
      d[i][j] = abs(i - j);
    CHECK(tsp(d).X == 2 * (n - 1) && tsp(d, false).X == n - 1); }
  { Euler e(200000, false); // long cycle, iterative
    for (int i = 0; i < 200000; ++i) e.add_edge(i, (i + 1) % 200000);
    CHECK(SZ(e.solve(0)) == 200001); }
  cout << "euler_tsp_brute: PASS\n";
}
