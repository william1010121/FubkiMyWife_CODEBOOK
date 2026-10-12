Euler e(n, true); // false: undirected
e.add_edge(u, v);
auto p = e.solve(s); // {} if no Euler walk from s
// circuit: start anywhere with an edge; path: start at
// out-in=1 vertex (directed) or an odd vertex (undirected)
