// a is convex: a[i+1]-a[i] <= a[i+2]-a[i+1].
// Empty input has the usual empty-convolution result.
template<class T>
vector<T> min_plus_convolution(const vector<T> &a,
  const vector<T> &b) {
  int n = (int)a.size(), m = (int)b.size();
  if (!n || !m) return {};
  vector<T> c(n + m - 1);
  auto dc = [&](auto Y, int l, int r, int jl, int jr) {
    if (l > r) return;
    int mid = (l + r) / 2, from = -1;
    T best{};
    for (int j = jl; j <= jr; ++j)
      if (int i = mid - j; i >= 0 && i < n) {
        T value = a[i] + b[j];
        if (from == -1 || value < best)
          best = value, from = j;
      }
    // Every output index has at least one valid (i,j) pair.  Keep this guard
    // so a malformed recursive range cannot pass -1 onward.
    if (from == -1) return;
    c[mid] = best;
    Y(Y, l, mid - 1, jl, from),
      Y(Y, mid + 1, r, from, jr);
  };
  return dc(dc, 0, n - 1 + m - 1, 0, m - 1), c;
}
