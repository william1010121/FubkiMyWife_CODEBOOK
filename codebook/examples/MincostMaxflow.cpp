static MinCostMaxFlow mcmf; mcmf.init(n);
mcmf.add_edge(u, v, cap, cost);
ll flow, cost;
mcmf.solve(s, t, flow, cost); // SPFA + Johnson + Dijkstra
// s!=t: no negative residual cycles anywhere.
// Additional flow/cost per call; s==t returns both zero.
// neg=false skips SPFA only if active residual costs >=0.
