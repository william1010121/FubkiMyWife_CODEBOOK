WeightGraph g(n); // 1-based
g.add_edge(u, v, w);
auto [sum, cnt] = g.solve();
// total weight, number of matched pairs
