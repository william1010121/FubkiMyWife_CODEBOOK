link(x, y); cut(x, y);
int s = query(x, y); // path XOR
change(x, val); bool c = conn(x, y);
Splay *r = get_root(x), *l = lca(x, y);
