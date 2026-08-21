static dominator_tree dt; dt.init(n); dt.add_edge(u, v); // 1-base
dt.tarjan(root);
dt.tree[u]; // children in dominator tree
