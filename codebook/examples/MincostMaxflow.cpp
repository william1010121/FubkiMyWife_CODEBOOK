static MinCostMaxFlow mcmf; mcmf.init(n);
mcmf.add_edge(u, v, cap, cost);
ll flow, cost;
mcmf.solve(s, t, flow, cost); // initial SPFA, then Dijkstra with potentials
// No negative cycle reachable from s in the residual graph.
// Each call returns additional flow/cost; s == t returns both as zero.
// neg=false skips initial SPFA only when all active residual costs are nonnegative.
