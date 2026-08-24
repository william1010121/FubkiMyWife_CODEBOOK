struct L {
  ll m, k, id;
  L() : id(-1) {}
  L(ll a, ll b, ll c) : m(a), k(b), id(c) {}
  ll at(ll x) { return m * x + k; }
};
class LiChao { // maintain max
  int n; vector<L> nodes;
  void insert(int l, int r, int rt, L ln) {
    int m = (l + r) >> 1;
    L &o = nodes[rt];
    if (o.id == -1) return o = ln, void();
    bool a = o.at(l) < ln.at(l);
    if (o.at(m) < ln.at(m)) a ^= 1, swap(o, ln);
    if (r - l == 1) return;
    if (a) insert(l, m, rt << 1, ln);
    else insert(m, r, rt << 1 | 1, ln);
  }
  ll query(int l, int r, int rt, ll x) {
    int m = (l + r) >> 1; ll ret = -INF;
    if (nodes[rt].id != -1) ret = nodes[rt].at(x);
    if (r - l == 1) return ret;
    if (x < m) return max(ret,
      query(l, m, rt << 1, x));
    return max(ret, query(m, r, rt << 1 | 1, x));
  } public:
  LiChao(int n_) : n(n_ > 0 ? n_ : 0), nodes(n * 4) {}
  // An empty x-domain is a valid empty container: inserts are ignored and
  // queries return the same sentinel as an otherwise empty tree.
  void insert(L ln) { if (n > 0) insert(0, n, 1, ln); }
  ll query(ll x) { return n > 0 ? query(0, n, 1, x) : -INF; }
};
