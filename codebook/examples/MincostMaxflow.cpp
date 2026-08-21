static MinCostMaxFlow mcmf; mcmf.init(n);
mcmf.add_edge(u, v, cap, cost);
ll flow, cost;
mcmf.solve(s, t, flow, cost); // neg=true handles negative edges
