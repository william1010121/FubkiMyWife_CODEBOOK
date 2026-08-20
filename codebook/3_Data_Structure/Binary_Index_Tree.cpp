struct Binary_Index_Tree {
  int bit[MAXN + 1], lazy[MAXN + 1], n;
  int lb(int x) { return x & -x; }
  void add(int *t, int x, int v) {
    for (; x <= n; x += lb(x)) t[x] += v;
  }
  int sum(int *t, int x) {
    int r = 0;
    for (; x; x -= lb(x)) r += t[x];
    return r;
  }
  void range_add(int l, int r, int v) {
    add(bit, l, v), add(bit, r + 1, -v);
    add(lazy, l, v * (l - 1)), add(lazy, r + 1, -v * r);
  }
  void init(int _n, int *data) {
    n = _n;
    fill_n(bit, n + 1, 0), fill_n(lazy, n + 1, 0);
    for (int i = 1; i <= n; ++i) range_add(i, i, data[i]);
  }
  void suf_modify(int x, int v) {
    range_add(x, n, v);
  }
  void modify(int x, int v) {
    range_add(x, x, v);
  }
  int query(int x) {
    return sum(bit, x) * x - sum(lazy, x);
  }
};
