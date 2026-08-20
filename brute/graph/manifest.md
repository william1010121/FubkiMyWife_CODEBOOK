# graphs-1 verification manifest

Scope: `codebook/2_Graph/BCC_Vertex.cpp`, `Bridge.cpp`, `SCC.cpp`, `2SAT.cpp`,
`Virtual_Tree.cpp`, and `Dominator_Tree.cpp` only.

All harnesses are independent single-process C++ programs. They use fixed-seed
small randomized inputs and an in-harness brute-force oracle; no threads or
parallel stress are used.

| Template | Harness | Oracle coverage | Online problem |
|---|---|---|---|
| BCC_Vertex | `brute/graph/BCC_Vertex/harness.cpp` | vertex-block partition, articulation points, block-cut edges; disconnected random multigraphs and parallel edges | [Luogu P8435](https://www.luogu.com.cn/problem/P8435) |
| Bridge | `brute/graph/Bridge/harness.cpp` | brute edge deletion for bridges and DSU partition after removing bridges; disconnected random multigraphs and parallel edges | [Luogu P8436](https://www.luogu.com.cn/problem/P8436) |
| SCC | `brute/graph/SCC/harness.cpp` | Floyd-Warshall reachability equivalence and reverse-topological component IDs; directed multigraphs | [Luogu P2746](https://www.luogu.com.cn/problem/P2746) |
| 2SAT | `brute/graph/2SAT/harness.cpp` | exhaustive assignments for satisfiability plus returned witness validation | [Luogu P4782](https://www.luogu.com.cn/problem/P4782) |
| Virtual_Tree | `brute/graph/Virtual_Tree/harness.cpp` | pairwise-LCA closure and nearest-virtual-ancestor edges; arbitrary rooted trees, singleton keys, non-key virtual roots | [Codeforces 613D](https://codeforces.com/problemset/problem/613/D), [Luogu P2495](https://www.luogu.com.cn/problem/P2495) |
| Dominator_Tree | `brute/graph/Dominator_Tree/harness.cpp` | iterative dominator-set oracle and immediate-dominator parent comparison; reachable and unreachable directed multigraphs | [Luogu P5180](https://www.luogu.com.cn/problem/P5180), [Codeforces 757F](https://codeforces.com/contest/757/problem/F) |

## Recorded local result

The harnesses were compiled and run sequentially earlier in this turn with
`g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic`. Results: BCC_Vertex 5000,
Bridge 5000, SCC 5000, 2SAT 6000, Virtual_Tree 5000, and Dominator_Tree
5000 cases passed. The compile emitted only the existing assignment-in-if
warning in `Dominator_Tree.cpp`.
