mcmf.init(n); // negative costs allowed
mcmf.add_edge(u, v, cap, cost);
mcmf.solve(mxlg);
// mxlg = highest set bit of max capacity
