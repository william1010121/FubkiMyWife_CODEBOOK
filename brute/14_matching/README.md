# Matching edge-case harnesses

These focused tests use small recursive oracles rather than another matching
implementation:

- `bipartite_edge_cases.cpp`: exhaustive unequal-side graphs, empty sides,
  reinitialisation, duplicate edges, isolated vertices, and seeded patterns.
- `kuhn_munkres_exhaustive.cpp`: every complete 3-by-3 matrix over
  `{-2,-1,0,1,2}`, every tiny missing-edge mask with a forced perfect
  assignment, signed random matrices, and large-magnitude ties.
- `maximum_weight_matching_exhaustive.cpp`: exhaustive signed undirected
  weights through four vertices, unmatched/nonpositive edges, overwrite order,
  ties, and signed random graphs. The oracle follows the template's contract:
  only positive edges can improve a maximum-weight matching.
- `maximum_simple_matching_edges.cpp`: exhaustive graphs through five
  vertices, odd cycles, stars, cliques, disconnected/shared-center blossoms,
  duplicate insertion, and seeded sparse/dense graphs.
- `minimum_weight_matching_wrong_boundary.cpp`: exhaustive signed perfect
  matching cases through four vertices plus random even graphs. Its filename
  is intentional: a mismatch is reported as a template contract defect rather
  than hidden.

The first four harnesses should print `PASS`. The last one also prints `PASS`
only if the `*_wrong.cpp` template agrees with exhaustive minimum perfect
matching; otherwise it exits nonzero and prints the first counterexample it
encounters. No runner, codebook template, PDF, or other brute directory is
modified here.
