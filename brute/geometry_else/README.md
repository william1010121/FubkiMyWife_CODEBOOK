# Geometry / Else brute verification

This directory is owned by the `geometry-else` partition.  It contains one
manifest covering every source/template currently present in
`codebook/8_Geometry` and `codebook/9_Else`, including files commented out of
`content.tex`.

Run from the repository root:

```sh
python3 brute/geometry_else/run.py
```

The runner compiles and executes the two independent single-process harnesses
with `-O0 -std=c++20`.  It deliberately does not start a thread pool or a
parallel stress job.  Geometry tests use integer cases, exact predicates, and
`long double` tolerances; Else tests compare against exhaustive small-input
oracles where the template has a complete callable interface.

`not-runnable` entries are intentional: they are explanatory material,
unfinished skeletons, or snippets whose required surrounding declarations are
not present in the file.  They are still listed, with the missing context and
an actual problem/reference URL, so the checklist does not silently omit them.
