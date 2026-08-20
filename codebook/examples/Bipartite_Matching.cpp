BM.init(nl, nr); // left 0..nl-1
BM.add_edge(l, r);
int m = BM.matching();
// pair (i, BM.mp[i]) when mp[i] != -1
