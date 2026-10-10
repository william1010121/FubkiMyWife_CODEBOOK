#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace fft_case {
#include "../../codebook/7_Polynomial/FFT_chrislaiisme.cpp"
}
namespace ntt_case {
#include "../../codebook/7_Polynomial/NTT_chrislaiisme.cpp"
}
namespace fwt_case {
#include "../../codebook/7_Polynomial/Fast_Walsh_Transform.cpp"
}
ll power(ll a, ll e) {
  ll r = 1, mod = ntt_case::mod;
  for (; e; e >>= 1, a = a * a % mod)
    if (e & 1) r = r * a % mod;
  return r;
}
vector<ll> dft(const vector<ll> &a, bool inverse) {
  int n = a.size(); ll mod = ntt_case::mod;
  ll w = power(3, (mod - 1) / n);
  if (inverse) w = power(w, mod - 2);
  vector<ll> b(n);
  for (int k = 0; k < n; ++k) {
    ll wk = power(w, k), t = 1;
    for (ll x : a) b[k] = (b[k] + x * t) % mod, t = t * wk % mod;
    if (inverse) b[k] = b[k] * power(n, mod - 2) % mod;
  }
  return b;
}
int main() {
  mt19937 rng(0x50ff);
  ll mod = ntt_case::mod;
  // Independent DFT checks convention/order even if both directions err.
  for (int n = 1; n <= 64; n *= 2) for (int tc = 0; tc < 20; ++tc) {
    vector<ll> a(n);
    for (auto &x : a) x = tc & 1 ? rng() % mod : mod - 1;
    for (bool inv : {false, true}) {
      auto b = a; ntt_case::ntt(b, inv);
      if (b != dft(a, inv)) abort();
    }
  }
  for (ll x : {0LL, 1LL, mod - 1})
    for (ll e : {0LL, 1LL, 2LL, LLONG_MAX})
      for (ll acc : {0LL, 1LL, mod - 1})
        if (ntt_case::pwr(x, e, acc) != acc * power(x, e) % mod)
          abort();
  for (int n : {1, 2, 3, 7, 8, 9, 31, 32, 33}) {
    vector<ll> a(n, mod - 1), b(n + 1, mod - 1);
    auto c = ntt_case::mult(a, b);
    for (int i = 0; i < int(c.size()); ++i)
      if (c[i] != min({i + 1, n, 2 * n - i})) abort();
    vector<int> x(n), y(n + 1);
    for (int &v : x) v = int(rng() % 2001) - 1000;
    y[0] = 1; y[1] = -1;
    auto z = fft_case::mult(x, y);
    for (int i = 0; i < int(z.size()); ++i) {
      int want = (i < n ? x[i] : 0) -
                 (i && i <= n ? x[i - 1] : 0);
      if (z[i] != want) abort();
    }
  }
  if (!fft_case::mult({}, {}).empty() ||
      !ntt_case::mult({}, {}).empty()) abort();
  // Verify the highest legal root order and inverse-length formula.
  const int limit = 1 << 23;
  vector<ll> large(limit, mod - 1); large[1] = 0;
  ntt_case::ntt(large);
  if (large[0] != mod - limit + 1) abort();
  ll omega = power(3, (mod - 1) / limit), w = 1;
  for (int i = 1; i < limit; ++i) {
    w = w * omega % mod;
    if (large[i] != w) abort();
  }
  ntt_case::intt(large);
  for (int i = 0; i < limit; ++i)
    if (large[i] != (i == 1 ? 0 : mod - 1)) abort();
  for (int L : {0, 6, 1, 8, 2, 7, 0}) {
    int n = 1 << L;
    vector<int> a(n), b(n), want(n);
    for (int &x : a) x = int(rng() % 5) - 2;
    for (int &x : b) x = int(rng() % 5) - 2;
    for (int mask = 0; mask < n; ++mask)
      for (int s = mask;; s = (s - 1) & mask) {
        want[mask] += a[s] * b[mask ^ s];
        if (!s) break;
      }
    auto x = a, y = b;
    fwt_case::subset_convolution(x.data(), y.data(), x.data(), L);
    if (x != want) abort();
    x = a; y = b;
    fwt_case::subset_convolution(x.data(), y.data(), y.data(), L);
    if (y != want) abort();
    fill(want.begin(), want.end(), 0);
    for (int mask = 0; mask < n; ++mask)
      for (int s = mask;; s = (s - 1) & mask) {
        want[mask] += a[s] * a[mask ^ s];
        if (!s) break;
      }
    x = a;
    fwt_case::subset_convolution(x.data(), x.data(), x.data(), L);
    if (x != want) abort();
    // Direct zeta/Mobius oracles, independent of a convolution.
    x = a; y.assign(n, 0);
    for (int mask = 0; mask < n; ++mask)
      for (int s = mask;; s = (s - 1) & mask) {
        y[mask] += a[s]; if (!s) break;
      }
    fwt_case::fwt(x.data(), n, 1);
    if (x != y) abort();
    fwt_case::fwt(x.data(), n, -1);
    if (x != a) abort();
  }
  for (int edge : {INT_MIN + 1, INT_MAX}) {
    // Multiplication by -1 must fit too, not only the final result.
    vector<int> a = {edge, 0}, b = a;
    fwt_case::fwt(a.data(), 2, 1);
    if (a != vector<int>({edge, edge})) abort();
    fwt_case::fwt(a.data(), 2, -1);
    if (a != b) abort();
    int one = 1, out;
    fwt_case::subset_convolution(&edge, &one, &out, 0);
    if (out != edge) abort();
  }
  vector<int> minimum = {0, INT_MIN};
  fwt_case::fwt(minimum.data(), 2, 1);
  fwt_case::fwt(minimum.data(), 2, -1);
  if (minimum != vector<int>({0, INT_MIN})) abort();
  int one = 1, out;
  fwt_case::subset_convolution(&minimum[1], &one, &out, 0);
  if (out != INT_MIN) abort();
  cout << "g08_transform_adversarial: PASS independent NTT DFT, "
          "2^23 boundary, signed FFT differences, FWT aliases/extremes\n";
}
