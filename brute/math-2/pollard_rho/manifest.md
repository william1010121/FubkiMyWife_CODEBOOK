# Pollard-Rho harness

- Template: `codebook/6_Math/Pollard_Rho.cpp`
- Problem URL: https://judge.yosupo.jp/problem/factorize
- Oracle: trial division factorization for small values and `__int128` multiplication for the large test set.
- Coverage: every `n` from 1 through 100,000, all prime powers in range, semiprimes, and selected 64-bit composites.
- Command: `c++ -std=c++17 -O2 harness.cpp -o harness && ./harness`
