# math-3 brute harnesses

This directory belongs only to the `codebook/6_Math` partition. `manifest.tsv` has one row for every one of the 26 `.cpp` files in that directory, including the two non-algorithm snippets.

The `harness_*.cpp` files are separated by topic. Each uses a small exact or finite-domain oracle; no network judge submission is performed here. A future runner must compile and execute them serially, with at most one process at a time, so the global four-CPU limit is respected.

`Primes.cpp` is a commented constant list, so it remains `manifest-only` rather than being falsely reported as an algorithm test. The bare `floor_enumeration.cpp` loop is exercised verbatim by the dedicated `math-1/floor_enumeration` harness. `Simultaneous_Equations.cpp` and `cantor_expansion.cpp` include the source fixes made in this partition.
