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
    xl[m] = min(xl[m], xl[lc[m]]), xr[m] = max(xr[m], xr[lc[m]]),
    yl[m] = min(yl[m], yl[lc[m]]), yr[m] = max(yr[m], yr[lc[m]]);
  rc[m] = build(m + 1, r, dep + 1);
  if (~rc[m])
    xl[m] = min(xl[m], xl[rc[m]]), xr[m] = max(xr[m], xr[rc[m]]),
    yl[m] = min(yl[m], yl[rc[m]]), yr[m] = max(yr[m], yr[rc[m]]);
  return m;
}
bool bound(const point &q, int o, long long d) {
  double ds = sqrt(d + 1.0);
  return !(q.x < xl[o] - ds || q.x > xr[o] + ds ||
    q.y < yl[o] - ds || q.y > yr[o] + ds);
}
long long dist(const point &a, const point &b) {
  return (a.x - b.x) * 1ll * (a.x - b.x) +
    (a.y - b.y) * 1ll * (a.y - b.y);
}
void dfs(
  const point &q, long long &d, int o, int dep = 0) {
  if (!bound(q, o, d)) return;
  long long cd = dist(p[o], q);
  if (cd) d = min(d, cd);
  int a = lc[o], b = rc[o];
  if (!(dep & 1 ? q.x < p[o].x : q.y < p[o].y)) swap(a, b);
  if (~a) dfs(q, d, a, dep + 1);
  if (~b) dfs(q, d, b, dep + 1);
}
void init(const vector<point> &v) {
  copy(v.begin(), v.end(), p); root = build(0, v.size());
}
long long nearest(const point &q) {
  long long res = 1e18; if (~root) dfs(q, res, root); return res;
}
} // namespace kdt
