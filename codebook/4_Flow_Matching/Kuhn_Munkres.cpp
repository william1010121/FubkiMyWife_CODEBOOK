struct KM { // Max weight; needs perfect matching, n <= N. O(n^3)
  // 0-base; missing = -INF; INF > 2*n*max|real w|; arithmetic fits ll.
  ll w[N][N], hl[N], hr[N], slk[N];
  int fl[N], fr[N], pre[N], qu[N], ql, qr, n;
  bool vl[N], vr[N];
  void init(int _n) {
    n = _n; for (int i = 0; i < n; ++i) fill_n(w[i], n, -INF);
  }
  void add_edge(int a, int b, ll wei) { w[a][b] = wei; }
  bool Check(int y) { // y is right; queue stores left vertices.
    if (vr[y] = 1, ~fr[y]) return vl[qu[qr++] = fr[y]] = 1;
    while (~y) swap(y, fl[fr[y] = pre[y]]); return 0;
  }
  void bfs(int s) { // unmatched left root
    fill_n(slk, n, INF), fill_n(vl, n, 0), fill_n(vr, n, 0);
    ql = qr = 0, qu[qr++] = s, vl[s] = 1;
    for (ll d;;) {
      while (ql < qr)
        for (int y = 0, x = qu[ql++]; y < n; ++y)
          if (!vr[y] && slk[y] >= (d = hl[x] + hr[y] - w[x][y])) {
            if (pre[y] = x, d) slk[y] = d;
            else if (!Check(y)) return;
          } d = INF;
      for (int x = 0; x < n; ++x) if (!vr[x]) d = min(d, slk[x]);
      for (int x = 0; x < n; ++x) {
        if (vr[x]) hr[x] += d;
        else slk[x] -= d;
        if (vl[x]) hl[x] -= d;
      } for (int x = 0; x < n; ++x)
        if (!vr[x] && !slk[x] && !Check(x)) return;
    }
  }
  ll solve() {
    fill_n(fl, n, -1), fill_n(fr, n, -1), fill_n(hl, n, 0);
    fill_n(hr, n, -INF);
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
      hr[j] = max(hr[j], w[i][j]);
    for (int i = 0; i < n; ++i) bfs(i);
    ll res = 0; for (int i = 0; i < n; ++i) res += w[i][fl[i]];
    return res;
  }
};
