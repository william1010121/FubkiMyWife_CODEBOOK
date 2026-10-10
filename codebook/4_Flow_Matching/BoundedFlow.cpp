// Requires MaxFlow (Dinic.cpp); MAXN >= n+3, 0 <= lo <= hi.
// Arithmetic fits int; INF >= max capacity and sum of positive cnt.
struct BoundedFlow : MaxFlow { // 0-base; n>=0; solve once, mutates graph
  int vn, cnt[MAXN];
  void add_edge(int u, int v, int cap) { // zero lower bound
    int id = SZ(G[u]); MaxFlow::add_edge(u, v, cap);
    G[u][id].rev += u == v; // correct Dinic's self-loop index
  }
  void init(int _n) {
    vn = _n, MaxFlow::init(vn + 3);
    fill_n(cnt, vn, 0);
  }
  void add_edge(int u, int v, int lo, int hi) {
    cnt[u] -= lo, cnt[v] += lo; int id = SZ(G[u]);
    add_edge(u, v, hi);
    G[u][id].flow = lo;
  }
  bool solve() {
    int sum = 0;
    for (int i = 0; i < vn; ++i)
      if (cnt[i] > 0) add_edge(vn + 1, i, cnt[i]), sum += cnt[i];
      else if (cnt[i] < 0) add_edge(i, vn + 2, -cnt[i]);
    if (sum != maxflow(vn + 1, vn + 2)) sum = -1;
    for (int i = 0; i < vn; ++i)
      if (cnt[i] > 0) G[vn + 1].pop_back(), G[i].pop_back();
      else if (cnt[i] < 0)
        G[i].pop_back(), G[vn + 2].pop_back();
    return sum != -1;
  }
  int solve(int _s, int _t) { // max nonnegative flow; -1 if infeasible
    if (_s == _t) return solve() ? 0 : -1; // circulation
    add_edge(_t, _s, INF);
    if (!solve()) return -1;
    int x = G[_t].back().flow; // remove the balancing edge
    G[_t].pop_back(), G[_s].pop_back();
    return x + maxflow(_s, _t);
  }
};
