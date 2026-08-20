# Prime-counting harness

- Template: `codebook/6_Math/PiCount.cpp`
- Problem URL: https://judge.yosupo.jp/problem/counting_primes
- Oracle: exact sieve for every value through `2,000,000`, plus published reference values at `10^9`, `10^12`, and `10^13`.
- Coverage: boundary values, dense small range, and large-input regression values.
- Command: `c++ -std=c++17 -O2 harness.cpp -o harness && ./harness`
