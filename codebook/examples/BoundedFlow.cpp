// Include Dinic first; MAXN >= n+3.
static BoundedFlow bf; bf.init(n);
bf.add_edge(u, v, lo, hi); // lower bound lo
bool feasible = bf.solve(); // Circulation; solve once.
// Or bf.solve(s, t): max nonnegative flow; -1 on failure.
