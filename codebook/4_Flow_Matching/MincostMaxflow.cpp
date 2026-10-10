struct MinCostMaxFlow { // 0-base
  struct Edge
  { ll from, to, cap, flow, cost, rev; } *past[N];
  vector<Edge> G[N];
  int inq[N], n, s, t;
  ll dis[N], up[N], pot[N];
  bool BellmanFord() {
    fill_n(dis, n, INF), fill_n(inq, n, 0);
    queue<int> q;
    auto relax = [&](int u, ll d, ll cap, Edge *e) {
      if (cap > 0 && dis[u] > d) {
        dis[u] = d, up[u] = cap, past[u] = e;
        if (!inq[u]) inq[u] = 1, q.push(u);
      }
    }; relax(s, 0, INF, 0);
    while (!q.empty()) {
      int u = q.front(); q.pop(), inq[u] = 0;
      for (auto &e : G[u]) if (e.cap > e.flow)
        relax(e.to, dis[u] + e.cost,
          min(up[u], e.cap - e.flow), &e);
    } return dis[t] != INF;
  }
  bool Dijkstra() {
    fill_n(dis, n, INF);
    using State = pair<ll, int>;
    priority_queue<State, vector<State>, greater<State>> q;
    dis[s] = 0, up[s] = INF, past[s] = nullptr;
    q.push({0, s});
    while (!q.empty()) {
      auto [d, u] = q.top(); q.pop();
      if (d != dis[u]) continue;
      for (auto &e : G[u]) if (e.cap > e.flow) {
        ll nd = d + e.cost + pot[u] - pot[e.to];
        if (nd < dis[e.to]) {
          dis[e.to] = nd, past[e.to] = &e;
          up[e.to] = min(up[u], e.cap - e.flow);
          q.push({nd, e.to});
        }
      }
    }
    return dis[t] != INF;
  }
  void solve(int _s, int _t, ll &flow, ll &cost,
      bool neg = true) {
    s = _s, t = _t, flow = 0, cost = 0;
    // A zero-length s-t flow has zero cost.  Besides defining the API for this degenerate query, this avoids Bellman-Ford on a self-terminal residual graph (which can contain a source-reachable negative cycle).
    if (_s == _t) return;
    // Rebuild potentials for this source, including on repeated solve calls.
    fill_n(pot, n, 0);
    bool need_bf = neg;
    if (!need_bf) for (int u = 0; u < n; ++u)
      for (const auto &e : G[u])
        if (e.cap > e.flow && e.cost < 0) need_bf = true;
    if (need_bf) {
      if (!BellmanFord()) return;
      for (int i = 0; i < n; ++i)
        if (dis[i] != INF) pot[i] = dis[i];
    }
    while (Dijkstra()) {
      flow += up[t];
      cost += up[t] * (dis[t] + pot[t] - pot[s]);
      for (int i = 0; i < n; ++i)
        if (dis[i] != INF) pot[i] += dis[i];
      for (int i = t; past[i]; i = past[i]->from) {
        auto &e = *past[i];
        e.flow += up[t], G[e.to][e.rev].flow -= up[t];
      }
    }
  }
  void init(int _n) {
    n = _n, fill_n(pot, n, 0);
    for (int i = 0; i < n; ++i) G[i].clear();
  }
  void add_edge(ll a, ll b, ll cap, ll cost) {
    G[a].pb({a, b, cap, 0, cost, SZ(G[b]) + (a == b)}
      ), G[b].pb({b, a, 0, 0, -cost, SZ(G[a]) - 1});
  }
};
