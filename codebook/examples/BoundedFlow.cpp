static BoundedFlow bf; bf.init(n);
bf.add_edge(u, v, lo, hi); // lower bound lo
bool feasible = bf.solve(); // circulation
// or: int f = bf.solve(s, t); // bounded max flow
