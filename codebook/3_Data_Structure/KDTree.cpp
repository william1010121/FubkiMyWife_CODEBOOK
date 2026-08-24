namespace kdt {
int root, lc[maxn], rc[maxn], xl[maxn], xr[maxn],
  yl[maxn], yr[maxn];
point p[maxn];
int build(int l, int r, int dep = 0) {
  if (l == r) return -1;
  function<bool(const point &, const point &)> f =
    [dep](const point &a, const point &b) {
      return dep & 1 ? a.x < b.x : a.y < b.y;
    };
  int m = (l + r) >> 1;
  nth_element(p + l, p + m, p + r, f);
  xl[m] = xr[m] = p[m].x;
  yl[m] = yr[m] = p[m].y;
  lc[m] = build(l, m, dep + 1);
  if (~lc[m])
    xl[m] = min(xl[m], xl[lc[m]]),
      xr[m] = max(xr[m], xr[lc[m]]),
    yl[m] = min(yl[m], yl[lc[m]]),
      yr[m] = max(yr[m], yr[lc[m]]);
  rc[m] = build(m + 1, r, dep + 1);
  if (~rc[m])
    xl[m] = min(xl[m], xl[rc[m]]),
      xr[m] = max(xr[m], xr[rc[m]]),
    yl[m] = min(yl[m], yl[rc[m]]),
      yr[m] = max(yr[m], yr[rc[m]]);
  return m;
}
bool bound(const point &q, int o, long long d) {
  // Keep the pruning test exact.  In particular, converting d to a
  // floating-point radius can round down near a perfect square.
  __int128 dx = 0, dy = 0;
  if (q.x < xl[o]) dx = (__int128)xl[o] - q.x;
  else if (q.x > xr[o]) dx = (__int128)q.x - xr[o];
  if (q.y < yl[o]) dy = (__int128)yl[o] - q.y;
  else if (q.y > yr[o]) dy = (__int128)q.y - yr[o];
  return dx * dx + dy * dy <= (__int128)d;
}
long long dist(const point &a, const point &b) {
  __int128 dx = (__int128)a.x - b.x;
  __int128 dy = (__int128)a.y - b.y;
  __int128 d = dx * dx + dy * dy;
  // The public return type is long long; saturate distances that do not fit.
  return d > LLONG_MAX ? LLONG_MAX : (long long)d;
}
void dfs(
  const point &q, long long &d, int o, int dep = 0) {
  if (!bound(q, o, d)) return;
  long long cd = dist(p[o], q);
  if (cd) d = min(d, cd);
  int a = lc[o], b = rc[o];
  if (!(dep & 1 ? q.x < p[o].x : q.y < p[o].y)) swap(a,
    b);
  if (~a) dfs(q, d, a, dep + 1);
  if (~b) dfs(q, d, b, dep + 1);
}
void init(const vector<point> &v) {
  copy(v.begin(), v.end(), p);
  root = build(0, v.size());
}
long long nearest(const point &q) {
  long long res = LLONG_MAX;
  if (~root) dfs(q, res, root);
  return res;
}
} // namespace kdt
