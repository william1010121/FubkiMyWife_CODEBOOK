pt center(const pt &a, const pt &b, const pt &c) {
  pt p0 = b - a, p1 = c - a;
  double c1 = norm2(p0) * 0.5, c2 = norm2(p1) * 0.5;
  double d = p0 ^ p1;
  return a + pt(c1 * p1.y - c2 * p0.y,
                c2 * p0.x - c1 * p1.x) / d;
}
// Shuffles p; expected O(n), worst O(n^3).
circle min_enclosing(vector<pt> &p) {
  static mt19937 rng(712367821);
  shuffle(ALL(p), rng);
  double r = 0;
  pt cent;
  for (int i = 0; i < SZ(p); ++i) {
    if (norm2(cent - p[i]) <= r) continue;
    cent = p[i], r = 0;
    for (int j = 0; j < i; ++j) {
      if (norm2(cent - p[j]) <= r) continue;
      cent = (p[i] + p[j]) / 2, r = norm2(p[j] - cent);
      for (int k = 0; k < j; ++k) {
        if (norm2(cent - p[k]) <= r) continue;
        cent = center(p[i], p[j], p[k]), r = norm2(p[k] - cent);
      }
    }
  } return circle(cent, sqrt(r));
}
