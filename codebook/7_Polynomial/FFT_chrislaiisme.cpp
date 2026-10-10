using cd = complex<double>;
const double pi = acos(-1);
// n=2^k, 0<=k<=29; O(n log n); finite complex arithmetic.
void fft(vector<cd> &vec, bool inv = false) {
  int n = vec.size(); vector<cd> roots(n / 2);
  for (int i = 0, j = 0; i < n; ++i) { // bit-reversed counter
    if (i < j) swap(vec[i], vec[j]);
    for (int bit = n >> 1; (j ^= bit) < bit; bit >>= 1);
  }
  for (int len = 2; len <= n; len <<= 1) {
    double ang = 2 * pi / len * (inv ? -1 : 1);
    cd wlen(cos(ang), sin(ang)), w = 1;
    for (int j = 0; j < len / 2; ++j) roots[j] = w, w *= wlen;
    for (int i = 0; i < n; i += len)
      for (int j = 0; j < len / 2; ++j) {
        cd a = vec[i + j], b = roots[j] * vec[i + len / 2 + j];
        vec[i + j] = a + b, vec[i + len / 2 + j] = a - b;
      }
  }
  if (inv) for (auto &x : vec) x /= n;
}
void ifft(vector<cd> &vec) { fft(vec, true); }
// Padded n<=2^29; results fit int; error before rounding <0.5.
vector<int> mult(const vector<int> &a, const vector<int> &b) {
  if (a.empty() || b.empty()) return {};
  int sz = a.size() + b.size() - 1, n = 1;
  while (n < sz) n <<= 1;
  vector<cd> ca(n), cb(n); copy(a.begin(), a.end(), ca.begin());
  copy(b.begin(), b.end(), cb.begin());
  fft(ca); if (&a == &b) cb = ca; else fft(cb);
  for (int i = 0; i < n; ++i) ca[i] *= cb[i];
  ifft(ca); vector<int> res(sz);
  for (int i = 0; i < sz; ++i) res[i] = round(ca[i].real());
  return res;
}
