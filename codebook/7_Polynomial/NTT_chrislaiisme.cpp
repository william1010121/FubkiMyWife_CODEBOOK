const ll mod = 998244353, g = 3;
// pwr: b>=0, a/ret in [0,mod).
ll pwr(ll a, ll b, ll ret = 1) {
  for (; b; b >>= 1, a = a * a % mod)
    if (b & 1) ret = ret * a % mod;
  return ret;
}
// Direct/padded n=2^k, 0<=k<=23; [0,mod); O(n log n).
void ntt(vector<ll> &vec, bool inv = false) {
  int n = vec.size(); if (n == 1) return;
  vector<int> roots(n / 2, 1);
  ll w = pwr(g, (mod - 1) / n * (inv ? n - 1 : 1));
  for (int j = 1; j < n / 2; ++j)
    roots[j] = roots[j - 1] * w % mod;
  for (int len = n; len > 1; len >>= 1) { // DIF
    for (int i = 0; i < n; i += len)
      for (int j = 0; j < len / 2; ++j) {
        ll a = vec[i + j], b = vec[i + j + len / 2];
        vec[i + j] = a + b < mod ? a + b : a + b - mod;
        vec[i + j + len/2] = (a-b+mod) * roots[j] % mod;
      }
    for (int j = 1; j < len / 4; ++j)
      roots[j] = roots[2 * j];
  }
  ll invn = mod - (mod - 1) / n; // n divides mod-1.
  for (int i = 0, j = 0; i < n; ++i) { // bit reversal
    if (i < j) swap(vec[i], vec[j]);
    if (inv) vec[i] = vec[i] * invn % mod;
    for (int bit = n >> 1; (j ^= bit) < bit; bit >>= 1);
  }
}
void intt(vector<ll> &vec) { ntt(vec, true); }
vector<ll> mult(vector<ll> a, vector<ll> b) {
  if (a.empty() || b.empty()) return {};
  int sz = a.size() + b.size() - 1, n = 1;
  for (; n < sz; n <<= 1);
  a.resize(n), b.resize(n), ntt(a), ntt(b);
  for (int i = 0; i < n; ++i) a[i] = a[i] * b[i] % mod;
  intt(a), a.resize(sz); return a;
}
