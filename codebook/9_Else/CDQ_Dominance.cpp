// Counts other points <= in all dims; O(n log^(max(1,k-1)) n).
vector<ll> dominance(vector<vector<int>> a) { // Equal-size rows.
  int n = SZ(a), k = n ? SZ(a[0]) : 0;
  if (k) { // Only the last dim needs compression (for the BIT).
    vector<int> v(n);
    for (int i = 0; i < n; ++i) v[i] = a[i].back();
    sort(ALL(v));
    for (auto &p : a) p.back() = lower_bound(ALL(v), p.back())-v.begin()+1;
  }
  vector<int> ord(n), grp(n), rep, w;
  iota(ALL(ord), 0);
  sort(ALL(ord), [&](int i, int j) { return a[i] < a[j]; });
  for (int i = 0; i < n; ++i) { // Merge equal points, weight = count.
    if (!i || a[ord[i]] != a[ord[i - 1]]) rep.pb(ord[i]), w.pb(0);
    ++w.back(), grp[ord[i]] = SZ(rep) - 1;
  }
  vector<ll> ans(SZ(rep)), bit(n + 1), res(n);
  struct It { int g; bool up, qu; };
  auto val = [&](It x, int d) { return a[rep[x.g]][d]; };
  // Count earlier updates satisfying dims d..k-1 for each query.
  auto rec = [&](auto &&self, int d, vector<It> &v) -> void {
    if (d >= k) {
      ll s = 0;
      for (auto x : v) {
        if (x.qu) ans[x.g] += s; if (x.up) s += w[x.g];
      } return;
    } if (d == k - 1) {
      for (auto x : v) {
        if (x.qu) for (int i = val(x, d); i; i -= i & -i)
          ans[x.g] += bit[i];
        if (x.up) for (int i = val(x, d); i <= n; i += i & -i)
          bit[i] += w[x.g];
      }
      for (auto x : v) if (x.up)
        for (int i = val(x, d); i <= n; i += i & -i) bit[i] = 0;
      return;
    } int mid = SZ(v) / 2;
    if (!mid) return;
    vector<It> L(v.begin(), v.begin() + mid), R(v.begin()+mid, v.end()), c;
    self(self, d, L), self(self, d, R);
    for (auto x : L) if (x.up) c.pb({x.g, 1, 0});
    for (auto x : R) if (x.qu) c.pb({x.g, 0, 1});
    stable_sort(ALL(c), [&](It x, It y) { return val(x, d) < val(y, d); });
    self(self, d + 1, c); // Stable ties keep left updates first.
  };
  vector<It> v(SZ(rep));
  for (int i = 0; i < SZ(v); ++i) v[i] = {i, 1, 1};
  rec(rec, 1, v);
  for (int i = 0; i < n; ++i) res[i] = ans[grp[i]] + w[grp[i]] - 1;
  return res;
}
