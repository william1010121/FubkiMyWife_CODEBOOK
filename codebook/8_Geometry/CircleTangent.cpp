// Positive radii; coincident circles have no finite tangent list.
vector<L> tangent(C a, C b) {
  if (a.r < b.r) swap(a, b);
  vector<L> z; double d = (a.c - b.c).abs();
  if (same(d, 0) || d + b.r < a.r) return z;
  P u = (b.c - a.c) / d;
  auto one = [&] {
    P i = u * a.r, j(i.y, -i.x);
    z.emplace_back(a.c + i, a.c + i + j);
  };
  auto two = [&](double e, double side) {
    double o = acos(e / d);
    for (double t : {o, -o}) {
      P v = u.rot(t);
      z.emplace_back(a.c + v * a.r, b.c + v * (side * b.r));
    }
  };
  if (same(d + b.r, a.r)) one();
  else {
    two(a.r - b.r, 1);
    if (same(d, a.r + b.r)) one();
    else if (d > a.r + b.r) two(a.r + b.r, -1);
  } return z;
}
vector<L> tangent(C c, P p) {
  vector<L> z; double d = (p - c.c).abs();
  if (same(d, c.r)) z.emplace_back(p, p + (p - c.c).rot(pi / 2));
  else if (d > c.r) {
    double o = acos(c.r / d); P u = (p - c.c) / d;
    for (double t : {o, -o}) z.emplace_back(c.c + u.rot(t) * c.r, p);
  } return z;
}
