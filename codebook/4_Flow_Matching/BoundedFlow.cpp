// Needs MaxFlow (Dinic.cpp); MAXN >= n+3; 0 <= lo <= hi.
// int math fits; INF >= capacity and total positive cnt.
struct BoundedFlow : MaxFlow { // 0-base; n>=0; solve once
  int vn, cnt[MAXN];
  void add_edge(int u, int v, int cap) { // lower bound 0
    int id = SZ(G[u]); MaxFlow::add_edge(u, v, cap);
    G[u][id].rev += u == v; // Fix self-loop reverse index.
  }
  void init(int _n) {
    vn = _n, MaxFlow::init(vn + 3); fill_n(cnt, vn, 0);
  }
  void add_edge(int u, int v, int lo, int hi) {
    cnt[u] -= lo, cnt[v] += lo; int id = SZ(G[u]);
    add_edge(u, v, hi); G[u][id].flow = lo;
  }
  bool solve() { // Mutates graph, even on failure.
    int sum = 0;
    for (int i = 0; i < vn; ++i)
      if (cnt[i] > 0)
        add_edge(vn + 1, i, cnt[i]), sum += cnt[i];
      else if (cnt[i] < 0) add_edge(i, vn + 2, -cnt[i]);
    if (sum != maxflow(vn + 1, vn + 2)) sum = -1;
    for (int i = 0; i < vn; ++i)
      if (cnt[i]) G[i].pop_back();
    G[vn + 1].clear(), G[vn + 2].clear(); // Demand arcs.
    return sum != -1;
  }
  // Max nonnegative flow; -1 if infeasible.
  int solve(int _s, int _t) {
    if (_s == _t) return solve() ? 0 : -1; // Circulation.
    add_edge(_t, _s, INF);
    if (!solve()) return -1;
    int x = G[_t].back().flow; // Drop balancing arc.
    G[_t].pop_back(), G[_s].pop_back();
    return x + maxflow(_s, _t);
  }
};
