WeightGraph g(n); // 1-based
g.g[u][v].w = g.g[v][u].w = w;
auto [sum, cnt] = g.solve();
// total weight, number of matched pairs
