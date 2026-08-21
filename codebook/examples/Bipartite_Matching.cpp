static Bipartite_Matching bm; bm.init(nl, nr); // left 0..nl-1
bm.add_edge(l, r);
int m = bm.matching();
// pair (i, bm.mp[i]) when bm.mp[i] != -1
