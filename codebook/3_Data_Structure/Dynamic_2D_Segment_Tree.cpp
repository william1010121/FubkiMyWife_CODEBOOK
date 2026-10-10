// A: Seg2D -- rectangle ADD + rectangle SUM, initially all zero.
// 0-based, half-open rectangles [xl,xr) x [yl,yr).
// O(log n log m) per operation. int sums/products may overflow.
// ---------- 1D sum tree ----------
struct Seg {
    struct Node {
        int sum = 0, lazy = 0;
        Node *l = nullptr, *r = nullptr;
    };
    int n;
    Node *root = nullptr;
    Seg(int n = 0) : n(n) {}
    void apply(Node *&idx, int l, int r, int v) {
        if (!idx) idx = new Node();
        idx->sum += v * (r - l);
        idx->lazy += v;
    }
    void push(Node *idx, int l, int r) {
        if (!idx || !idx->lazy || r - l == 1) return;
        int m = (l + r) / 2;
        apply(idx->l, l, m, idx->lazy);
        apply(idx->r, m, r, idx->lazy);
        idx->lazy = 0;
    }
    void update(Node *&idx, int l, int r, int ql, int qr, int v) {
        if (qr <= l || r <= ql) return;
        if (!idx) idx = new Node();
        if (ql <= l && r <= qr) {
            apply(idx, l, r, v);
            return;
        }
        push(idx, l, r);
        int m = (l + r) / 2;
        update(idx->l, l, m, ql, qr, v);
        update(idx->r, m, r, ql, qr, v);
        idx->sum =
            (idx->l ? idx->l->sum : 0) +
            (idx->r ? idx->r->sum : 0);
    }
    int query(Node *idx, int l, int r, int ql, int qr) {
        if (!idx || qr <= l || r <= ql) return 0;
        if (ql <= l && r <= qr) return idx->sum;
        push(idx, l, r);
        int m = (l + r) / 2;
        return query(idx->l, l, m, ql, qr)
             + query(idx->r, m, r, ql, qr);
    }
    void update(int l, int r, int v) {
        update(root, 0, n, l, r, v);
    }
    int query(int l, int r) {
        return query(root, 0, n, l, r);
    }
};
// ---------- 2D sum tree ----------
struct Seg2D {
    struct Node {
        Seg sum, lazy;
        Node *l = nullptr, *r = nullptr;
        Node(int m) : sum(m), lazy(m) {}
    };
    int n, m;
    Node *root = nullptr;
    Seg2D(int n, int m) : n(n), m(m) {}
    void update(Node *&idx, int l, int r,
                int xl, int xr, int yl, int yr, int v) {
        if (xr <= l || r <= xl) return;
        if (!idx) idx = new Node(m);
        int rows = min(r, xr) - max(l, xl);
        // sum includes all updates intersecting this x interval.
        idx->sum.update(yl, yr, v * rows);
        if (xl <= l && r <= xr) {
            // lazy records updates covering this entire x interval.
            idx->lazy.update(yl, yr, v);
            return;
        }
        int md = (l + r) / 2;
        update(idx->l, l, md, xl, xr, yl, yr, v);
        update(idx->r, md, r, xl, xr, yl, yr, v);
    }
    int query(Node *idx, int l, int r,
              int xl, int xr, int yl, int yr) {
        if (!idx || xr <= l || r <= xl) return 0;
        if (xl <= l && r <= xr)
            return idx->sum.query(yl, yr);
        int rows = min(r, xr) - max(l, xl);
        int md = (l + r) / 2;
        // Children do not include this node's lazy; add it separately.
        return idx->lazy.query(yl, yr) * rows
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
