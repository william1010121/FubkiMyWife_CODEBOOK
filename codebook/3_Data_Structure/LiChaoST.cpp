// Max on integer x in [0,n); O(log n). id != -1; m*x+k fits ll.
// INF bounds values. n <= 0: insert ignored, query returns -INF.
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
    bool a = o.at(l) < ln.at(l);
    if (o.at(m) < ln.at(m)) a ^= 1, swap(o, ln);
    if (r - l == 1 || o.m == ln.m) return;
    if (a) insert(l, m, rt << 1, ln);
    else insert(m, r, rt << 1 | 1, ln);
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
