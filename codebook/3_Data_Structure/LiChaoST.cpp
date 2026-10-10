// Max on integer x in [0,n); O(log n); inserted id != -1.
// Caller INF > 0; values in [-INF,INF]; m*x and m*x+k fit ll.
// For n>0, 4*n fits int/memory; n<=0: insert ignored, query -INF.
struct L {
  ll m, k, id; L() : id(-1) {}
  L(ll a, ll b, ll c) : m(a), k(b), id(c) {}
  ll at(ll x) { return m * x + k; }
};
class LiChao {
  int n; vector<L> nodes;
  void insert(int l, int r, int rt, L ln) {
    int m = (l + r) >> 1; L &o = nodes[rt];
    if (o.id == -1) return o = ln, void();
    if (o.at(m) < ln.at(m)) swap(o, ln);
    if (r - l == 1 || o.m == ln.m) return;
    if (ln.m < o.m) { // can win only left
      if (ln.at(l) > o.at(l)) insert(l, m, rt << 1, ln);
    } else if (ln.at(r - 1) > o.at(r - 1))
      insert(m, r, rt << 1 | 1, ln);
  }
  ll query(int l, int r, int rt, ll x) {
    if (nodes[rt].id == -1) return -INF;
    int m = (l + r) >> 1; ll ret = nodes[rt].at(x);
    if (r - l == 1) return ret;
    return max(ret, x < m ? query(l, m, rt << 1, x)
                         : query(m, r, rt << 1 | 1, x));
  } public:
  LiChao(int n_) : n(max(n_, 0)), nodes(n * 4) {}
  void insert(L ln) { if (n) insert(0, n, 1, ln); }
  ll query(ll x) { return n ? query(0, n, 1, x) : -INF; }
};
