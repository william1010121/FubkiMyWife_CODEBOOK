Dinic.init(n); // build graph in Dinic first
GomoryHu(n); // g[i] = parent, gh_w[i] = edge weight to g[i]
// mincut(u,v) = min gh_w[x] on the tree path u-v
