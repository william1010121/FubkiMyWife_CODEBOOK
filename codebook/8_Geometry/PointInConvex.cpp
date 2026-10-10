// CCW convex; no repeated/intermediate collinear vertices.
// No closing duplicate. Queries too: |x|,|y|<=1e18.
// O(log n): 0 outside, 1 boundary, 2 strictly inside.
struct IPoint { long long x, y; };
__int128 cross(IPoint a, IPoint b, IPoint c) {
  return ((__int128)b.x - a.x) * ((__int128)c.y - a.y)
       - ((__int128)b.y - a.y) * ((__int128)c.x - a.x);
}
bool onSegment(IPoint a, IPoint b, IPoint p) {
  return cross(a, b, p) == 0 &&
    min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
    min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}
int pointInConvex(const vector<IPoint>& ch, IPoint p) {
  int n = ch.size();
  if (n < 3) return n && onSegment(ch[0], ch[n - 1], p);
  auto a = cross(ch[0], ch[1], p),
       b = cross(ch[0], ch[n - 1], p);
  if (a < 0 || b > 0) return 0;
  if (!a) return onSegment(ch[0], ch[1], p);
  if (!b) return onSegment(ch[0], ch[n - 1], p);
  auto it = partition_point(ch.begin() + 1, ch.end(),
    [&](IPoint q) { return cross(ch[0], q, p) >= 0; });
  auto c = cross(*prev(it), *it, p);
  return c < 0 ? 0 : (c == 0 ? 1 : 2);
}
