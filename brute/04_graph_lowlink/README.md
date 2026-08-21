# Low-link boundary tests

These focused harnesses cover `BCC_Vertex.cpp` and `Bridge.cpp` without
changing the codebook templates or the central runner.

- `bcc_exhaustive.cpp`: every undirected multigraph on 1--4 vertices when
  each loop or unordered pair has multiplicity 0, 1, or 2.
- `bcc_edge_cases.cpp`: isolated vertices, loop-only graphs, articulation
  chains, cycles, parallel edges, disconnected blocks, and fixed-seed random
  edge-order permutations.
- `bridge_exhaustive.cpp`: the same exhaustive multigraph family, checking
  every edge by deletion and checking the resulting 2-edge-connected
  partition.
- `bridge_edge_cases.cpp`: bridge chains, cycles, parallel bundles, loops,
  disconnected graphs, and fixed-seed random edge-order permutations.

The oracle uses only BFS connectivity after deleting one vertex or one edge.
For BCC, maximal vertex sets whose every pair remains connected after deleting
any third vertex are enumerated directly; isolated loop-only vertices are
singleton blocks because the template exposes vertex blocks rather than loop
edges.

Each `.cpp` is an independent C++17 executable and is discovered by
`brute/run_all.sh` automatically. The central runner should remain the only
full-suite process; focused local runs should be sequential and resource
limited.
