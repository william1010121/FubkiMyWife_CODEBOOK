// Simple CW/CCW polygon; nonzero area, finite arithmetic.
// |p| fits int; n unused; reliable doubles; O(|p|).
Point BaryCenter(vector<Point> &p, int n) {
  Point res(0, 0), u = p[1] - p[0]; double s = 0;
  for (int i = 2; i < (int)p.size(); ++i) {
    Point v = p[i] - p[0]; double t = Cross(u, v);
    s += t, res.x += (u.x + v.x) * t,
      res.y += (u.y + v.y) * t, u = v;
  } return Point(p[0].x + res.x / (3 * s),
    p[0].y + res.y / (3 * s));
}
