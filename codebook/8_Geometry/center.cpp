// Nonzero-area simple polygon, CW/CCW; O(p.size()). n is unused.
Point BaryCenter(vector<Point> &p, int n) {
  Point res(0, 0); double s = 0;
  for (int i = 1; i + 1 < (int)p.size(); ++i) {
    double t = Cross(p[i] - p[0], p[i + 1] - p[0]);
    s += t, res.x += (p[0].x + p[i].x + p[i + 1].x) * t;
    res.y += (p[0].y + p[i].y + p[i + 1].y) * t;
  } return Point(res.x / (3 * s), res.y / (3 * s));
}
