sw.init(n); // undirected, adj matrix
sw.addEdge(u, v, w); // weights accumulate
int cut = sw.solve(n); // global min cut
