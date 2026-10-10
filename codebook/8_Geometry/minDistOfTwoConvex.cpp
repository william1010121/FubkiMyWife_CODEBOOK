double _point_seg_dist(Point p, Point a, Point b) {
  double dx = b.x - a.x, dy = b.y - a.y, z = dx * dx + dy * dy;
  double t = z ? clamp(((p.x - a.x) * dx +
    (p.y - a.y) * dy) / z, 0., 1.) : 0;
  return hypot(p.x - a.x - t * dx, p.y - a.y - t * dy); }
double _cross(Point a, Point b, Point c) {
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x); }
bool _on_segment(Point a, Point b, Point p) {
  return fabs(_cross(a, b, p)) <= 1e-10 &&
    min(a.x, b.x) - 1e-10 <= p.x && p.x <= max(a.x, b.x) + 1e-10 &&
    min(a.y, b.y) - 1e-10 <= p.y && p.y <= max(a.y, b.y) + 1e-10; }
bool _segments_intersect(Point a, Point b, Point c, Point d) {
  double x1 = _cross(a, b, c), x2 = _cross(a, b, d),
    x3 = _cross(c, d, a), x4 = _cross(c, d, b);
  return _on_segment(a, b, c) || _on_segment(a, b, d) ||
    _on_segment(c, d, a) || _on_segment(c, d, b) ||
    ((x1 > 0) != (x2 > 0) && (x3 > 0) != (x4 > 0)); }
bool _inside_convex(Point p, Point poly[], int n) {
  int sign = 0; for (int i = 0; i < n; ++i) {
    double c = _cross(poly[i], poly[(i + 1) % n], p);
    if (fabs(c) > 1e-10) { int s = c > 0 ? 1 : -1;
      if (sign && sign != s) return false;
      sign = s; }
  } return true;
}
// Nonempty convex polygons, CW/CCW; O(n+m) time and space.
double TwoConvexHullMinDist(Point P[], Point Q[], int n, int m) {
  auto prepare = [&](Point p[], int n, int s) {
    vector<Point> a(p, p + n);
    for (auto &v : a) v.x *= s, v.y *= s;
    double area = 0; for (int i = 1; i + 1 < (int)a.size(); ++i)
      area += _cross(a[0], a[i], a[i + 1]);
    if (area < 0) reverse(a.begin(), a.end());
    auto less = [](Point u, Point v) { return
      tie(u.y, u.x) < tie(v.y, v.x); };
    if (!area) { auto [l, r] = minmax_element(a.begin(), a.end(), less);
      a = {*l, *r}; } // Collinear polygon becomes a segment.
    rotate(a.begin(), min_element(a.begin(), a.end(), less), a.end());
    return a; };
  auto a = prepare(P, n, 1), b = prepare(Q, m, -1);
  n = a.size(), m = b.size();
  auto sum = [&](int i, int j) { return Point{
    a[i % n].x + b[j % m].x, a[i % n].y + b[j % m].y}; };
  double ans = 1e100, area = 0; bool inside = true;
  for (int i = 0, j = 0; i < n || j < m;) {
    Point p = sum(i, j), u = a[(i + 1) % n], v = b[(j + 1) % m];
    double z = _cross({0, 0}, {u.x - a[i % n].x, u.y - a[i % n].y},
      {v.x - b[j % m].x, v.y - b[j % m].y});
    bool az = u.x == a[i % n].x && u.y == a[i % n].y,
      bz = v.x == b[j % m].x && v.y == b[j % m].y;
    bool x = i < n && (j == m || az || (!bz && z >= 0)),
      y = j < m && (i == n || bz || (!az && z <= 0));
    i += x, j += y; Point q = sum(i, j);
    inside &= _cross(p, q, {0, 0}) >= -1e-10;
    area += _cross({0, 0}, p, q);
    ans = min(ans, _point_seg_dist({0, 0}, p, q));
  } return inside && area > 0 ? 0 : ans; // Origin to P + (-Q).
}
