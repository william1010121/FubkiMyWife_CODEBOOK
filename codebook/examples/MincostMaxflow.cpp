static MinCostMaxFlow mcmf; mcmf.init(n);
mcmf.add_edge(u, v, cap, cost);
ll flow, cost;
mcmf.solve(s, t, flow, cost); // SPFA, then Dijkstra with potentials
// For s != t: no negative residual cycles, even in unreachable parts.
// Each call returns additional flow/cost; s == t returns both as zero.
// neg=false skips SPFA only if all active residual costs are nonnegative.
