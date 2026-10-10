// Distinct points, no 3 collinear; n fits int.
// O(n^2 log n) time, O(n^2) event storage.
vector<int> rotatingSweepLineOrder(const vector<pii> &ps) {
  int n = ps.size(); vector<int> id(n), pos(n);
  struct Event { int a, b; ll x, y; };
  vector<Event> line; line.reserve(1LL * n * (n - 1) / 2);
  for (int i = 0; i < n; ++i)
    for (int j = i + 1; j < n; ++j) {
      ll x = ll(ps[i].X) - ps[j].X,
        y = ll(ps[i].Y) - ps[j].Y;
      // x>0 or x=0,y>0; vertical events last.
      if (x < 0 || (x == 0 && y < 0)) x = -x, y = -y;
      line.push_back({i, j, x, y});
    }
  sort(ALL(line), [](const Event &a, const Event &b) {
    return (__int128)a.y * b.x < (__int128)b.y * a.x;
  });
  iota(ALL(id), 0);
  sort(ALL(id), [&](int a, int b) {
    return ps[a] < ps[b]; });
  for (int i = 0; i < n; ++i) pos[id[i]] = i;
  for (auto e : line)
    swap(id[pos[e.a]], id[pos[e.b]]),
      swap(pos[e.a], pos[e.b]);
  return id;
}
void rotatingSweepLine(vector<pii> &ps) {
  rotatingSweepLineOrder(ps); }
