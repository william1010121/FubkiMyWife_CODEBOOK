static MaxClique mc; mc.init(n); mc.add_edge(u, v);
int k = mc.solve(); // sol[0..k) = clique
// fast for n <= 100
