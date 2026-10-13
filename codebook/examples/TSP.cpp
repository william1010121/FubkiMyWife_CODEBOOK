vector<vector<ll>> d(n, vector<ll>(n, INF)); // d[i][j]
auto [c, ord] = tsp(d);        // min tour 0->..->0
auto [c2, ord2] = tsp(d, false); // min Hamiltonian path
