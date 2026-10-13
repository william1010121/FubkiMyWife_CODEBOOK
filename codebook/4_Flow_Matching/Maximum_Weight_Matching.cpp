struct WeightGraph { // Loopless undirected; 1-base; O(n^3)
  // n>=0; w>0 only; all int arithmetic (incl. 2*w) fits.
  struct edge { int u, v, w; }; int n, nx;
  vector<int> lab, slk, match, st, pa, S, vis;
  vector<vector<edge>> g;
  vector<vector<int>> flo, flo_from; queue<int> q;
  WeightGraph(int n_) : n(n_), nx(n * 2), lab(nx + 1),
    slk(nx + 1), g(nx + 1, vector<edge>(nx + 1)),
    flo(nx + 1), flo_from(nx + 1, vector(n + 1, 0)) {
    match = st = pa = S = vis = slk;
    for (int u = 1; u <= n; ++u)
      for (int v = 1; v <= n; ++v)
        g[u][v] = {u, v, 0};
  }
  int E(edge e) {
    return lab[e.u] + lab[e.v] - g[e.u][e.v].w * 2; }
  void update_slk(int u, int x, int &s) {
    if (!s || E(g[u][x]) < E(g[s][x])) s = u; }
  void set_slk(int x) {
    slk[x] = 0;
    for (int u = 1; u <= n; ++u)
      if (g[u][x].w > 0 && st[u] != x && S[st[u]] == 0)
        update_slk(u, x, slk[x]);
  }
  void q_push(int x) {
    if (x <= n) q.push(x);
    else for (int y : flo[x]) q_push(y); }
  void set_st(int x, int b) {
    st[x] = b;
    if (x > n) for (int y : flo[x]) set_st(y, b); }
  vector<int> split_flo(vector<int> &f, int xr) {
    auto it = find(ALL(f), xr);
    if (auto pr = it - f.begin(); pr % 2 == 1)
      reverse(1 + ALL(f)), it = f.end() - pr;
    auto res = vector(f.begin(), it);
    return f.erase(f.begin(), it), res;
  }
  void set_match(int u, int v) {
    match[u] = g[u][v].v;
    if (u <= n) return;
    int xr = flo_from[u][g[u][v].u];
    auto &f = flo[u], z = split_flo(f, xr);
    for (int i = 0; i < SZ(z); ++i)
      set_match(z[i], z[i ^ 1]);
    set_match(xr, v); f.insert(f.end(), ALL(z));
  }
  void augment(int u, int v) {
    for (;;) {
      int xnv = st[match[u]]; set_match(u, v);
      if (!xnv) return;
      set_match(v = xnv, u = st[pa[xnv]]); }
  }
  int lca(int u, int v) {
    static int t = 0;
    for (++t; u || v; swap(u, v)) if (u) {
      if (vis[u] == t) return u;
      vis[u] = t, u = st[match[u]];
      if (u) u = st[pa[u]];
    } return 0;
  }
  void add_blossom(int u, int o, int v) {
    int b = find(n + 1 + ALL(st), 0) - begin(st);
    lab[b] = 0, S[b] = 0, match[b] = match[o];
    auto &f = flo[b]; f.clear(); f.pb(o);
    for (int t : {u, v}) {
      reverse(1 + ALL(f));
      for (int x = t, y; x != o; x = st[pa[y]])
        f.pb(x), f.pb(y = st[match[x]]), q_push(y);
    } set_st(b, b);
    for (int x = 1; x <= nx; ++x)
      g[b][x].w = g[x][b].w = 0;
    fill(ALL(flo_from[b]), 0);
    for (int xs : flo[b]) {
      for (int x = 1; x <= nx; ++x)
        if (g[b][x].w == 0 || E(g[xs][x]) < E(g[b][x]))
          g[b][x] = g[xs][x], g[x][b] = g[x][xs];
      for (int x = 1; x <= n; ++x)
        if (flo_from[xs][x]) flo_from[b][x] = xs;
    } set_slk(b);
  }
  void expand_blossom(int b) {
    for (int x : flo[b]) set_st(x, x);
    int xr = flo_from[b][g[b][pa[b]].u], xs = -1;
    for (int x : split_flo(flo[b], xr)) {
      if (xs == -1) { xs = x; continue; }
      pa[xs] = g[x][xs].u, S[xs] = 1, S[x] = 0;
      slk[xs] = 0, set_slk(x), q_push(x), xs = -1;
    } for (int x : flo[b])
      if (x == xr) S[x] = 1, pa[x] = pa[b];
      else S[x] = -1, set_slk(x);
    st[b] = 0;
  }
  bool on_found_edge(const edge &e) {
    if (int u = st[e.u], v = st[e.v]; S[v] == -1) {
      int nu = st[match[v]]; pa[v] = e.u, S[v] = 1;
      slk[v] = slk[nu] = S[nu] = 0; q_push(nu);
    } else if (S[v] == 0) {
      if (int o = lca(u, v)) add_blossom(u, o, v);
      else return augment(u, v), augment(v, u), true;
    } return false;
  }
  bool matching() {
    fill(ALL(S), -1), fill(ALL(slk), 0); q = {};
    for (int x = 1; x <= nx; ++x)
      if (st[x] == x && !match[x])
        pa[x] = S[x] = 0, q_push(x);
    if (q.empty()) return false;
    for (;;) {
      while (!q.empty()) {
        int u = q.front(); q.pop();
        if (S[st[u]] == 1) continue;
        for (int v = 1; v <= n; ++v)
          if (g[u][v].w > 0 && st[u] != st[v]) {
            if (E(g[u][v]))
              update_slk(u, st[v], slk[st[v]]);
            else if (on_found_edge(g[u][v])) return true;
          }
      } int d = 0x3f3f3f3f;
      for (int b = n + 1; b <= nx; ++b)
        if (st[b] == b && S[b] == 1)
          d = min(d, lab[b] / 2);
      for (int x = 1; x <= nx; ++x)
        if (int s = slk[x]; st[x] == x && s && S[x] <= 0)
          d = min(d, E(g[s][x]) / (S[x] + 2));
      for (int u = 1; u <= n; ++u)
        if (S[st[u]] == 1) lab[u] += d;
        else if (S[st[u]] == 0) {
          if (lab[u] <= d) return false;
          lab[u] -= d;
        }
      for (int b = n + 1; b <= nx; ++b)
        if (st[b] == b && S[b] >= 0)
          lab[b] += d * (2 - 4 * S[b]);
      for (int x = 1; x <= nx; ++x)
        if (int s = slk[x]; st[x] == x && s && st[s] != x
            && E(g[s][x]) == 0 && on_found_edge(g[s][x]))
          return true;
      for (int b = n + 1; b <= nx; ++b)
        if (st[b] == b && S[b] == 1 && lab[b] == 0)
          expand_blossom(b);
    }
  }
  pair<ll, int> solve() { // Rebuild; ties: any pair count.
    fill(ALL(match), 0), fill(ALL(st), 0); // Reset.
    for (int u = 0; u <= n; ++u)
      st[u] = flo_from[u][u] = u;
    int w_max = 0, n_matches = 0; ll tot_weight = 0;
    for (int u = 1; u <= n; ++u)
      for (int v = 1; v <= u; ++v)
        w_max = max(w_max, g[u][v].w);
    fill(ALL(lab), w_max); // Tight seeds; n/2 are optimal.
    for (int u = 1; u <= n; ++u) if (w_max && !match[u])
      for (int v = u + 1; v <= n; ++v)
        if (!match[v] && g[u][v].w == w_max) {
          match[u] = v, match[v] = u, ++n_matches; break; }
    if (n_matches < n / 2) while (matching()) ++n_matches;
    for (int u = 1; u <= n; ++u)
      if (match[u] && match[u] < u)
        tot_weight += g[u][match[u]].w;
    return {tot_weight, n_matches};
  }
  void add_edge(int u, int v, int w) {
    g[u][v].w = g[v][u].w = w; }
};
