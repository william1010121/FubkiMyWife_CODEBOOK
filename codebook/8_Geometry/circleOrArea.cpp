void AppendCoverSegment(C &a, C &b, vector<pair<double, double>> &res) {
  if (max(abs(a.c.x - b.c.x), abs(a.c.y - b.c.y)) >
      a.r + b.r + eps) return;
  double d = (a.c - b.c).abs(); if (same(a.r + b.r, d)) return;
  if (d <= abs(a.r - b.r) + eps) {
    if (a.r < b.r) res.emplace_back(0, 2 * pi);
  } else if (d < abs(a.r + b.r) - eps) {
    double o = acos((sq(a.r) + sq(d) - sq(b.r)) / (2 * a.r * d)),
      z = (b.c - a.c).angle();
    if (z < 0) z += 2 * pi; double l = z - o, r = z + o;
    if (l < 0) l += 2 * pi; if (r > 2 * pi) r -= 2 * pi;
    if (l > r) res.emplace_back(l, 2 * pi), res.emplace_back(0, r);
    else res.emplace_back(l, r);
  }
}
vector<pair<double, double>> CoverSegment(C &a, C &b) {
  vector<pair<double, double>> v; AppendCoverSegment(a, b, v); return v; }
// r>=0; no eps-coincident pairs (d, |dr| <= eps); O(n^2 log n).
// Floating intermediates finite; geometry resolved to required tolerance.
double CircleUnionArea(vector<C> c) {
  if (!c.empty()) { P o = c[0].c; for (auto &v : c) v.c = v.c - o; }
  sort(c.begin(), c.end(), [](C a, C b) { return a.r > b.r; });
  double a = 0; vector<pair<double, double>> s;
  for (int i = 0; i < (int)c.size(); ++i) {
    double w = 0; s = {{2 * pi, 2 * pi}};
    for (int j = 0; j < (int)c.size(); ++j) if (i != j) {
      AppendCoverSegment(c[i], c[j], s);
      if (s.back() == make_pair(0., 2 * pi)) { s.clear(); break; }
    }
    sort(s.begin(), s.end());
    auto F = [&](double t) { return c[i].r * (c[i].r * t +
      c[i].c.x * sin(t) - c[i].c.y * cos(t)); };
    for (auto [l, r] : s) {
      if (l > w) a += F(l) - F(w);
      w = max(w, r);
    }
  } return a * 0.5;
}
