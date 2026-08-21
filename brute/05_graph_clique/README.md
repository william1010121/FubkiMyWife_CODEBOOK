# Clique-template edge-case brute tests

These four independent harnesses cover the four clique templates in
`codebook/2_Graph/`:

- `maximum_clique_exhaustive.cpp`: `Maximum_Clique.cpp`
- `maximum_clique_dyn_exhaustive.cpp`: `Maximum_Clique_Dyn.cpp`
- `minimum_clique_cover_exhaustive.cpp`: `Minimum_Clique_Cover.cpp`
- `number_of_maximal_clique_exhaustive.cpp`: `NumberofMaximalClique.cpp`

Each harness exhausts every simple undirected graph through six vertices,
then checks deterministic empty-edge/complete/star cases and seeded random
graphs. Every inserted edge is also inserted as a duplicate, and selected
edges are inserted in reverse order. The oracles enumerate vertex subsets;
the clique-cover oracle uses an exact subset-partition DP.

The static maximum-clique template is tested only for `n >= 1`, matching its
valid use domain; the dynamic maximum-clique, clique-cover, and maximal-clique
tests also include `n == 0`.
