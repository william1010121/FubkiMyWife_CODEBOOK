// Requires MaxFlow (Dinic.cpp); MAXN >= vertex count + 3.
struct BoundedFlow : MaxFlow { // 0-base, solve mutates the graph
  int vn, cnt[MAXN];
  using MaxFlow::add_edge; // ordinary zero-lower-bound edge
  void init(int _n) {
    vn = _n; MaxFlow::init(vn + 3);
    fill_n(cnt, vn, 0);
  }
  void add_edge(int u, int v, int lo, int hi) {
    cnt[u] -= lo; cnt[v] += lo;
    int id = G[u].size();
    MaxFlow::add_edge(u, v, hi);
    G[u][id].flow = lo;
  }
  bool solve() {
    int sum = 0;
    for (int i = 0; i < vn; ++i)
      if (cnt[i] > 0)
        add_edge(vn + 1, i, cnt[i]), sum += cnt[i];
      else if (cnt[i] < 0) add_edge(i, vn + 2, -cnt[i]);
    if (sum != maxflow(vn + 1, vn + 2)) sum = -1;
    for (int i = 0; i < vn; ++i)
      if (cnt[i] > 0)
        G[vn + 1].pop_back(), G[i].pop_back();
      else if (cnt[i] < 0)
        G[i].pop_back(), G[vn + 2].pop_back();
    return sum != -1;
  }
  int solve(int _s, int _t) {
    // Same terminals: circulation feasibility.
    if (_s == _t) return solve() ? 0 : -1;
    add_edge(_t, _s, INF);
    if (!solve()) return -1;
    int fake = SZ(G[_t]) - 1, rev = G[_t][fake].rev;
    int x = G[_t][fake].flow;
    G[_t][fake].cap = G[_t][fake].flow;
    G[_s][rev].cap = G[_s][rev].flow;
    x += maxflow(_s, _t);
    return G[_t].pop_back(), G[_s].pop_back(), x;
  }
};
