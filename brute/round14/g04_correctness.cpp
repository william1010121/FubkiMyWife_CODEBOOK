#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
#define X first
#define Y second
#define SZ(a) int((a).size())
#define ALL(a) (a).begin(), (a).end()
#define pb push_back
#include "../../codebook/3_Data_Structure/IntervalContainer.cpp"
#include "../../codebook/9_Else/simulated_annealing.cpp"
namespace crt {
#include "../../codebook/6_Math/ax+by=gcd.cpp"
#include "../../codebook/6_Math/chineseRemainder.cpp"
}
namespace ranks {
constexpr int N = 4097;
#include "../../codebook/3_Data_Structure/BIT_kth.cpp"
}
namespace clique {
constexpr int N = 65;
#include "../../codebook/2_Graph/Maximum_Clique_Dyn.cpp"
}
struct Point {
  double x, y;
  Point(double x = 0, double y = 0) : x(x), y(y) {}
  Point operator+(Point p) const { return {x + p.x, y + p.y}; }
  Point operator-(Point p) const { return {x - p.x, y - p.y}; }
  Point operator*(double k) const { return {x * k, y * k}; }
  Point operator/(double k) const { return {x / k, y / k}; }
};
double Cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }
double len(Point a) { return hypot(a.x, a.y); }
#include "../../codebook/8_Geometry/triangleCenter.cpp"
#include "../../codebook/5_String/Smallest_Rotation.cpp"
#include "../../codebook/9_Else/All_LCS.cpp"
#include "../../codebook/6_Math/PiCount.cpp"
#include "../../codebook/9_Else/DynamicConvexTrick.cpp"

void intervals() {
  vector<int> xs{INT_MIN, INT_MIN + 1, -1000000, -3, -2, -1,
                 0, 1, 2, 3, 1000000, INT_MAX - 1, INT_MAX};
  vector<bool> covered(xs.size() - 1);
  set<pii> actual;
  mt19937 rng(14041);
  for (int step = 0; step < 30000; ++step) {
    int l = rng() % xs.size(), r = rng() % xs.size();
    if (l > r) swap(l, r);
    bool add = rng() % 2;
    if (add) {
      auto it = addInterval(actual, xs[l], xs[r]);
      if (l == r) assert(it == actual.end());
      else assert(it != actual.end() && it->X <= xs[l] &&
                  it->Y >= xs[r]);
    } else removeInterval(actual, xs[l], xs[r]);
    for (int i = l; i < r; ++i) covered[i] = add;
    set<pii> want;
    for (int i = 0; i < SZ(covered);) {
      if (!covered[i]) { ++i; continue; }
      int start = i;
      while (i < SZ(covered) && covered[i]) ++i;
      want.emplace(xs[start], xs[i]);
    }
    assert(actual == want);
  }
}

void fenwick() {
  using namespace ranks;
  vector<int> a(N + 1); a[1] = INT_MAX;
  fill_n(bit, N + 1, 0);
  auto update = [](int i, int d) {
    for (; i <= N; i += i & -i) bit[i] += d;
  };
  update(1, INT_MAX);
  mt19937 rng(14042);
  for (int step = 0; step < 20000; ++step) {
    int from = 1 + rng() % N, to = 1 + rng() % N;
    if (step % 100 == 0) from = 1;
    int d = a[from] ? 1 + rng() % a[from] : 0;
    a[from] -= d; update(from, -d);
    a[to] += d; update(to, d);
    for (int k : {1, INT_MAX, int(1 + rng() % INT_MAX)}) {
      ll sum = 0; int want = 0;
      do { sum += a[++want]; } while (sum < k);
      assert(query_kth(k) == want);
    }
  }
}

void remainders() {
  mt19937_64 rng(14043);
  for (int tc = 0; tc < 20000; ++tc) {
    ll x = (ll)rng(), y = (ll)rng();
    ll a = 1 + rng() % 80, b = 1 + rng() % 80;
    auto mod = [](ll x, ll m) { return (x % m + m) % m; };
    ll want = -1, period = lcm(a, b);
    for (ll r = mod(x, a); r < period; r += a)
      if (r % b == mod(y, b)) { want = r; break; }
    assert(crt::solve(x, a, y, b) == want);
    assert(crt::solve(y, b, x, a) == want);
  }
  // Dividing moduli and full-width residues, with maximal output period.
  for (ll x : {LLONG_MIN, -1LL, 0LL, LLONG_MAX})
    for (ll m : {1LL, 7LL, LLONG_MAX}) {
      ll got = crt::solve(x, LLONG_MAX, x, m);
      ll want = x % LLONG_MAX;
      if (want < 0) want += LLONG_MAX;
      assert(got == want);
    }
}

int clique_oracle(const vector<unsigned> &g) {
  int n = SZ(g), best = 0;
  vector<bool> good(1 << n); good[0] = true;
  for (unsigned s = 1; s < good.size(); ++s) {
    int u = countr_zero(s); unsigned rest = s & (s - 1);
    good[s] = good[rest] && (g[u] & rest) == rest;
    if (good[s]) best = max(best, popcount(s));
  }
  return best;
}
void cliques() {
  clique::MaxClique mc;
  mt19937 rng(14044);
  for (int tc = 0; tc < 1800; ++tc) {
    int n = rng() % 15; mc.init(n);
    vector<unsigned> g(n);
    for (int phase = 0; phase < 3; ++phase) {
      for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
        if (rng() % 4 == 0) {
          mc.add_edge(u, v); mc.add_edge(v, u);
          g[u] |= 1U << v; g[v] |= 1U << u;
        }
      for (auto &row : mc.cs) row.set(); // Poison shared colour scratch.
      fill_n(mc.sol, clique::N, -1);
      int want = clique_oracle(g), got = mc.solve();
      assert(got == want && mc.solve() == want);
      set<int> seen;
      for (int i = 0; i < got; ++i) {
        int u = mc.sol[i];
        assert(0 <= u && u < n && seen.insert(u).second);
        for (int j = 0; j < i; ++j) assert(g[u] >> mc.sol[j] & 1U);
      }
    }
  }
}

void centers() {
  mt19937 rng(14045);
  for (int tc = 0; tc < 5000; ++tc) {
    array<Point, 3> p;
    for (auto &v : p) v = {double(int(rng() % 101) - 50),
                           double(int(rng() % 101) - 50)};
    if (Cross(p[1] - p[0], p[2] - p[0]) == 0) continue;
    // Exact power-of-two transforms preserve these integer coordinates.
    double scale = ldexp(1., int(rng() % 21) - 10);
    Point offset{1048576 * scale, -1048576 * scale};
    array<int, 3> order{0, 1, 2};
    Point reference = TriangleCircumCenter(p[0], p[1], p[2]);
    do {
      Point a = offset + p[order[0]] * scale;
      Point b = offset + p[order[1]] * scale;
      Point c = offset + p[order[2]] * scale;
      Point o = TriangleCircumCenter(a, b, c);
      assert(len((o - offset) / scale - reference) < 1e-7);
      Point h = TriangleOrthoCenter(a, b, c);
      Point mass = TriangleMassCenter(a, b, c);
      assert(len((mass - offset) / scale -
                 (p[0] + p[1] + p[2]) / 3) < 1e-7);
      auto dot = [](Point u, Point v) { return u.x*v.x + u.y*v.y; };
      assert(abs(dot((h-a)/scale, (b-c)/scale)) < 1e-4);
      Point in = (TriangleInnerCenter(a, b, c) - offset) / scale;
      auto distance = [&](Point u, Point v) {
        return abs(Cross(v - u, in - u)) / len(v - u);
      };
      double d = distance(p[0], p[1]);
      assert(abs(d - distance(p[1], p[2])) < 1e-7);
      assert(abs(d - distance(p[2], p[0])) < 1e-7);
    } while (next_permutation(ALL(order)));
  }
}

string rotation_oracle(string s) {
  string best = s;
  for (int i = 1; i < SZ(s); ++i)
    best = min(best, s.substr(i) + s.substr(0, i));
  return best;
}
set<string> subsequences(const string &s) {
  set<string> out;
  for (int mask = 0; mask < (1 << SZ(s)); ++mask) {
    string t;
    for (int i = 0; i < SZ(s); ++i) if (mask >> i & 1) t += s[i];
    out.insert(t);
  }
  return out;
}
void strings() {
  mt19937 rng(14046);
  assert(mcp("").empty());
  for (int tc = 0; tc < 1200; ++tc) {
    string s(rng() % 12, '\0'), t(rng() % 12, '\0');
    for (char &c : s) c = char((rng() % 4) * 85);
    for (char &c : t) c = char((rng() % 4) * 85);
    assert(mcp(s) == rotation_oracle(s));
    string repeated;
    for (int k = 0; k < 7; ++k) repeated += s;
    string want_rotation;
    for (int k = 0; k < 7; ++k) want_rotation += rotation_oracle(s);
    assert(mcp(repeated) == want_rotation);
    auto a = subsequences(s), b = subsequences(t);
    vector<string> common, want; size_t longest = 0;
    set_intersection(ALL(a), ALL(b), back_inserter(common));
    for (auto &u : common) longest = max(longest, u.size());
    for (auto &u : common) if (u.size() == longest) want.pb(u);
    assert(all_lcs(s, t) == want && all_lcs(t, s) == want);
  }
  string s(200000, char(255)); s.back() = 0;
  assert(mcp(s) == string(1, '\0') + string(199999, char(255)));
}

void prime_windows() {
  const int limit = 3162280;
  vector<bool> composite(limit + 1); vector<int> primes;
  for (int p = 2; p <= limit; ++p) if (!composite[p]) {
    primes.pb(p);
    if (ll(p) * p <= limit)
      for (int k = p*p; k <= limit; k += p) composite[k] = true;
  }
  // Independent segmented counts across square and fourth-power cutoffs.
  for (ll center : {1000000LL, 97LL*97*97*97, 997LL*997*997*997,
                    3162277LL*3162277}) {
    ll l = center - 1000, r = center + 1000;
    vector<bool> marked(r-l+1);
    for (ll p : primes) {
      if (p*p > r) break;
      for (ll k = max(p*p, (l+p-1)/p*p); k <= r; k += p)
        marked[k-l] = true;
    }
    ll base = PrimeCount(l-1), count = 0;
    for (ll x = l; x <= r; ++x) {
      count += !marked[x-l];
      if (x == center || x == r) assert(PrimeCount(x) == base + count);
    }
  }
  assert(PrimeCount(LLONG_MIN) == 0 && PrimeCount(1) == 0);
}

void hulls() {
  mt19937_64 rng(14047);
  for (int tc = 0; tc < 300; ++tc) {
    DynamicHull hull; vector<pll> lines;
    for (int step = 0; step < 100; ++step) {
      ll a = int(rng() % 7) - 3, b = ll(rng() % 2000001) - 1000000;
      hull.addline(a, b); lines.pb({a, b});
      for (ll x : {-DynamicHull::kInf + 1, -1LL, 0LL, 1LL,
                   DynamicHull::kInf, ll(rng() % 2000001) - 1000000}) {
        __int128 best = -((__int128)1 << 126);
        for (auto [s, t] : lines) best = max(best, (__int128)s*x+t);
        assert(hull.query(x) == best);
      }
    }
  }
  // Crossings outside both finite sentinels, with safe ll arithmetic.
  vector<pll> lines{{0, -2000000000000000000LL}, {1, 0},
                    {0, 2000000000000000000LL}, {-1, 0}};
  sort(ALL(lines));
  do {
    DynamicHull hull;
    for (auto [a,b] : lines) hull.addline(a,b);
    for (ll x : {-DynamicHull::kInf+1, 0LL, DynamicHull::kInf})
      assert(hull.query(x) == 2000000000000000000LL);
  } while (next_permutation(ALL(lines)));
}

struct CopyOnly {
  int x;
  CopyOnly(int x) : x(x) {}
  CopyOnly(const CopyOnly &) = default;
  CopyOnly &operator=(const CopyOnly &) = default;
  CopyOnly(CopyOnly &&) = delete;
  CopyOnly &operator=(CopyOnly &&) = delete;
};
void annealing() {
  for (int iterations : {-7, 0}) {
    mt19937_64 rng(14048), saved = rng;
    auto got = simulated_annealing(CopyOnly(50),
      [](const CopyOnly &s) { return double(s.x); },
      [](const CopyOnly &, auto &) -> CopyOnly {
        abort();
      }, rng, iterations);
    assert(got.x == 50 && rng == saved);
  }
  for (double temperature : {0., 1e-15, 3., 1e6}) {
    mt19937_64 rng(14048), expected_rng = rng;
    vector<int> visits;
    auto next = [&](const CopyOnly &s, auto &r) {
      visits.pb(s.x); return CopyOnly(int(r() % 201) - 100);
    };
    auto score = [](const CopyOnly &s) { return double(abs(s.x)); };
    auto got = simulated_annealing(CopyOnly(50), score, next, rng,
                                  500, temperature, 0.97);
    int current = 50, best = 50;
    uniform_real_distribution<double> unit(0, 1);
    for (int i = 0; i < 500; ++i) {
      assert(visits[i] == current);
      int candidate = int(expected_rng() % 201) - 100;
      long double delta = abs(current) - abs(candidate);
      if (delta > 0 || expl(delta / max(temperature, 1e-15))
          >= unit(expected_rng)) current = candidate;
      if (abs(current) < abs(best)) best = current;
      temperature *= 0.97;
    }
    assert(got.x == best && rng == expected_rng);
  }
}

int main() {
  auto check = [](auto f, const char *name) {
    f(); cout << "PASS " << name << '\n' << flush;
  };
  check(intervals, "interval sequences");
  check(fenwick, "Fenwick transfers");
  check(remainders, "CRT residues");
  check(cliques, "clique reuse");
  check(centers, "triangle permutations and transforms");
  check(strings, "rotation and all LCS");
  check(prime_windows, "segmented prime counts");
  check(hulls, "online maximum hull");
  check(annealing, "copy-only annealing and RNG replay");
  cout << "PASS round14 g04: all ten templates\n";
}
