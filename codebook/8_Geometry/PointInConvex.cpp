// CCW convex polygon; no repeated/collinear intermediate vertices.
// Do not repeat the first vertex at the end. Query: O(log n).
// Integer coordinates |x|, |y| <= 1e18 (including query points).
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
// 0 outside, 1 boundary, 2 strictly inside.
int pointInConvex(const vector<IPoint>& ch, IPoint p) {
  int n = ch.size();
  if (n == 0) return 0;
  if (n == 1) return p.x == ch[0].x && p.y == ch[0].y;
  if (n == 2) return onSegment(ch[0], ch[1], p);
  auto a = cross(ch[0], ch[1], p);
  auto b = cross(ch[0], ch[n - 1], p);
  if (a < 0 || b > 0) return 0;
  if (a == 0) return onSegment(ch[0], ch[1], p);
  if (b == 0) return onSegment(ch[0], ch[n - 1], p);
  int l = 1, r = n - 1;
  while (r - l > 1) {
    int m = l + (r - l) / 2;
    if (cross(ch[0], ch[m], p) >= 0) l = m;
    else r = m;
  }
  auto c = cross(ch[l], ch[r], p);
  return c < 0 ? 0 : (c == 0 ? 1 : 2);
}
