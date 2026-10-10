static KM km; km.init(n); // square; perfect matching
km.add_edge(i, j, w); // min: use -w; missing=-INF
ll best = km.solve();
// km.fl: left -> right; km.fr: right -> left
