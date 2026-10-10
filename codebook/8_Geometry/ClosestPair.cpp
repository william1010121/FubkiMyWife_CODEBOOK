double closest_pair(int l, int r) {
  // Inclusive [l, r], sorted by x; p is never modified.
  // O(n log n) time, O(n) space. Fewer than 2 points: 1e9.
  if (l >= r) return 1e9;
  int n = r - l + 1;
  vector<int> ord(n), tmp(n);
  iota(ord.begin(), ord.end(), l);
  auto by_y = [&](int a, int b) {
    return p[a].y < p[b].y;
  };
  auto solve = [&](auto &&self, int lo, int hi) -> double {
    if (hi - lo <= 3) {
      double d = numeric_limits<double>::infinity();
      for (int i = lo; i < hi; ++i)
        for (int j = i + 1; j < hi; ++j)
          d = min(d, dist(p[ord[i]], p[ord[j]]));
      sort(ord.begin() + lo, ord.begin() + hi, by_y);
      return d;
    }
    int mid = (lo + hi) / 2;
    double x = p[ord[mid]].x;
    double d = min(self(self, lo, mid), self(self, mid, hi));
    merge(ord.begin() + lo, ord.begin() + mid,
      ord.begin() + mid, ord.begin() + hi,
      tmp.begin() + lo, by_y);
    copy(tmp.begin() + lo, tmp.begin() + hi, ord.begin() + lo);
    int sz = 0;
    for (int i = lo; i < hi; ++i)
      if (fabs(p[ord[i]].x - x) < d) tmp[lo + sz++] = ord[i];
    for (int i = 0; i < sz; ++i)
      for (int j = i + 1; j < sz &&
          p[tmp[lo + j]].y - p[tmp[lo + i]].y < d; ++j)
        d = min(d, dist(p[tmp[lo + i]], p[tmp[lo + j]]));
    return d;
  };
  return solve(solve, 0, n);
}
