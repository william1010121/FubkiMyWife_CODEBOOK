Point TriangleCircumCenter(Point a, Point b, Point c) {
  Point u = b - a, v = c - a;
  double x = u.x * u.x + u.y * u.y, y = v.x * v.x + v.y * v.y;
  double d = 2 * Cross(u, v);
  return a + Point((v.y * x - u.y * y) / d, (u.x * y - v.x * x) / d);
}

Point TriangleMassCenter(Point a, Point b, Point c) {
  return (a + b + c) / 3.0;
}

Point TriangleOrthoCenter(Point a, Point b, Point c) { 
  return TriangleMassCenter(a, b, c) * 3.0 - TriangleCircumCenter(a, b, c) * 2.0;
}

Point TriangleInnerCenter(Point a, Point b, Point c) {
  double la = len(b - c);
  double lb = len(a - c);
  double lc = len(a - b);
  double s = la + lb + lc;
  return {(la * a.x + lb * b.x + lc * c.x) / s,
      (la * a.y + lb * b.y + lc * c.y) / s};
}
