# SCC / 2SAT stronger brute tests

These focused harnesses are intentionally kept outside the central runner so
the persistent worker can schedule them with the global CPU and memory cap.
They include simple, independent oracles and fail on the first counterexample.

| Harness | Coverage |
|---|---|
| `scc_exhaustive.cpp` | Every simple directed graph on 1--4 vertices, including every self-loop pattern; Floyd-style transitive closure oracle. |
| `scc_multigraph_edges.cpp` | Empty/disconnected graphs, repeated arcs, repeated self-loops, chains, cycles, complete multigraphs, and seeded random directed multigraphs. |
| `sat_exhaustive.cpp` | Every formula over one and two variables; all ordered formulas of length at most three over three variables, including units, tautologies, contradictions, and duplicate clauses. |
| `sat_edge_cases.cpp` | Explicit boundary formulas plus seeded random unit/tautology/duplicate/conflict cases and assignment-preserving satisfiable formulas. |

Compile each file independently with C++17.  The harnesses do not use threads
or invoke the repository-wide runner.
