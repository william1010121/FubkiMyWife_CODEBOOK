double _point_seg_dist(Point p, Point a, Point b) {
  double dx = b.x - a.x, dy = b.y - a.y;
  double z = dx * dx + dy * dy;
  double t = z ? clamp(((p.x - a.x) * dx +
    (p.y - a.y) * dy) / z, 0., 1.) : 0;
  return hypot(p.x - a.x - t * dx, p.y - a.y - t * dy); }
double _cross(Point a, Point b, Point c) {
  return (b.x - a.x) * (c.y - a.y) -
    (b.y - a.y) * (c.x - a.x); }
bool _on_segment(Point a, Point b, Point p) {
  return fabs(_cross(a, b, p)) <= 1e-10 &&
    min(a.x, b.x) - 1e-10 <= p.x &&
    p.x <= max(a.x, b.x) + 1e-10 &&
    min(a.y, b.y) - 1e-10 <= p.y &&
    p.y <= max(a.y, b.y) + 1e-10; }
bool _segments_intersect(Point a, Point b,
    Point c, Point d) {
  double x1 = _cross(a, b, c), x2 = _cross(a, b, d),
    x3 = _cross(c, d, a), x4 = _cross(c, d, b);
  return _on_segment(a, b, c) || _on_segment(a, b, d) ||
    _on_segment(c, d, a) || _on_segment(c, d, b) ||
    ((x1 > 0) != (x2 > 0) && (x3 > 0) != (x4 > 0)); }
bool _inside_convex(Point p, Point poly[], int n) {
  // Nonzero area.
  int sign = 0; for (int i = 0; i < n; ++i) {
    double c = _cross(poly[i], poly[(i + 1) % n], p);
    if (fabs(c) > 1e-10) { int s = c > 0 ? 1 : -1;
      if (sign && sign != s) return false;
      sign = s; }
  } return true;
}
// Nonempty cyclic convex CW/CCW; repeats OK.
// Finite arithmetic; reliable signs and 1e-10 tests.
double TwoConvexHullMinDist(Point P[], Point Q[],
    int n, int m) {
  auto prepare = [&](Point p[], int n, int s) {
    vector<Point> a; a.reserve(n + 1); a.assign(p, p + n);
    for (auto &v : a) v.x *= s, v.y *= s;
    double area = 0;
    for (int i = 1; i + 1 < (int)a.size(); ++i)
      area += _cross(a[0], a[i], a[i + 1]);
    if (area < 0) reverse(ALL(a));
    auto less = [](Point u, Point v) { return
      tie(u.y, u.x) < tie(v.y, v.x); };
    if (!area) { auto [l, r] = minmax_element(ALL(a),less);
      a = {*l, *r}; } // Collinear: segment.
    rotate(a.begin(), min_element(ALL(a), less), a.end());
    a.push_back(a[0]); return a; }; // Close for indexing.
  auto a = prepare(P, n, 1), b = prepare(Q, m, -1);
  n = a.size() - 1, m = b.size() - 1;
  auto sum = [&](int i, int j) { return Point{
    a[i].x + b[j].x, a[i].y + b[j].y}; };
  double ans = numeric_limits<double>::infinity();
  double area = 0; bool inside = true;
  auto half = [](Point p) { return // Opposite edges.
    p.y < 0 || (!p.y && p.x < 0); };
  for (int i = 0, j = 0; i < n || j < m;) {
    Point p = sum(i, j), u = a[min(i + 1, n)],
      v = b[min(j + 1, m)];
    u.x -= a[i].x, u.y -= a[i].y,
      v.x -= b[j].x, v.y -= b[j].y;
    double z = _cross({0, 0}, u, v);
    bool az = !u.x && !u.y, bz = !v.x && !v.y;
    if (!az && !bz && half(u) != half(v))
      z = half(v) - half(u);
    bool x = i < n && (j == m || az || (!bz && z >= 0)),
      y = j < m && (i == n || bz || (!az && z <= 0));
    i += x, j += y; Point q = sum(i, j);
    inside &= _cross(p, q, {0, 0}) >= -1e-10;
    area += _cross({0, 0}, p, q);
    ans = min(ans, _point_seg_dist({0, 0}, p, q));
  } // Minkowski difference; O(n+m) time/space.
  return inside && area > 0 ? 0 : ans;
}
