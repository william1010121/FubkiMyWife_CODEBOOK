double closest_pair(int l, int r) {
  // Inclusive [l,r]; valid finite x-sorted p if l<r; Euclidean dist.
  // O(n log n), O(n) space; finite arithmetic; p unchanged; l>=r: 1e9.
  if (l >= r) return 1e9;
  double d = dist(p[l], p[l + 1]);
  set<pair<double, int>> active; // (y, index) in the x-window.
  for (int i = l, j = l; i <= r; ++i) {
    while (j < i && p[i].x - p[j].x >= d)
      active.erase({p[j].y, j}), ++j;
    auto it = active.lower_bound({p[i].y - d, l});
    for (; it != active.end() && it->first <= p[i].y + d; ++it)
      d = min(d, dist(p[i], p[it->second]));
    if (d == 0) return 0;
    if (i < r && p[i + 1].x - p[i].x < d) active.insert({p[i].y, i});
  } return d;
}
