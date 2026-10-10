static KM km; km.init(n); // square, perfect matching; missing = -INF
km.add_edge(i, j, w); // min: use -w
ll best = km.solve();
// km.fl[i] = right matched to left i; km.fr[j] = left matched to right j
