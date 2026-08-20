flow.init(n); // vertices are 0..n-1; the template uses flow.s=n+1, flow.t=n+2
flow.addEdge(u, v, c);
int max_flow = flow.solve();
