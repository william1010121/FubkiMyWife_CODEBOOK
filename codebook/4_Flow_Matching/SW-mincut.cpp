struct SW { // 0-base, undirected, nonnegative weights. O(V^3)
  // 0<=n<=MXN; all cut sums fit int; loops ignored, solve mutates.
  static const int MXN = 514, INF = 2147483647;
  int vst[MXN], edge[MXN][MXN], wei[MXN];
  void init(int n) { for (int i = 0; i < n; ++i)
    fill_n(edge[i], n, 0); }
  void addEdge(int u, int v, int w) {
    if (u != v) edge[u][v] += w, edge[v][u] += w;
  }
  int search(int &s, int &t, int n) { // n>=1; one phase
    fill_n(vst, n, 0), fill_n(wei, n, 0);
    s = t = -1; int mx, cur;
    for (int j = 0; j < n; ++j) {
      cur = max_element(wei, wei + n) - wei, mx = wei[cur];
      vst[cur] = 1, wei[cur] = -1, s = t, t = cur;
      for (int i = 0; i < n; ++i)
        wei[i] += !vst[i] * edge[cur][i];
    } return mx;
  }
  int solve(int n) {
    int res = n <= 1 ? 0 : INF; // empty cut for n <= 1
    for (int x, y; n > 1; --n) {
      res = min(res, search(x, y, n)); if (!res) break;
      for (int i = 0; i < n; ++i)
        if (i != x && i != y) edge[i][x] = (edge[x][i] += edge[y][i]);
      for (int i = 0; i < n; ++i)
        edge[y][i] = edge[n - 1][i], edge[i][y] = edge[i][n - 1];
      edge[y][y] = 0;
    } return res;
  }
} sw;
