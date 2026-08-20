hld.init(n); hld.add_edge(a, b);
hld.val[u] = w; hld.build(); // 1-base
hld.query(a, b); // O(log^2 n)
// skeleton: wire a segtree into build/query
