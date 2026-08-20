bf.init(n);
bf.add_edge(u, v, lo, hi); // lower bound lo
if (!bf.solve()) puts("infeasible");
int f = bf.solve(s, t); // -1 if infeasible
