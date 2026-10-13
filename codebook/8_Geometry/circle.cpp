struct C { // r>=0; finite arithmetic; O(1) operations
  P c; double r;
  C(P c = P(0, 0), double r = 0) : c(c), r(r) {}
};
vector<P> Intersect(C a, C b) {
  if (a.r > b.r) swap(a, b);
  double d = (a.c - b.c).abs();
  if (same(d, 0)) return {}; // Coincident: no finite list.
  if (same(a.r + b.r, d))
    return {a.c + (b.c - a.c) / d * a.r};
  if (same(d + a.r, b.r))
    return {a.c - (b.c - a.c) / d * a.r};
  if (a.r + b.r <= d || d + a.r < b.r) return {};
  double z = d < b.r / 2
    ? sq(d) + (a.r - b.r) * (a.r + b.r)
    : sq(a.r) + (d - b.r) * (d + b.r);
  double h = z / (2 * a.r * d);
  P u = (b.c - a.c) / d, v = u * (a.r * h),
    w = P(-u.y, u.x) * (a.r * sqrt(max(0., 1 - h * h)));
  return {a.c + v + w, a.c + v - w};
}
double IntersectArea(C a, C b) {
  if (a.r > b.r) swap(a, b);
  double d = (a.c - b.c).abs();
  if (d >= a.r + b.r - eps) return 0;
  if (d + a.r <= b.r + eps) return sq(a.r) * acos(-1);
  double z = d < b.r / 2
    ? sq(d) + (a.r - b.r) * (a.r + b.r)
    : sq(a.r) + (d - b.r) * (d + b.r), x = z / (2 * d),
    y = sqrt(max(0., sq(a.r) - sq(x))); // Chord (x,+/-y).
  auto cap = [](double r, double t) { // Small-angle cap
    return sq(r) * (t < 1e-3
      ? t * t * t * (2. / 3 - 2. / 15 * t * t)
      : t - sin(2 * t) / 2);
  }; return cap(a.r, atan2(y, x))
          + cap(b.r, atan2(y, d - x));
}
// r>=0; endpoint tolerance eps in coordinate length.
// Drop parameter-range checks for an infinite line.
vector<P> CircleCrossLine(P a, P b, P o, double r) {
  P u = b - a, v = a - o; double A = u * u;
  if (!A) // Zero-length segment is a point.
    return same((a - o).abs(), r)
      ? vector<P>{a} : vector<P>{};
  // Compensated cross preserves small tilted-line offsets.
  double t0 = u.y * v.x,
    z = fma(u.x, v.y, -t0) - fma(u.y, v.x, -t0);
  double mid = -(u * v) / A, d = sq(r) - sq(z) / A,
    tol = eps / sqrt(A);
  P p = P(-u.y, u.x) * (z / A);
  auto within = [&](double t) {
    return -tol <= t && t <= 1 + tol; };
  vector<P> t;
  if (d >= -eps) {
    double root = sqrt(max(0., d) / A),
      i = mid - root, j = mid + root;
    if (within(i))
      t.reserve(2), t.push_back(o + p - u * root);
    if (root > 0 && within(j))
      t.push_back(o + p + u * root);
  } return t;
}
// r>=0; unsigned disk cap OAB; last root exits from a.
double AreaOfCircleTriangle(P a, P b, double r) {
  bool ina = a.abs() < r, inb = b.abs() < r;
  if (ina && inb) return abs(a ^ b) / 2;
  auto p = CircleCrossLine(a, b, P(0, 0), r);
  if (ina) return SectorArea(b, p.back(), r)
                + abs(a ^ p.back()) / 2;
  if (inb) return SectorArea(p[0], a, r)
                + abs(p[0] ^ b) / 2;
  if (p.size() == 2) return SectorArea(a, p[0], r) +
    SectorArea(p[1], b, r) + abs(p[0] ^ p[1]) / 2;
  return SectorArea(a, b, r);
}
// r>=0; any triangle, exactly 3 vertices; disk at origin.
double AreaOfCircleTriangle(
  const vector<P> &ps, double r) {
  double ans = 0;
  for (int i = 0; i < 3; ++i) {
    int j = (i + 1) % 3; double z = ps[i] ^ ps[j];
    if (z) ans += AreaOfCircleTriangle(ps[i], ps[j], r) *
      (z > 0 ? 1 : -1);
  } return abs(ans);
}
