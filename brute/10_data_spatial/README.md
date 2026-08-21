# Data/spatial edge-case harnesses

These standalone C++17 tests use deterministic exhaustive cases and seeded
random cases with simple scan/vector oracles.  They are intentionally kept in
this partition so the central runner can execute them independently.

| Harness | Template | Coverage |
|---|---|---|
| `kdtree_static_edges.cpp` | `KDTree.cpp` | One point, all duplicates, duplicate grids, extreme signed coordinates, query points, and seeded random nearest scans. |
| `kdtree_useful_edges.cpp` | `KDTree_useful.cpp` | Constructor insertion, duplicate insert/erase, kth L1 nearest, inclusive rectangle queries, sorted/adversarial updates, rebuilds, missing deletes, and complete drain. |
| `lichao_edges.cpp` | `LiChaoST.cpp` | Empty tree, domain sizes 1/2, boundaries, equal slopes/intercepts, negative coefficients, exhaustive small coefficients, and seeded random line scans. |
| `range_beats_edges.cpp` | `Range_Chmin_Chmax_Add_Range_Sum.cpp` | Singletons, equal extrema, full/partial ranges, no-op clamps, large signed values, adversarial clamp/add order, and seeded random O(n) range checks. |

Each harness fails with a diagnostic on the first mismatch and was checked
sequentially with `rbox` using one compiler/test process at a time.
