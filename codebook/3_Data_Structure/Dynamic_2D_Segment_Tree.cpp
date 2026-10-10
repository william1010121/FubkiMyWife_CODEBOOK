// n,m >= 0; rectangle add/sum, initially zero; 0-base half-open.
// O(log n log m); ranges within domain; int arithmetic must fit.
struct Seg { // 1D range add/sum; query allocates no nodes
  struct Node {
    int sum = 0, lazy = 0;
    Node *l = nullptr, *r = nullptr;
  };
  int n; Node *root = nullptr;
  Seg(int n = 0) : n(n) {}
  void apply(Node *&idx, int l, int r, int v) {
    if (!idx) idx = new Node();
    idx->sum += v * (r - l), idx->lazy += v;
  }
  void push(Node *idx, int l, int r) {
    if (!idx || !idx->lazy || r - l == 1) return;
    int m = l + (r - l) / 2;
    apply(idx->l, l, m, idx->lazy);
    apply(idx->r, m, r, idx->lazy), idx->lazy = 0;
  }
  void update(Node *&idx, int l, int r, int ql, int qr, int v) {
    if (ql >= qr || qr <= l || r <= ql) return;
    if (ql <= l && r <= qr) return apply(idx, l, r, v);
    if (!idx) idx = new Node();
    idx->sum += v * (min(r, qr) - max(l, ql));
    int m = l + (r - l) / 2;
    update(idx->l, l, m, ql, qr, v);
    update(idx->r, m, r, ql, qr, v);
  }
  int query(Node *idx, int l, int r, int ql, int qr) {
    if (!idx || ql >= qr || qr <= l || r <= ql) return 0;
    if (ql <= l && r <= qr) return idx->sum;
    int m = l + (r - l) / 2;
    // Children exclude this lazy; count only the queried overlap.
    return idx->lazy * (min(r, qr) - max(l, ql))
         + query(idx->l, l, m, ql, qr)
         + query(idx->r, m, r, ql, qr);
  }
  void update(int l, int r, int v) { update(root, 0, n, l, r, v); }
  int query(int l, int r) { return query(root, 0, n, l, r); }
};
struct Seg2D {
  struct Node {
    Seg sum, lazy;
    Node *l = nullptr, *r = nullptr;
    Node(int m) : sum(m), lazy(m) {}
  };
  int n, m; Node *root = nullptr;
  Seg2D(int n, int m) : n(n), m(m) {}
  void update(Node *&idx, int l, int r,
              int xl, int xr, int yl, int yr, int v) {
    if (xl >= xr || yl >= yr || xr <= l || r <= xl) return;
    if (!idx) idx = new Node(m);
    // sum counts all updates intersecting this x interval.
    idx->sum.update(yl, yr, v * (min(r, xr) - max(l, xl)));
    if (xl <= l && r <= xr) return idx->lazy.update(yl, yr, v);
    int md = l + (r - l) / 2;
    update(idx->l, l, md, xl, xr, yl, yr, v);
    update(idx->r, md, r, xl, xr, yl, yr, v);
  }
  int query(Node *idx, int l, int r,
            int xl, int xr, int yl, int yr) {
    if (!idx || xl >= xr || yl >= yr || xr <= l || r <= xl) return 0;
    if (xl <= l && r <= xr) return idx->sum.query(yl, yr);
    int md = l + (r - l) / 2;
    // lazy covers this full x interval; children exclude it.
    return idx->lazy.query(yl, yr) * (min(r, xr) - max(l, xl))
         + query(idx->l, l, md, xl, xr, yl, yr)
         + query(idx->r, md, r, xl, xr, yl, yr);
  }
  void update(int xl, int xr, int yl, int yr, int v) {
    update(root, 0, n, xl, xr, yl, yr, v);
  }
  int query(int xl, int xr, int yl, int yr) {
    return query(root, 0, n, xl, xr, yl, yr);
  }
};
