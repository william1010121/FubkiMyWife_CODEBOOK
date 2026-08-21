# String pattern edge and brute tests

These focused C++17 harnesses are independent of the repository-wide runner.
Each uses exhaustive tiny-alphabet cases plus deterministic seeded cases and a
simple direct oracle; a mismatch prints the complete failing input and exits
nonzero.

| Harness | Coverage and oracle |
|---|---|
| `kmp_exhaustive.cpp` | Every binary text through length 8 against every nonempty binary pattern through length 6, ternary cases, overlaps, empty text, and seeded long cases; direct substring comparison. |
| `aho_corasick_exhaustive.cpp` | Every ABC text through length 5 against all nonempty ABC patterns through length 3, empty text, empty pattern sets, nested/overlapping patterns, duplicate insertions sharing endpoints, and seeded random dictionaries; direct occurrence counting. |
| `manacher_exhaustive.cpp` | Every binary string through length 10 and ternary string through length 7, empty/singleton/repeated/even palindromes, and seeded cases; direct transformed-string palindrome expansion. |
| `z_value_exhaustive.cpp` | Every binary string through length 13 and ternary string through length 8, empty/singleton/repeated/periodic strings, and seeded cases; direct LCP expansion from position zero. |

The KMP source currently emits 1-based match positions, so the KMP oracle
checks that actual contract. The PDF text describes those positions as 0-based;
that documentation mismatch is reported separately rather than silently
changing the template. Empty KMP patterns and empty AC patterns are excluded:
the templates do not define those inputs (`t[0]`/root semantics would be
ambiguous), while empty texts and empty dictionaries are tested.
