# Math numeric edge-case harnesses

These focused tests are deliberately kept in this assignment directory so
they can be run independently by the central persistent worker.  Each harness
uses a direct finite-domain oracle and deterministic seeds; failures print the
complete small counterexample and exit nonzero.

- `harness_floor_ceil_edges.cpp`: signed exhaustive cases plus `int` boundary values.
- `harness_floor_enumeration_edges.cpp`: direct interval scans, maximality checks,
  and large values through `INT_MAX-1`.
- `harness_floor_sum_edges.cpp`: exhaustive and seeded cases around `a,b`/`m`
  reduction boundaries, with `n=0`, exact divisibility, and `m=1`.
- `harness_mod_min_edges.cpp`: exhaustive small intervals and a period-based
  oracle for seeded random cases, including singleton intervals and `a=0`.
- `harness_fraction_edges.cpp`: negative denominators, zero numerators, all
  binary operations, and seeded associativity checks.
- `harness_gaussian_gcd_edges.cpp`: signed Gaussian coordinates, zero `a`, and
  a maximal-common-divisor norm oracle.
- `harness_simultaneous_edges.cpp`: exact independent Gauss-Jordan oracle,
  exhaustive `1..3` equation/variable systems, and signed random systems.

The tests intentionally do not call undefined contracts: `floor_sum` is tested
with `n>=0,m>0,a,b>=0`; `ModMin` with `m>0, 0<=a<m, 0<=l<=r<m`;
`floor_enumeration` with `n>=1`; Gaussian gcd with nonzero second operand; and
fractions with nonzero denominators.  This is recorded rather than silently
claiming support for negative `floor_sum`/`ModMin` inputs or zero moduli.
