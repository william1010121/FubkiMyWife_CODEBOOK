vector<pair<double, double>> CoverSegment(C &a, C &b) {
  double d = (a.c - b.c).abs(); vector<pair<double, double>> res;
  if (same(a.r + b.r, d)) return res;
  if (d <= abs(a.r - b.r) + eps) {
    if (a.r < b.r) res.emplace_back(0, 2 * pi);
  } else if (d < abs(a.r + b.r) - eps) {
    double o = acos((sq(a.r) + sq(d) - sq(b.r)) / (2 * a.r * d)),
      z = (b.c - a.c).angle();
    if (z < 0) z += 2 * pi;
    double l = z - o, r = z + o;
    if (l < 0) l += 2 * pi;
    if (r > 2 * pi) r -= 2 * pi;
    if (l > r) res.emplace_back(l, 2 * pi), res.emplace_back(0, r);
    else res.emplace_back(l, r);
  } return res;
}
// Remove duplicate circles first; O(n^2 log n).
double CircleUnionArea(vector<C> c) {
  double a = 0;
  for (int i = 0; i < (int)c.size(); ++i) {
    double w = 0; vector<pair<double, double>> s = {{2 * pi, 2 * pi}};
    for (int j = 0; j < (int)c.size(); ++j) if (i != j)
      for (auto e : CoverSegment(c[i], c[j])) s.push_back(e);
    sort(s.begin(), s.end());
    auto F = [&](double t) { return c[i].r * (c[i].r * t +
      c[i].c.x * sin(t) - c[i].c.y * cos(t)); };
    for (auto [l, r] : s) {
      if (l > w) a += F(l) - F(w);
      w = max(w, r);
    }
  } return a * 0.5;
}
