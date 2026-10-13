// Init 0<=n<=N; IDs [0,n); cap>=0.
// s!=t: no negative residual cycles anywhere.
// INF>0; ll math fits; finite BF/reduced distances < INF.
// A augmentations: O(VE + A(E+V)log(E+V)).
struct MinCostMaxFlow {
  struct Edge { int from, to; ll cap, flow, cost;
    int rev; } *past[N];
  vector<Edge> G[N];
  bool inq[N]; int n, s, t;
  ll dis[N], up[N], pot[N];
  bool BellmanFord() {
    fill_n(dis, n, INF), fill_n(inq, n, 0); queue<int> q;
    dis[s] = 0, up[s] = LLONG_MAX, past[s] = nullptr;
    q.push(s), inq[s] = 1;
    while (!q.empty()) {
      int u = q.front(); q.pop(), inq[u] = 0;
      for (auto &e : G[u])
        if (e.cap > e.flow && dis[e.to] > dis[u]+e.cost) {
          dis[e.to] = dis[u] + e.cost, past[e.to] = &e;
          up[e.to] = min(up[u], e.cap - e.flow);
          if (!inq[e.to]) q.push(e.to), inq[e.to] = 1;
        }
    } return dis[t] != INF;
  }
  bool Dijkstra() {
    fill_n(dis, n, INF); using State = pair<ll, int>;
    priority_queue<State, vector<State>, greater<State>> q;
    dis[s] = 0, up[s] = LLONG_MAX, past[s] = nullptr;
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
    } return dis[t] != INF;
  }
  void solve(int _s, int _t, ll &flow, ll &cost,
      bool neg = true) {
    s = _s, t = _t, flow = cost = 0; // added flow/cost
    if (s == t) return; // zero-length flow
    // Rebuild potentials each call, including neg=false.
    fill_n(pot, n, 0); bool need_bf = neg;
    if (!neg) for (int u = 0; u < n; ++u)
      for (auto &e : G[u])
        need_bf |= e.cap > e.flow && e.cost < 0;
    if (need_bf ? !BellmanFord() : !Dijkstra()) return;
    do { // Reuse the initial shortest path.
      flow += up[t];
      cost += up[t] * (dis[t] + pot[t] - pot[s]);
      for (int i = 0; i < n; ++i)
        if (dis[i] != INF) pot[i] += dis[i];
      for (int i = t; past[i]; i = past[i]->from) {
        auto &e = *past[i]; e.flow += up[t];
        G[e.to][e.rev].flow -= up[t];
      }
    } while (Dijkstra());
  }
  void init(int _n) {
    n = _n, fill_n(pot, n, 0);
    for (int i = 0; i < n; ++i) G[i].clear();
  }
  void add_edge(ll a, ll b, ll cap, ll cost) {
    G[a].pb({int(a), int(b), cap, 0, cost,
      SZ(G[b]) + (a == b)});
    G[b].pb({int(b), int(a), 0, 0, -cost, SZ(G[a]) - 1});
  }
};
