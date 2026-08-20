MaxFlow Dinic;
int g[MAXN], gh_w[MAXN];
void GomoryHu(int n) { // 0-base
  fill_n(g, n, 0), fill_n(gh_w, n, 0);
  for (int i = 1; i < n; ++i) {
    Dinic.reset(); int p = g[i], cut = Dinic.maxflow(i, p);
    gh_w[i] = cut;
    for (int j = i + 1; j < n; ++j)
      if (g[j] == p && ~Dinic.dis[j])
        g[j] = i;
    if (p && ~Dinic.dis[g[p]])
      g[i] = g[p], g[p] = i, gh_w[i] = gh_w[p], gh_w[p] = cut;
  }
}
