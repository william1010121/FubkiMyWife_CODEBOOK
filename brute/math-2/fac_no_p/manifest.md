# Factorial-without-p harness

- Template: `codebook/6_Math/fac_no_p.cpp`
- Problem URL: https://judge.yosupo.jp/problem/many_factorials
- Oracle: direct product of `1..n` after removing every factor `p`, reduced modulo `p^k`.
- Coverage: every prime `p <= 19`, `k <= 4`, `n <= 250`, plus larger `n` cases where the direct oracle remains cheap.
- Command: `c++ -std=c++17 -O2 harness.cpp -o harness && ./harness`
