static KM km; km.init(n); // square; missing edge = -INF
km.add_edge(i, j, w); // min: use -w
ll best = km.solve();
// km.fl[j] = left node matched to right j
