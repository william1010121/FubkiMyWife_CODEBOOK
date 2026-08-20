Dinic.init(n); // build graph in Dinic first
GomoryHu(n); // g[i] = parent of i in tree
// mincut(u,v) = min edge on tree path u-v
