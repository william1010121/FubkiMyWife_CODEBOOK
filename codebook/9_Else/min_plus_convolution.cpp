// Convex a: a[i+1]-a[i] <= a[i+2]-a[i+1]; b arbitrary.
// O((n+m) log(n+m)); empty input -> empty output; sums must fit T.
template<class T>
vector<T> min_plus_convolution(const vector<T> &a, const vector<T> &b) {
  int n = (int)a.size(), m = (int)b.size();
  if (!n || !m) return {};
  vector<T> c(n + m - 1);
  auto dc = [&](auto &&self, int l, int r, int jl, int jr) -> void {
    if (l > r) return;
    int mid = (l + r) / 2, from = max(jl, mid - n + 1);
    T best = a[mid - from] + b[from];
    for (int j = from + 1; j <= min(jr, mid); ++j)
      if (T v = a[mid - j] + b[j]; v < best) best = v, from = j;
    c[mid] = best; // Leftmost argmin is monotone.
    self(self, l, mid - 1, jl, from);
    self(self, mid + 1, r, from, jr);
  };
  return dc(dc, 0, n + m - 2, 0, m - 1), c;
}
