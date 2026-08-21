static Clique_Cover cc; cc.init(n); cc.add_edge(u, v);
int k = cc.solve(); // O(n 2^n)
// probabilistic; needs fwt()
