# Advanced graph boundary harnesses

These are isolated, test-only harnesses for the four graph templates assigned
to this directory:

- `zhu_liu_exhaustive.cpp`: enumerates one incoming edge for every non-root,
  rejects cycles that do not reach the root, and checks the exact minimum.
  It exhausts simple directed topologies through four vertices and adds
  parallel arcs, loops, negative/zero weights, disconnected cases, and seeded
  random multigraphs.
- `dmst_fast_exhaustive.cpp`: uses the same independent parent-choice oracle,
  then validates the returned edge IDs, root reachability, and total weight.
  Self-loops are included as inputs because the implementation should discard
  them while selecting incoming edges.
- `minimum_mean_cycle_exhaustive.cpp`: enumerates all simple directed cycles,
  including one-vertex loops, and compares reduced rational means. It covers
  exhaustive topologies through three vertices and seeded sparse/dense random
  graphs with negative, zero, and positive weights.
- `vizing_exhaustive.cpp`: brute-forces the exact chromatic index for every
  simple graph through five vertices, tries three edge orders, and validates
  the returned adjacent-matrix coloring. Additional seeded random simple
  graphs cover larger sparse cases.

Vizing is tested under its simple-undirected-graph contract; loops and
parallel edges are deliberately not passed to that template because its
adjacency-matrix API cannot represent them.

Run individual harnesses sequentially, for example:

```sh
g++ -std=c++17 -O2 brute/06_graph_advanced/zhu_liu_exhaustive.cpp -o /tmp/zhu
/tmp/zhu
```

No runner, template, PDF, or file outside this directory is modified here.
