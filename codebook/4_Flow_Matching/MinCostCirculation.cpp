struct MinCostCirculation { // 0-base; solve once, O(V(E+1)^2log(C+2))
  // n <= N, cap >= 0; finite distances < INF; arithmetic fits ll.
  struct Edge { int from, to; ll cap, fcap, flow, cost; int rev; };
  Edge *past[N]; vector<Edge> G[N];
  ll dis[N]; bool inq[N]; int n;
  void BellmanFord(int s) {
    fill_n(dis, n, INF), fill_n(inq, n, 0); queue<int> q;
    dis[s] = 0, past[s] = nullptr, q.push(s), inq[s] = 1;
    while (!q.empty()) {
      int u = q.front(); q.pop(), inq[u] = 0;
      for (auto &e : G[u])
        if (e.cap > e.flow && dis[e.to] > dis[u] + e.cost) {
          dis[e.to] = dis[u] + e.cost, past[e.to] = &e;
          if (!inq[e.to]) q.push(e.to), inq[e.to] = 1;
        }
    }
  }
  void try_edge(Edge &cur) {
    if (cur.cap > cur.flow) return ++cur.cap, void();
    BellmanFord(cur.to);
    if (dis[cur.from] != INF && dis[cur.from] + cur.cost < 0) {
      ++cur.flow, --G[cur.to][cur.rev].flow;
      for (int i = cur.from; past[i]; i = past[i]->from) {
        auto &e = *past[i]; ++e.flow, --G[e.to][e.rev].flow;
      }
    } ++cur.cap;
  }
  void solve(int mxlg) { // highest bit of max cap; 0 if all zero
    for (int b = mxlg; b >= 0; --b) {
      for (int i = 0; i < n; ++i)
        for (auto &e : G[i]) e.cap *= 2, e.flow *= 2;
      for (int i = 0; i < n; ++i)
        for (auto &e : G[i]) if (e.fcap >> b & 1) try_edge(e);
    }
  }
  void init(int _n) {
    n = _n; for (int i = 0; i < n; ++i) G[i].clear();
  }
  void add_edge(ll a, ll b, ll cap, ll cost) {
    G[a].pb({int(a), int(b), 0, cap, 0, cost, SZ(G[b]) + (a == b)});
    G[b].pb({int(b), int(a), 0, 0, 0, -cost, SZ(G[a]) - 1});
  }
} mcmf;
