// Distinct points, no three collinear; O(n^2 log n), O(n^2) space.
vector<int> rotatingSweepLineOrder(const vector<pair<int,int>> &ps) {
  int n = ps.size(); vector<int> id(n), pos(n);
  struct Event { int a, b; long long x, y; };
  vector<Event> line; line.reserve(1LL * n * (n - 1) / 2);
  for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) {
    long long x = 1LL * ps[i].first - ps[j].first,
      y = 1LL * ps[i].second - ps[j].second;
    // Unoriented directions need a common half-plane before sorting.
    if (x < 0 || (x == 0 && y < 0)) x = -x, y = -y;
    line.push_back({i, j, x, y});
  }
  sort(line.begin(), line.end(), [](const Event &a, const Event &b) {
    if (!a.x || !b.x) return a.x && !b.x; // Vertical events last.
    return (__int128)a.y * b.x < (__int128)b.y * a.x;
  });
  iota(id.begin(), id.end(), 0);
  sort(id.begin(), id.end(), [&](int a, int b) { return ps[a] < ps[b]; });
  for (int i = 0; i < n; ++i) pos[id[i]] = i;
  for (auto e : line)
    swap(id[pos[e.a]], id[pos[e.b]]), swap(pos[e.a], pos[e.b]);
  return id;
}
void rotatingSweepLine(vector<pair<int,int>> &ps) {
  (void)rotatingSweepLineOrder(ps);
}
