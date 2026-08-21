# Math-number edge-case harnesses

These focused C++17-compatible harnesses are intentionally separate from the
older `math-1` and `math-2` tests.  They cover:

- `primes_miller_extra.cpp`: a sieve through 1,200,000, seeded composites,
  64-bit Miller--Rabin witnesses, and the `Primes.cpp` list shape/content.
- `pollard_extra.cpp`: trial-factorized small values, prime powers,
  semiprimes, a 64-bit Mersenne prime, `LLONG_MAX`, and seeded products of
  known small primes.
- `get_mu_extra.cpp`: independent smallest-prime-factor Mobius values,
  prime-count checks, and the divisor-sum identity through 500,000.
- `mod_inverse_extra.cpp`: complete small prime tables and large-prime
  prefixes checked against an independent extended-Euclid inverse.
- `ax_by_extra.cpp`: zero cases, exhaustive small nonnegative inputs,
  64-bit boundary pairs, and seeded nonnegative inputs checked in `__int128`.
- `chinese_remainder_extra.cpp`: exact signed-`__int128` CRT oracle for exhaustive
  small moduli, negative residues, feasible/infeasible non-coprime cases, and
  large-modulus boundary cases.
- `discrete_log_extra.cpp`: direct cycle enumeration, seeded non-unit cases,
  moduli around the template's `kStep`, and solvable four-argument cases.

All tests fail at the first mismatch and print the complete counterexample.
The CRT boundary case with large negative residues is deliberately included
to detect the overflow risk called out by the template itself.
