// Other points <= in all dims; fixed k: O(n log^max(1,k-1) n).
// Equal-size rows; n,k fit int; enough recursion stack.
vector<ll> dominance(vector<vector<int>> a) {
  int n = SZ(a), k = n ? SZ(a[0]) : 0, m = n;
  if (k > 1) { // Only the last dim needs compression (for the BIT).
    vector<int> v(n);
    for (int i = 0; i < n; ++i) v[i] = a[i].back();
    sort(ALL(v)); v.erase(unique(ALL(v)), v.end()), m = SZ(v);
    for (auto &p : a) p.back() = lower_bound(ALL(v), p.back())-v.begin()+1;
  }
  vector<int> ord(n), grp(n), rep, w, bit(m + 1LL); iota(ALL(ord), 0);
  sort(ALL(ord), [&](int i, int j) { return a[i] < a[j]; });
  for (int i = 0; i < n; ++i) { // Merge equal points, weight = count.
    if (!i || a[ord[i]] != a[ord[i - 1]]) rep.pb(ord[i]), w.pb(0);
    ++w.back(), grp[ord[i]] = SZ(rep) - 1;
  }
  vector<ll> ans(SZ(rep)), res(n);
  if (k < 2) exclusive_scan(ALL(w), ans.begin(), 0LL);
  vector<int> coord; coord.reserve(rep.size() * size_t(k));
  for (int i : rep) coord.insert(coord.end(), ALL(a[i]));
  struct It { int g; bool up, qu; };
  auto val = [&](It x, int d) { return coord[size_t(x.g) * k + d]; };
  // Count earlier updates satisfying dims d..k-1 for each query.
  auto rec = [&](auto &&self, int d, span<It> v) -> void {
    if (SZ(v) < 2) return;
    if (d == k - 1) {
      for (auto x : v) {
        if (x.qu) for (int i = val(x, d); i; i -= i & -i)
          ans[x.g] += bit[i];
        if (x.up) for (ll i = val(x, d); i <= m; i += i & -i)
          bit[i] += w[x.g];
      }
      for (auto x : v) if (x.up)
        for (ll i = val(x, d); i <= m; i += i & -i) bit[i] = 0;
      return;
    } int mid = SZ(v) / 2;
    self(self, d, v.first(mid)), self(self, d, v.subspan(mid));
    vector<It> c; c.reserve(v.size());
    for (auto x : v.first(mid)) if (x.up) c.pb({x.g, 1, 0});
    for (auto x : v.subspan(mid)) if (x.qu) c.pb({x.g, 0, 1});
    sort(ALL(c), [&](It x, It y) {
      return pair(val(x, d), !x.up) < pair(val(y, d), !y.up); });
    self(self, d + 1, c); // Equal coords: updates before queries.
  };
  vector<It> v(SZ(rep));
  for (int i = 0; i < SZ(v); ++i) v[i] = {i, 1, 1};
  if (k > 1) rec(rec, 1, v);
  for (int i = 0; i < n; ++i) res[i] = ans[grp[i]] + w[grp[i]] - 1;
  return res; }
