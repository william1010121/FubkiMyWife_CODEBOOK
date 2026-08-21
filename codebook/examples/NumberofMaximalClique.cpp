static BronKerbosch bk; bk.init(n); bk.add_edge(u, v); // 1-base
int c = bk.solve();
// full number of maximal cliques
