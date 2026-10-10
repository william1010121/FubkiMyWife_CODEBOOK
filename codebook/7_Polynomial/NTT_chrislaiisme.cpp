const ll mod = 998244353, g = 3;
ll pwr(ll a, ll b, ll ret = 1) {
  for (; b; b >>= 1, a = a * a % mod) if (b & 1) ret = ret * a % mod;
  return ret;
}
// Residues in [0,mod); length 2^k, k<=23; O(n log n).
void ntt(vector<ll> &vec, bool inv = false) {
  int n = vec.size();
  for (int i = 0, j = 0; i < n; ++i) { // bit-reversed counter
    if (i < j) swap(vec[i], vec[j]);
    for (int bit = n >> 1; (j ^= bit) < bit; bit >>= 1);
  }
  for (int len = 2; len <= n; len <<= 1) {
    ll wlen = pwr(g, (mod - 1) / len * (inv ? len - 1 : 1));
    for (int i = 0; i < n; i += len) {
      ll w = 1;
      for (int j = 0; j < len / 2; ++j) {
        ll a = vec[i + j], b = w * vec[i + len / 2 + j] % mod;
        vec[i + j] = a + b < mod ? a + b : a + b - mod;
        vec[i + len / 2 + j] = a - b >= 0 ? a - b : a - b + mod;
        w = w * wlen % mod;
      }
    }
  }
  if (inv) {
    ll invn = pwr(n, mod - 2);
    for (auto &x : vec) x = x * invn % mod;
  }
}
void intt(vector<ll> &vec) { ntt(vec, true); }
vector<ll> mult(vector<ll> a, vector<ll> b) {
  if (a.empty() || b.empty()) return {};
  int sz = a.size() + b.size() - 1, n = 1;
  while (n < sz) n <<= 1;
  a.resize(n), b.resize(n), ntt(a), ntt(b);
  for (int i = 0; i < n; ++i) a[i] = a[i] * b[i] % mod;
  intt(a), a.resize(sz); return a;
}
