// a_n, a_i=sum_j c[j]a[i-1-j] (BM output); a: first k
// terms; n>=0. T: 0/1 init, +=, *. O(k^2 log n).
template<class T> T linear_rec(const vector<T> &a,
                               const vector<T> &c, ll n) {
  int k = SZ(c); if (!k) return T(0);
  auto mul = [&](const vector<T> &x, const vector<T> &y) {
    vector<T> r(2 * k + 1);
    for (int i = 0; i <= k; ++i)
      for (int j = 0; j <= k; ++j) r[i + j] += x[i] * y[j];
    for (int i = 2 * k; i > k; --i)
      for (int j = 1; j <= k; ++j) r[i-j] += r[i] * c[j-1];
    r.resize(k + 1); return r; };
  vector<T> p(k + 1), e(p); p[0] = e[1] = 1;
  for (++n; n; n /= 2, e = mul(e, e))
    if (n & 1) p = mul(p, e);
  T res = 0;
  for (int i = 0; i < k; ++i) res += p[i + 1] * a[i];
  return res; }
