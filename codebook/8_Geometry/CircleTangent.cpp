// eps>0, radii>0; finite math, reliable distance tests.
// Resolved contacts/directions; eps-close centers: empty.
vector<L> tangent(C a, C b) {
  if (a.r < b.r) swap(a, b);
  vector<L> z; double d = (a.c - b.c).abs();
  if (same(d, 0) || d + b.r < a.r) return z;
  P u = (b.c - a.c) / d; z.reserve(4);
  for (double side : {1., -1.}) { // External first.
    bool touch = side == 1 ? same(d + b.r, a.r) :
      same(d, a.r + b.r);
    if (touch) { P i = u * a.r, j(i.y, -i.x);
      z.emplace_back(a.c + i, a.c + i + j);
      if (side == 1) break;
    } else if (side == 1 || d > a.r + b.r) {
      double h = (a.r - side * b.r) / d;
      double t = sqrt(max(0., 1 - h * h));
      for (double k : {t, -t}) {
        P v = u * h + P(-u.y, u.x) * k;
        z.emplace_back(a.c + v * a.r,
                       b.c + v * (side * b.r));
      }
    }
  } return z;
}
vector<L> tangent(C c, P p) {
  vector<L> z; double d = (p - c.c).abs();
  if (d > 0 && same(d, c.r))
    z.emplace_back(p, p + (p - c.c).rot(pi / 2));
  else if (d > c.r) {
    double h = c.r / d;
    double t = sqrt(max(0., (1 - h) * (1 + h)));
    P u = (p - c.c) / d; z.reserve(2);
    for (double k : {t, -t})
      z.emplace_back(
        c.c + (u * h + P(-u.y, u.x) * k) * c.r, p);
  } return z;
}
