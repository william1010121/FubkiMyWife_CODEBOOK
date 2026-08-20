Poly_t f{1, 2, 3}, g{4, 5};
auto h = f.Mul(g), fi = f.Inv(); // f[0]!=0
auto q = f.Ln(); // f[0]==1
auto e = g.Exp(); // g[0]==0
auto y = f.Eval({1, 2, 3}); // multipoint
