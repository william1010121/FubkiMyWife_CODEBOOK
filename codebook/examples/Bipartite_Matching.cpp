static Bipartite_Matching bm; bm.init(nl, nr); // 0-base
bm.add_edge(l, r);
int m = bm.matching();
// pair (i, bm.mp[i]) when bm.mp[i] != -1
