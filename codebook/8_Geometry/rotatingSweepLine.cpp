// Distinct points, no three collinear; O(n^2 log n), O(n^2) space.
vector<int> rotatingSweepLineOrder(const vector<pair<int,int>> &ps) {
  int n = ps.size();
  vector<int> id(n), pos(n);
  vector<pair<int,int>> line;
  line.reserve(1LL * n * (n - 1) / 2);
  for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
    line.emplace_back(i, j);
  auto direction = [&](pair<int,int> e) {
    long long x = 1LL * ps[e.first].first - ps[e.second].first,
      y = 1LL * ps[e.first].second - ps[e.second].second;
    // Unoriented directions need a common half-plane before sorting.
    if (x < 0 || (x == 0 && y < 0)) x = -x, y = -y;
    return pair<long long,long long>{x, y};
  };
  sort(line.begin(), line.end(), [&](auto a, auto b) {
    auto [ax, ay] = direction(a); auto [bx, by] = direction(b);
    if (!ax || !bx) return !ax && bx;
    return (__int128)ay * bx < (__int128)by * ax;
  });
  iota(id.begin(), id.end(), 0);
  sort(id.begin(), id.end(), [&](int a, int b) { return ps[a] < ps[b]; });
  for (int i = 0; i < n; ++i) pos[id[i]] = i;
  for (auto [a, b] : line)
    swap(id[pos[a]], id[pos[b]]), swap(pos[a], pos[b]);
  return id;
}
void rotatingSweepLine(vector<pair<int,int>> &ps) {
  (void)rotatingSweepLineOrder(ps);
}
