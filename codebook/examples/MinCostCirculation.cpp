mcmf.init(n); // negative costs allowed
mcmf.add_edge(u, v, cap, cost);
mcmf.solve(mxlg); // once after building the graph
// mxlg = highest set bit of max capacity (0 if all caps are zero)
