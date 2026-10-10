struct C {
  P c; double r;
  C(P c = P(0, 0), double r = 0) : c(c), r(r) {}
};
vector<P> Intersect(C a, C b) {
  if (a.r > b.r) swap(a, b);
  double d = (a.c - b.c).abs(); vector<P> p;
  if (same(d, 0)) return p; // Coincident: no finite point list.
  if (same(a.r + b.r, d))
    p.push_back(a.c + (b.c - a.c).unit() * a.r);
  else if (same(d + a.r, b.r))
    p.push_back(a.c - (b.c - a.c).unit() * a.r);
  else if (a.r + b.r > d && d + a.r >= b.r) {
    double o = acos((sq(a.r) + sq(d) - sq(b.r)) / (2 * a.r * d));
    P i = (b.c - a.c).unit();
    p.push_back(a.c + i.rot(o) * a.r);
    p.push_back(a.c + i.rot(-o) * a.r);
  } return p;
}
double IntersectArea(C a, C b) {
  if (a.r > b.r) swap(a, b);
  double d = (a.c - b.c).abs();
  if (d >= a.r + b.r - eps) return 0;
  if (d + a.r <= b.r + eps) return sq(a.r) * acos(-1);
  double p = acos((sq(a.r) + sq(d) - sq(b.r)) / (2 * a.r * d)),
    q = acos((sq(b.r) + sq(d) - sq(a.r)) / (2 * b.r * d));
  return p * sq(a.r) + q * sq(b.r) - a.r * d * sin(p);
}
// Segment by default; drop parameter-range checks for an infinite line.
vector<P> CircleCrossLine(P a, P b, P o, double r) {
  double x = b.x - a.x, y = b.y - a.y, A = sq(x) + sq(y);
  if (same(A, 0)) // Zero-length segment is a point.
    return same((a - o).abs(), r) ? vector<P>{a} : vector<P>{};
  double B = 2 * (x * (a.x - o.x) + y * (a.y - o.y)),
    C = sq(a.x - o.x) + sq(a.y - o.y) - sq(r), d = B * B - 4 * A * C;
  vector<P> t;
  if (d >= -eps) {
    double root = sqrt(max(0., d)), i = (-B - root) / (2 * A),
      j = (-B + root) / (2 * A);
    if (i <= 1 + eps && i >= -eps) t.push_back(a + (b - a) * i);
    if (fabs(j - i) > eps && j <= 1 + eps && j >= -eps)
      t.push_back(a + (b - a) * j);
  } return t;
}
// Unsigned area of circle(r) cap triangle OAB.
double AreaOfCircleTriangle(P a, P b, double r) {
  bool ina = a.abs() < r, inb = b.abs() < r;
  auto p = CircleCrossLine(a, b, P(0, 0), r);
  if (ina && inb) return abs(a ^ b) / 2;
  if (ina) return SectorArea(b, p[0], r) + abs(a ^ p[0]) / 2;
  if (inb) return SectorArea(p[0], a, r) + abs(p[0] ^ b) / 2;
  if (p.size() == 2) return SectorArea(a, p[0], r) +
    SectorArea(p[1], b, r) + abs(p[0] ^ p[1]) / 2;
  return SectorArea(a, b, r);
}
// Any triangle, three vertices; circle centered at origin.
double AreaOfCircleTriangle(vector<P> ps, double r) {
  double ans = 0;
  for (int i = 0; i < 3; ++i) {
    int j = (i + 1) % 3;
    double o = ps[i].angle() - ps[j].angle();
    if (o >= pi) o -= 2 * pi;
    if (o <= -pi) o += 2 * pi;
    ans += AreaOfCircleTriangle(ps[i], ps[j], r) * (o >= 0 ? 1 : -1);
  } return abs(ans);
}
