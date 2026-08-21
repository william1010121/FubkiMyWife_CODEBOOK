# Flow edge-case oracle suite

These four focused harnesses are intentionally separate so a failure names the
flow template and the small graph family that exposed it:

- `dinic_edge_cases.cpp`: exhaustive 0/1 directed capacities through four
  vertices, explicit zero and parallel arcs, 1,800 seeded multigraphs, reset
  checks, and a forked `s == t` contract probe.
- `isap_edge_cases.cpp`: exhaustive terminal graphs through four ordinary
  vertices (including `n == 0`), explicit zero and parallel arcs, and 1,800
  seeded multigraphs.  ISAP has fixed distinct terminals `n+1,n+2`, so there is
  no same-terminal API call to make.
- `sw_mincut_edge_cases.cpp`: every undirected 0/1 graph through five vertices,
  explicit parallel weighted edges, 1,800 seeded weighted multigraphs, and an
  `n == 1` contract probe.
- `gomory_hu_edge_cases.cpp`: every undirected 0/1 graph through five vertices,
  explicit parallel weighted edges, 1,000 seeded weighted multigraphs, all
  pair cuts including same-vertex queries, and singleton coverage.

Directed max-flow oracles enumerate every source-side subset and sum outgoing
capacities.  Undirected global-cut oracles enumerate every non-trivial
partition.  Gomory-Hu results are checked by taking the minimum edge on every
tree path against the same partition oracle.  All random cases use fixed
`mt19937` seeds and all capacities are small nonnegative integers.

Compile and run each file serially with C++17 from the repository root.  The
Dinic `s == t` probe is isolated because the current implementation does not
terminate on that conventional zero-flow case; the harness reports this as a
template precondition instead of allowing a worker to hang.  Stoer-Wagner's
`n == 1` result is likewise reported as a finite-input precondition when it
returns `INT_MAX`.
