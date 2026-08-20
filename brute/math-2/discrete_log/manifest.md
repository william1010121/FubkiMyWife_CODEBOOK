# Discrete-log harness

- Template: `codebook/6_Math/DiscreteLog.cpp`
- Problem URL: https://judge.yosupo.jp/problem/discrete_logarithm_mod
- Oracle: direct enumeration of `x^k mod m` and the first exponent that reaches `y`.
- Coverage: every `m <= 80`, every `0 <= x,y < m`, including non-coprime and non-unit bases; also checks the four-argument helper on solvable instances.
- Command: `c++ -std=c++17 -O2 harness.cpp -o harness && ./harness`
