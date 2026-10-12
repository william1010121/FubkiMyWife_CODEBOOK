// Brute checks for Linear_Recurrence.cpp and ManhattanMST.cpp.
#include "../../codebook/1_Basic/Default_code.cpp"
#define CHECK(c) do { if (!(c)) { \
  cerr << "FAIL line " << __LINE__ << ": " #c "\n"; exit(1); } } while (0)
const int MOD = 998244353;
struct mint { int v; mint(ll x = 0) : v(int((x % MOD + MOD) % MOD)) {}
  mint operator+(mint b) const { return v + b.v; }
  mint operator-(mint b) const { return v - b.v; }
  mint operator*(mint b) const { return ll(v) * b.v; }
  mint operator-() const { return -v; }
  mint &operator+=(mint b) { return *this = *this + b; }
  mint pw(ll e) const { mint r = 1, b = *this;
    for (; e; e >>= 1, b = b * b) if (e & 1) r = r * b; return r; }
  mint operator/(mint b) const { return *this * b.pw(MOD - 2); }
  bool operator==(mint b) const { return v == b.v; }
};
#include "../../codebook/6_Math/Berlekamp-Massey.cpp"
#include "../../codebook/6_Math/Linear_Recurrence.cpp"
#include "../../codebook/9_Else/ManhattanMST.cpp"
mt19937_64 rng(1013);
using Mat = vector<vector<mint>>;
Mat mul(const Mat &A, const Mat &B) { int k = SZ(A); Mat C(k, vector<mint>(k));
  for (int i = 0; i < k; ++i) for (int t = 0; t < k; ++t)
    for (int j = 0; j < k; ++j) C[i][j] += A[i][t] * B[t][j];
  return C; }
int main() {
  for (int tc = 0; tc < 3000; ++tc) { // direct iteration, small n
    int k = rng() % 6; vector<mint> a(k), c(k);
    for (auto &x : a) x = rng(); for (auto &x : c) x = rng();
    vector<mint> s = a;
    for (int i = k; i < 60; ++i) { mint v = 0;
      for (int j = 0; j < k; ++j) v += c[j] * s[i-1-j]; s.pb(v); }
    for (int n = 0; n < 60; ++n)
      CHECK(linear_rec(a, c, n) == (k ? s[n] : mint(0)));
  }
  for (int tc = 0; tc < 200; ++tc) { // matrix power, huge n
    int k = rng() % 8 + 1; vector<mint> a(k), c(k);
    for (auto &x : a) x = rng(); for (auto &x : c) x = rng();
    ll n = rng() >> 2;
    Mat M(k, vector<mint>(k)), R(k, vector<mint>(k));
    for (int j = 0; j < k; ++j) M[0][j] = c[j];
    for (int i = 1; i < k; ++i) M[i][i-1] = 1;
    for (int i = 0; i < k; ++i) R[i][i] = 1;
    ll e = n - (k - 1);
    if (e <= 0) { CHECK(linear_rec(a, c, n) == a[n]); continue; }
    for (Mat B = M; e; e >>= 1, B = mul(B, B)) if (e & 1) R = mul(R, B);
    mint want = 0; // state (a_{k-1},...,a_0)
    for (int j = 0; j < k; ++j) want += R[0][j] * a[k-1-j];
    CHECK(linear_rec(a, c, n) == want);
  }
  { vector<mint> fib = {0, 1, 1, 2, 3, 5, 8};
    auto c = BerlekampMassey(fib);
    CHECK(linear_rec(vector<mint>{0, 1}, c, 90) ==
          mint(2880067194370816120LL % MOD)); }
  { int k = 1000; vector<mint> a(k), c(k); // speed
    for (auto &x : a) x = rng(); for (auto &x : c) x = rng();
    auto t = clock(); linear_rec(a, c, (ll)1e18);
    CHECK(clock() - t < 5 * CLOCKS_PER_SEC); }
  for (int tc = 0; tc < 3000; ++tc) { // Manhattan MST vs Prim
    int n = rng() % 30 + 1, R = tc % 2 ? 5 : 1000000000;
    vector<pll> p(n);
    for (auto &[x, y] : p) x = ll(rng() % (2*R+1)) - R, y = ll(rng() % (2*R+1)) - R;
    auto dist = [&](int i, int j) {
      return llabs(p[i].X-p[j].X) + llabs(p[i].Y-p[j].Y); };
    vector<ll> d(n, LLONG_MAX); vector<int> in(n); d[0] = 0; ll want = 0;
    for (int it = 0; it < n; ++it) { int u = -1;
      for (int v = 0; v < n; ++v) if (!in[v] && (u < 0 || d[v] < d[u])) u = v;
      in[u] = 1, want += d[u];
      for (int v = 0; v < n; ++v) d[v] = min(d[v], dist(u, v)); }
    auto es = manhattan(p); CHECK(SZ(es) <= 4 * n);
    sort(ALL(es)); vector<int> f(n); iota(ALL(f), 0);
    auto find = [&](auto &&self, int x) -> int {
      return f[x] == x ? x : f[x] = self(self, f[x]); };
    ll got = 0;
    for (auto [w, i, j] : es) {
      CHECK(w == dist(i, j));
      int a = find(find, i), b = find(find, j);
      if (a != b) f[a] = b, got += w; }
    CHECK(got == want);
  }
  cout << "kitamasa_manhattan_brute: PASS\n";
}
