# Boundary-test findings

The edge-case pass added 119 focused C++ harnesses plus Python and inactive
geometry/inactive-source coverage. The complete runner now discovers 209 harnesses in total.
The first run with `JOBS=4` inside the
4-CPU/4-GiB rbox found the failures below. They were fixed in the templates;
the final run is `runner: jobs=4 timeout=60s tests=209 status=0`.

## Mismatches worth fixing

| Template | Boundary case | Observation |
|---|---|---|
| `MinimumMeanCycle.cpp` | `0 -> 0` with weight `6`, plus `0 -> 1` with weight `-6` | Fixed with SCC-local Karp DP, including one-vertex cycles. |
| `2D_Segment_Tree.cpp` | Partial query over negative values | Fixed both query accumulators to start at `INT_MIN`. |
| `Minimum_Weight_Matching_wrong.cpp` | `n=8` negative-weight perfect matching | Replaced the incorrect augmentation with an exact subset-DP solver for this small helper. |
| `chineseRemainder.cpp` | `(-1000000006 mod 1000000007)` and `(1000000008 mod 1000000009)` | Fixed intermediate difference, product, and modulus calculations with `__int128`. |
| `Mod_Inverse.cpp` | `mod=65537`, `i=9` | Fixed the recurrence's multiplication by widening it to `long long`. |
| `RotatingCaliper.cpp` | a second call, and a one-point set | Reset the global hull size on every call and preserve the singleton hull. |
| `11_Python/misc.py` | importing or executing the listing | Replaced undefined top-level examples with importable, brute-tested helpers. |
| `bigIntIO.cpp` | exact `INT128_MIN` and EOF | Uses unsigned magnitude arithmetic, exposes a checked read overload, and no longer negates the minimum signed value. |
| `KMP.cpp` | empty pattern and index contract | Handles the empty pattern; code, oracle, and description now consistently use 1-based positions. |
| `tree_hash.cpp` | seed zero | Normalizes zero to the nonzero default seed instead of collapsing every tree hash to zero. |
| `MainLorentz.cpp` | input containing `#`, NUL, or arbitrary bytes | Encodes bytes and uses an out-of-alphabet integer separator. |
| `DynamicConvexTrick_bb.cpp` | repeated slopes and queries between insertions | Replaced the comparator-switching set with a dynamic integer Li Chao tree. |
| `ManhattanMST.cpp` | random small point sets | Replaced the incorrect candidate sweep with the standard `-y` map sweep. |
| `DynamicMST.cpp` | a single update on a three-vertex tree | Leaf processing now runs Kruskal on all remaining candidates under the rollback-DSU state. |
| `Triangulation_Vonoroi.cpp` | three non-collinear sites | Replaced the heap-corrupting debug implementation with a verified finite-cell area reference. |

## Inputs requiring an explicit contract

- `Virtual_Tree.cpp`: `build()` now canonicalizes duplicate keys before
  constructing the tree.
- `MinimumSteinerTree.cpp`: empty and repeated terminal sets are now handled;
  an empty set returns `0`.
- `link_cut_tree.cpp`: `lca()` now uses the standard `access(x); access(y)`
  implementation and matches the root-relative oracle.
- `Suffix_Array.cpp` and `Smallest_Rotation.cpp`: empty strings now return an
  empty result safely.
