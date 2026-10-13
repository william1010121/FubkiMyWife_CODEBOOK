#include "../codebook/1_Basic/Default_code.cpp"
#include "../codebook/8_Geometry/PointInConvex.cpp"
namespace recurrence {
#include "../codebook/6_Math/Berlekamp-Massey.cpp"
}
namespace strings {
const int MAXn = 2048;
#include "../codebook/5_String/Z-value.cpp"
void check(const string &s, const string &t) {
  fill_n(z, MAXn, -123);
  make_z(s);
  assert(z[0] == -123);
  for (int i = 1; i < SZ(s); ++i) {
    int k = 0;
    while (i + k < SZ(s) && s[k] == s[i + k]) ++k;
    assert(z[i] == k);
  }
  for (int i = max(1, SZ(s)); i < MAXn; ++i) assert(z[i] == -123);
  ostringstream out;
  {
#define cout out
#include "../codebook/5_String/KMP.cpp"
#undef cout
    for (int i = 0; i < SZ(t); ++i) {
      int k = 0;
      for (int j = 1; j <= i; ++j)
        if (t.compare(0, j, t, i + 1 - j, j) == 0) k = j;
      assert(nxt[i] == k);
    }
  }
  vector<int> want, got;
  for (int i = 0; i + SZ(t) <= SZ(s); ++i)
    if (s.compare(i, t.size(), t) == 0) want.pb(i + 1);
  istringstream in(out.str());
  for (int i; in >> i;) got.pb(i);
  assert(got == want);
}
}
namespace rho {
ll mul(ll a, ll b, ll n) {
  assert(0 <= a && a < n && 0 <= b && b < n);
  return (__int128)a * b % n;
}
#include "../codebook/6_Math/Miller_Rabin.cpp"
bool prime(ll n) {
  for (ll a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022})
    if (!Miller_Rabin(a, n)) return false;
  return true;
}
#include "../codebook/6_Math/Pollard_Rho.cpp"
void check(ll n, const map<ll, int> &want) {
  __int128 product = 1;
  for (auto [p, e] : want) {
    // Independently certify the oracle factors by trial division.
    for (ll d = 2; d <= p / d; ++d) assert(p % d);
    for (int i = 0; i < e; ++i) product *= p;
  }
  assert(product == n);
  cnt.clear(); PollardRho(n); assert(cnt == want);
  PollardRho(1); assert(cnt == want);
}
}
struct F3 {
  int v;
  F3(int x = 0) : v((x % 3 + 3) % 3) {}
  F3 operator-() const { return -v; }
  F3 &operator+=(F3 b) { v = (v + b.v) % 3; return *this; }
  friend F3 operator*(F3 a, F3 b) { return a.v * b.v; }
  friend F3 operator/(F3 a, F3 b) {
    assert(b.v); return a * b; // Both nonzero F3 values invert themselves.
  }
  friend bool operator==(F3 a, F3 b) { return a.v == b.v; }
};
// Feasibility of each candidate order via independent F3 elimination.
bool possible(const vector<F3> &s, int k) {
  vector<vector<int>> a(SZ(s) - k, vector<int>(k + 1));
  for (int i = k; i < SZ(s); ++i) {
    for (int j = 0; j < k; ++j) a[i - k][j] = s[i - j - 1].v;
    a[i - k][k] = s[i].v;
  }
  int r = 0;
  for (int j = 0; j < k && r < SZ(a); ++j) {
    int p = r;
    while (p < SZ(a) && !a[p][j]) ++p;
    if (p == SZ(a)) continue;
    swap(a[p], a[r]); int inv = a[r][j];
    for (int c = j; c <= k; ++c) a[r][c] = a[r][c] * inv % 3;
    for (int i = r + 1; i < SZ(a); ++i) {
      int v = a[i][j];
      for (int c = j; c <= k; ++c)
        a[i][c] = (a[i][c] - v * a[r][c] % 3 + 3) % 3;
    }
    ++r;
  }
  for (int i = r; i < SZ(a); ++i) if (a[i][k]) return false;
  return true;
}
void bm_audit() {
  int count = 0;
  for (int n = 0, total = 1; n <= 8; ++n, total *= 3)
    for (int code = 0; code < total; ++code) {
      vector<F3> s(n); int z = code;
      for (auto &v : s) v = z % 3, z /= 3;
      auto c = recurrence::BerlekampMassey(s);
      int order = 0;
      while (!possible(s, order)) ++order;
      assert(SZ(c) == order);
      for (int i = order; i < n; ++i) {
        F3 v;
        for (int j = 0; j < order; ++j) v += c[j] * s[i - j - 1];
        assert(v == s[i]);
      }
      ++count;
    }
  cout << "PASS BM: " << count << " F3 sequences, minimal order\n";
}
__int128 turn(IPoint a, IPoint b, IPoint c) {
  return (__int128)(b.x - a.x) * (c.y - a.y) -
         (__int128)(b.y - a.y) * (c.x - a.x);
}
vector<IPoint> hull(vector<IPoint> p) {
  sort(ALL(p), [](IPoint a, IPoint b) {
    return tie(a.x, a.y) < tie(b.x, b.y);
  });
  if (SZ(p) <= 1) return p;
  vector<IPoint> h;
  for (auto q : p) {
    while (SZ(h) > 1 && turn(h[SZ(h)-2], h.back(), q) <= 0)
      h.pop_back();
    h.pb(q);
  }
  int k = SZ(h);
  for (int i = SZ(p) - 2; i >= 0; --i) {
    while (SZ(h) > k && turn(h[SZ(h)-2], h.back(), p[i]) <= 0)
      h.pop_back();
    h.pb(p[i]);
  }
  h.pop_back(); return h;
}
int location(const vector<IPoint> &h, IPoint p) {
  if (h.empty()) return 0;
  if (SZ(h) == 1) return h[0].x == p.x && h[0].y == p.y;
  if (SZ(h) == 2)
    return turn(h[0], h[1], p) == 0 &&
      min(h[0].x,h[1].x) <= p.x && p.x <= max(h[0].x,h[1].x) &&
      min(h[0].y,h[1].y) <= p.y && p.y <= max(h[0].y,h[1].y);
  bool edge = false;
  for (int i = 0; i < SZ(h); ++i) {
    auto t = turn(h[i], h[(i + 1) % SZ(h)], p);
    if (t < 0) return 0;
    edge |= t == 0;
  }
  return edge ? 1 : 2;
}
void convex_audit() {
  const ll B = 1000000000000000000LL;
  for (int mask = 0; mask < 512; ++mask) {
    vector<IPoint> points;
    for (int i = 0; i < 9; ++i)
      if (mask >> i & 1) points.pb({i / 3 - 1, i % 3 - 1});
    auto base = hull(points);
    for (ll scale : {1LL, 1000000000LL}) for (ll offset : {0LL,B-10}) {
      // Translations stay within the documented bound.
      if (offset && scale > 1) continue;
      auto h = base;
      for (auto &p : h) p.x = p.x * scale + offset,
                         p.y = p.y * scale - offset;
      for (int k = 0; k < max(1, SZ(h)); ++k) {
        for (ll x = -3; x <= 3; ++x) for (ll y = -3; y <= 3; ++y) {
          IPoint p{x * scale + offset, y * scale - offset};
          assert(pointInConvex(h, p) == location(h, p));
        }
        if (!h.empty()) rotate(h.begin(), h.begin() + 1, h.end());
      }
    }
  }
  cout << "PASS PointInConvex: every 3x3 subset, rotations/translations\n";
}
int main() {
  bm_audit(); convex_audit();
  rho::check(LLONG_MAX, {{7,2},{73,1},{127,1},{337,1},
                        {92737,1},{649657,1}});
  for (int e = 0; e <= 62; ++e) {
    map<ll,int> want; if (e) want[2] = e;
    rho::check(1LL << e, want);
    if (e < 62) { want[3] = 1; rho::check(3LL << e, want); }
  }
  for (ll a = 1, k = 0; k <= 20; ++k, a *= 3)
    for (ll b = 1, l = 0; l <= 20 && a <= LLONG_MAX / b; ++l,b *= 5) {
      map<ll,int> want; if (k) want[3] = k; if (l) want[5] = l;
      rho::check(a * b, want);
    }
  cout << "PASS PollardRho: LLONG_MAX, powers of 2, mixed prime powers\n";
  mt19937 rng(1403);
  for (int tc = 0; tc < 600; ++tc) {
    int n = tc % 3 == 0 ? 2048 : tc % 3 == 1 ? 1 : rng() % 2049;
    string s(n, '\0');
    for (char &c : s) c = char(rng() % (tc % 2 ? 256 : 3));
    if (tc % 7 == 0) fill(ALL(s), char(tc));
    int at = rng() % (n + 1);
    string t = s.substr(at, rng() % 129);
    if (tc % 5 == 0) t += char(255);
    strings::check(s, t);
  }
  strings::check("", ""); strings::check("", string(512, '\0'));
  cout << "PASS KMP/Z: poisoned capacity reuse, all-byte matches\n";
}
