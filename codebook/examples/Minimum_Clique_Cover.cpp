static Clique_Cover cc; cc.init(n); cc.add_edge(u, v);
int k = cc.solve(); // O(n 2^n)
// Mod 2^32 may overestimate; needs OR-zeta fwt().
