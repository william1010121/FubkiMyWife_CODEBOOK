# Data Structure partition manifest

Every harness below is a standalone C++17 program and is intended to be run
sequentially with one CPU. Random seeds are fixed. Each oracle is a small
multiset/array/tree brute force, so it is suitable for local stress checks and
does not launch parallel work.

| Template | Source | Actual online problem / official page | Local oracle and scope |
|---|---|---|---|
| Discrete Trick | `discrete_trick.cpp` | [CSES Salary Queries](https://cses.fi/problemset/task/1144/) | Sorted-list lower/upper-bound semantics, duplicates and absent keys; confirms no `sort`/`unique` is hidden in the snippet. |
| BIT kth | `BIT_kth.cpp` | [CSES Hotel Queries](https://cses.fi/problemset/task/1143/) | `bit_kth/main.cpp` builds random frequencies and checks every valid rank; requires power-of-two `N`. |
| IntervalContainer | `IntervalContainer.cpp` | [Codeforces 915E - Physical Education Lessons](https://codeforces.com/problemset/problem/915/E) | Random half-open add/remove against an explicit integer set; checks disjointness and canonical merging. |
| Leftist Tree | `Leftist_Tree.cpp` | [Luogu P3377 - Leftist Tree](https://www.luogu.com.cn/problem/P3377) | Random max-heap insertion/pop, subtree size and sum against a vector oracle. |
| Heavy-light Decomposition | `Heavy_light_Decomposition.cpp` | [Luogu P3384 - Tree Chain Splitting](https://www.luogu.com.cn/problem/P3384) | Random trees; compares path sums from the included segment tree with direct path sums. |
| Centroid Decomposition | `Centroid_Decomposition.cpp` | [Luogu P3806 - Centroid Decomposition](https://www.luogu.com.cn/problem/P3806) | Random weighted trees and repeated marks; compares sum of distances to every marked node. |
| Li Chao | `LiChaoST.cpp` | [Library Checker - Line Add Get Min](https://judge.yosupo.jp/problem/line_add_get_min) | Random integer-domain lines; checks maximum variant against scanning all lines (the source documents negation for minimum). |
| Link-cut tree | `link_cut_tree.cpp` | [Luogu P3690 - Link Cut Tree](https://www.luogu.com.cn/problem/P3690) | Chain path XOR, point change, cut/link and connectivity checks against the known chain. |
| KDTree | `KDTree.cpp` | [Luogu P4148 - Simple Problem](https://www.luogu.com.cn/problem/P4148) | Random points and queries; compares squared nearest non-identical distance with a scan. |
| Treap | `Treap.cpp` | [Luogu P3369 - Ordinary Balanced Tree](https://www.luogu.com.cn/problem/P3369) | Random duplicate insert/erase, kth and rank against `multiset`. |
| Range Chmin/Chmax/Add/Sum (inactive) | `Range_Chmin_Chmax_Add_Range_Sum.cpp` | [Library Checker - Range Chmin Chmax Add Range Sum](https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum) | Random range operations; compares every point and range sum with a vector. |
| min_heap (inactive) | `min_heap.cpp` | N/A (implementation helper; no canonical problem found) | Random lazy offsets and meld against explicit `(key,id)` vectors. |
| KDTree_useful (inactive) | `KDTree_useful.cpp` | [Luogu P4148 - Simple Problem](https://www.luogu.cn/problem/P4148) | Dynamic insert/erase and nearest-neighbour checks against a point vector. |
| Smart Pointer (inactive) | `Smart_Pointer.cpp` | N/A (C++ ownership helper, not an algorithmic problem) | Copy/assignment/value-lifetime behavior checks; no contest claim. |
| DSU (inactive) | `DSU.cpp` | [CSES Building Roads](https://cses.fi/problemset/task/1666/) | Random unions and all-pairs component equivalence against labels. |
| Segment Tree (inactive) | `Segment_Tree.cpp` | [CSES Dynamic Range Sum Queries](https://cses.fi/problemset/task/1648/) | Range add/range max against an array. |
| Binary Index Tree (inactive) | `Binary_Index_Tree.cpp` | [CSES Range Update Queries](https://cses.fi/problemset/task/1651/) | Point updates plus suffix updates, checking every prefix against an array. |
| Sparse Table (inactive) | `Sparse_table.cpp` | [CSES Static Range Minimum Queries](https://cses.fi/problemset/task/1647/) | Static range maximum queries against a scan (same idempotent sparse-table invariant). |
| 2D Segment Tree (inactive) | `2D_Segment_Tree.cpp` | [CSES Forest Queries](https://cses.fi/problemset/task/1652/) | Positive random matrix rectangle maxima against a nested-loop scan. |

`KDTree` in this codebook intentionally skips points at distance zero; the
oracle follows that documented contract. `LiChao` is a maximum container even
though the linked Library Checker task asks for minimum, so the harness tests
the exact source contract and the manifest records the sign conversion.
