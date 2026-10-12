static dominator_tree dt; dt.init(n);
dt.add_edge(u, v); dt.tarjan(root); // 1-base
dt.tree[u]; // Children in dominator tree
