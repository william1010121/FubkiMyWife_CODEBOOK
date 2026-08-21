# Graph/tree edge-case harnesses

These tests are intentionally independent and single-process.  They cover:

- `dominator_exhaustive.cpp`: every directed graph with at most four vertices,
  every root, self-loops, parallel edges, repeated `tarjan()` calls, and seeded
  larger multigraphs.  The oracle iterates dominator sets to a fixed point.
- `virtual_tree_exhaustive.cpp`: all rooted increasing-label trees through six
  vertices and every key subset, reset/rebuild checks, arbitrary key order, and
  repeated-key boundary probes.  The oracle closes keys under pairwise LCA and
  connects nearest virtual ancestors.
- `steiner_edge_oracle.cpp`: empty/single/repeated terminal cases,
  disconnected directed graphs, all simple directed graphs on three vertices,
  and seeded small multigraphs.  The oracle enumerates edge subsets and common
  sinks while charging selected vertices once.
- `four_circle_exhaustive.cpp`: disconnected graphs, complete/bipartite graphs,
  and every simple undirected graph through six vertices, with an ordered
  distinct-4-tuple oracle divided by eight.

Each file is compiled as C++17 and run sequentially under the repository's
resource-limited `rbox`.  The current source exposes two boundary-contract
failures: duplicate Virtual Tree keys create a self-loop, and an empty Steiner
terminal set returns the minimum vertex cost rather than the empty-tree cost
used by the oracle.  The harnesses keep these as fail-loud probes rather than
silently weakening the edge-case coverage.
