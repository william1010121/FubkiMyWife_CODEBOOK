mcmf.init(n); // negative costs allowed
mcmf.add_edge(u, v, cap, cost);
mcmf.solve(mxlg); // once after building the graph
// mxlg = max capacity's highest set bit; 0 if all zero
