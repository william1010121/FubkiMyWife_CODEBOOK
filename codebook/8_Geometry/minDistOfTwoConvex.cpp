// Convex polygons in cyclic order; self-contained O(nm) implementation.
double _point_seg_dist(Point p, Point a, Point b) {
  double dx = b.x - a.x, dy = b.y - a.y, z = dx * dx + dy * dy;
  double t = z ? clamp(((p.x - a.x) * dx + (p.y - a.y) * dy) / z, 0., 1.) : 0;
  return hypot(p.x - (a.x + t * dx), p.y - (a.y + t * dy));
}

double _cross(Point a, Point b, Point c) {
  return (b.x - a.x) * (c.y - a.y) -
         (b.y - a.y) * (c.x - a.x);
}

bool _on_segment(Point a, Point b, Point p) {
  return fabs(_cross(a, b, p)) <= 1e-10 &&
    min(a.x, b.x) - 1e-10 <= p.x && p.x <= max(a.x, b.x) + 1e-10 &&
    min(a.y, b.y) - 1e-10 <= p.y && p.y <= max(a.y, b.y) + 1e-10;
}

bool _segments_intersect(Point a, Point b, Point c, Point d) {
  double x1 = _cross(a, b, c), x2 = _cross(a, b, d);
  double x3 = _cross(c, d, a), x4 = _cross(c, d, b);
  if (_on_segment(a, b, c) || _on_segment(a, b, d) || _on_segment(c, d, a) || _on_segment(c, d, b)) return true;
  return (x1 > 0) != (x2 > 0) && (x3 > 0) != (x4 > 0);
}

bool _inside_convex(Point p, Point poly[], int n) {
  int sign = 0;
  for (int i = 0; i < n; ++i) {
    double c = _cross(poly[i], poly[(i + 1) % n], p);
    if (fabs(c) <= 1e-10) continue;
    if (!sign) sign = c > 0 ? 1 : -1;
    else if ((c > 0 ? 1 : -1) != sign) return false;
  }
  return true;
}

double TwoConvexHullMinDist(Point P[], Point Q[], int n, int m) {
  double ans = 1e100;
  for (int i = 0; i < n; ++i) {
    Point a = P[i], b = P[(i + 1) % n];
    for (int j = 0; j < m; ++j) {
      Point c = Q[j], d = Q[(j + 1) % m];
      if (_segments_intersect(a, b, c, d)) return 0;
      ans = min(ans, _point_seg_dist(a, c, d));
      ans = min(ans, _point_seg_dist(c, a, b));
    }
  }
  if (_inside_convex(P[0], Q, m) || _inside_convex(Q[0], P, n)) return 0;
  return ans;
}
