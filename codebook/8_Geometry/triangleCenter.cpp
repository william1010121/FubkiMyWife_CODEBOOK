// Noncollinear triangle; each center O(1).
Point TriangleCircumCenter(Point a, Point b, Point c) {
  Point u = b - a, v = c - a;
  double x = u.x * u.x + u.y * u.y,
    y = v.x * v.x + v.y * v.y, d = 2 * Cross(u, v);
  return a + Point((v.y * x - u.y * y) / d,
    (u.x * y - v.x * x) / d);
}
Point TriangleMassCenter(Point a, Point b, Point c) {
  return (a + b + c) / 3.0;
}
Point TriangleOrthoCenter(Point a, Point b, Point c) {
  return (a + b + c) - TriangleCircumCenter(a, b, c) * 2.0;
}
Point TriangleInnerCenter(Point a, Point b, Point c) {
  double la = len(b - c), lb = len(a - c), lc = len(a - b);
  return (a * la + b * lb + c * lc) / (la + lb + lc);
}
