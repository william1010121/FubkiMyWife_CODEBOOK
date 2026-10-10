bool operator<(const P &a, const P &b) {
  return a.x == b.x ? a.y < b.y : a.x < b.x; }
bool operator>(const P &a, const P &b) { return b < a; }
double crx(P a, P b, P c) { return (b - a) ^ (c - a); }
// CCW; drops collinear points. O(n log n); reliable cross/dot signs.
vector<P> convex(vector<P> ps) {
  if (SZ(ps) < 2) return ps;
  sort(ALL(ps));
  if (!(ps.front() < ps.back())) return {ps[0]};
  vector<P> p; p.reserve(2 * SZ(ps));
  auto add = [&](P q, int t) {
    while (SZ(p) > t && crx(p[SZ(p) - 2], q, p.back()) >= 0)
      p.pop_back();
    p.pb(q);
  };
  for (P q : ps) add(q, 1);
  int t = SZ(p);
  for (int i = SZ(ps) - 2; i >= 0; --i) add(ps[i], t);
  p.pop_back();
  return p;
}
int sgn(double x) {
  return same(x, 0) ? 0 : x > 0 ? 1 : -1; }
P isLL(P p1, P p2, P q1, P q2) { // Nonparallel lines.
  double a = crx(q1, q2, p1), b = -crx(q1, q2, p2);
  return (p1 * b + p2 * a) / (a + b);
}
struct CH { // Strictly convex CCW, n>=3; reliable eps tests; O(log n).
  int n;
  vector<P> p, u, d;
  CH() {}
  CH(vector<P> ps) : n(SZ(ps)), p(move(ps)) {
    rotate(p.begin(), min_element(ALL(p)), p.end());
    auto t = max_element(ALL(p));
    d = vector<P>(p.begin(), next(t));
    u = vector<P>(t, p.end()); u.pb(p[0]);
  }
  int find(vector<P> &v, P d) { // Peak of findFarest's chosen chain.
    int l = 0, r = SZ(v);
    while (l + 1 < r) {
      int m = (l + r) / 2;
      if (v[m - 1] * d >= v[m] * d) r = m;
      else l = m;
    } return l;
  }
  int findFarest(P v) {
    if (v.y > 0 || (v.y == 0 && v.x > 0))
      return (SZ(d) - 1 + find(u, v)) % n;
    return find(d, v);
  }
  P get(int l, int r, P a, P b) {
    int s = sgn(crx(a, b, p[l % n]));
    while (l + 1 < r) {
      int m = (l + r) >> 1;
      if (sgn(crx(a, b, p[m % n])) == s) l = m;
      else r = m;
    } return isLL(a, b, p[l % n], p[(l + 1) % n]);
  }
  vector<P> getLineIntersect(P a, P b) { // a != b
    P v = b - a;
    int x = findFarest(P(-v.y, v.x)), y = findFarest(P(v.y, -v.x));
    if (x > y) swap(x, y);
    if (sgn(crx(a, b, p[x])) * sgn(crx(a, b, p[y])) < 0)
      return {get(x, y, a, b), get(y, x + n, a, b)};
    return {}; // tangent case falls here
  }
  void update_tangent(P q, int i, int &a, int &b) {
    if (sgn(crx(q, p[a], p[i])) > 0) a = i;
    if (sgn(crx(q, p[b], p[i])) < 0) b = i;
  }
  void bs(int l, int r, P q, int &a, int &b) {
    if (l == r) return;
    update_tangent(q, l % n, a, b);
    int s = sgn(crx(q, p[l % n], p[(l + 1) % n]));
    while (l + 1 < r) {
      int m = (l + r) >> 1;
      if (sgn(crx(q, p[m % n], p[(m + 1) % n])) == s) l = m;
      else r = m;
    } update_tangent(q, r % n, a, b);
  }
  bool contain(P p) {
    if (p.x < d[0].x || p.x > d.back().x) return 0;
    auto it = lower_bound(ALL(d), p.x,
      [](P a, double x) { return a.x < x; });
    if (it->x == p.x && it->y > p.y) return 0;
    if (it->x != p.x && crx(*prev(it), *it, p) < -eps) return 0;
    it = lower_bound(ALL(u), p.x,
      [](P a, double x) { return a.x > x; });
    if (it->x == p.x && it->y < p.y) return 0;
    if (it->x != p.x && crx(*prev(it), *it, p) < -eps) return 0;
    return 1;
  }
  bool get_tangent(P p, int &a, int &b) { // b -> a
    if (contain(p)) return 0;
    a = b = 0;
    int i = lower_bound(ALL(d), p) - d.begin();
    bs(0, i, p, a, b);
    bs(i, SZ(d), p, a, b);
    i = lower_bound(ALL(u), p, greater<P>()) - u.begin();
    bs(SZ(d) - 1, SZ(d) - 1 + i, p, a, b);
    bs(SZ(d) - 1 + i, SZ(d) - 1 + SZ(u), p, a, b);
    return 1;
  }
};
