// Distinct integer points, |x|,|y| <= 5e8. Returns CCW triangles,
// empty if n < 3 or all collinear. O(n log n).
namespace delaunay {
struct Point {
  ll x, y;
  Point(ll x = 0, ll y = 0) : x(x), y(y) {}
  Point operator-(Point b) const { return {x - b.x, y - b.y}; }
  ll operator^(Point b) const { return x * b.y - y * b.x; }
  ll Norm() const { return x * x + y * y; }
  bool operator<(Point b) const { return tie(x, y) < tie(b.x, b.y); }
  bool operator==(Point b) const { return x == b.x && y == b.y; }
};
const Point kA(LLONG_MAX, LLONG_MAX);  // not equal to any other point
bool InCircle(Point p, Point a, Point b, Point c) {
  a = a - p, b = b - p, c = c - p;
  __int128 x = __int128(a.Norm()) * (b ^ c) +
    __int128(b.Norm()) * (c ^ a) + __int128(c.Norm()) * (a ^ b);
  ll s = (b - a) ^ (c - a);
  return x * ((s > 0) - (s < 0)) > 0;
}
struct Quad {
  bool mark = 0;
  Quad *o = 0, *rot = 0;
  Point p;
  Quad(Point p) : p(p) {}
  Point F() { return r()->p; }
  Quad* r() { return rot->rot; }
  Quad* prev() { return rot->o->rot; }
  Quad* next() { return r()->prev(); }
};
Quad* MakeEdge(Point s, Point t) {
  Quad* q[4] = {new Quad(s), new Quad(kA), new Quad(t), new Quad(kA)};
  for (int i = 0; i < 4; ++i) q[i]->o = q[-i & 3], q[i]->rot = q[(i + 1) & 3];
  return q[0];
}
void Splice(Quad* a, Quad* b) {
  swap(a->o->rot->o, b->o->rot->o), swap(a->o, b->o);
}
Quad* Connect(Quad* a, Quad* b) {
  Quad* q = MakeEdge(a->F(), b->p);
  Splice(q, a->next()), Splice(q->r(), b);
  return q;
}
void Kill(Quad* e) { Splice(e, e->prev()), Splice(e->r(), e->r()->prev()); }
pair<Quad*, Quad*> Dfs(const vector<Point>& s, int l, int r) {
  if (r - l <= 3) {
    Quad *a = MakeEdge(s[l], s[l + 1]), *b = MakeEdge(s[l + 1], s[r - 1]);
    if (r - l == 2) return {a, a->r()};
    Splice(a->r(), b);
    ll side = (s[l + 1] - s[l]) ^ (s[l + 2] - s[l]);
    Quad* c = side ? Connect(b, a) : 0;
    return {side < 0 ? c->r() : a, side < 0 ? c : b->r()};
  }
  int m = (l + r) / 2;
  auto [ra, a] = Dfs(s, l, m);
  auto [b, rb] = Dfs(s, m, r);
  while ((((a->F() - b->p) ^ (a->p - b->p)) < 0 && (a = a->next())) ||
      (((b->F() - a->p) ^ (b->p - a->p)) > 0 && (b = b->r()->o)));
  Quad* base = Connect(b->r(), a);
  auto Valid = [&](Quad* e) {
    return ((base->F() - e->F()) ^ (base->p - e->F())) > 0;
  };
  if (a->p == ra->p) ra = base->r();
  if (b->p == rb->p) rb = base;
  while (1) {
    Quad *lc = base->r()->o, *rc;
    if (Valid(lc))
      while (InCircle(lc->o->F(), base->F(), base->p, lc->F()))
        { Quad* t = lc->o; Kill(lc); lc = t; }
    rc = base->prev();
    if (Valid(rc))
      while (InCircle(rc->prev()->F(), base->F(), base->p, rc->F()))
        { Quad* t = rc->prev(); Kill(rc); rc = t; }
    if (!Valid(lc) && !Valid(rc)) break;
    if (!Valid(lc) || (Valid(rc) && InCircle(rc->F(), rc->p, lc->F(), lc->p)))
      base = Connect(rc, base->r());
    else base = Connect(base->r(), lc->r());
  }
  return {ra, rb};
}
vector<array<Point, 3>> Triangulate(vector<Point> pts) {
  sort(ALL(pts));
  if (SZ(pts) < 2) return {};
  Quad* e = Dfs(pts, 0, SZ(pts)).first;
  vector<Quad*> q = {e};
  while (((e->F() - e->o->F()) ^ (e->p - e->o->F())) < 0) e = e->o;
  auto Add = [&]() {
    Quad* c = e;
    do c->mark = 1, pts.pb(c->p), q.pb(c->r()), c = c->next();
    while (c != e);
  };
  Add(), pts.clear();
  for (int i = 0; i < SZ(q); ++i) if (!(e = q[i])->mark) Add();
  vector<array<Point, 3>> res(SZ(pts) / 3);
  for (int i = 0; i < SZ(pts); ++i) res[i / 3][i % 3] = pts[i];
  return res;
}
}  // namespace delaunay
