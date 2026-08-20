# Miller-Rabin harness

- Template: `codebook/6_Math/Miller_Rabin.cpp`
- Problem URL: https://judge.yosupo.jp/problem/primality_test
- Oracle: trial division for every `n <= 1,000,000`, plus deterministic 64-bit modular arithmetic for boundary witnesses.
- Coverage: `n < 2`, all values through `1e6`, Carmichael/pseudoprime values, and large known prime/composite values.
- Command: `c++ -std=c++17 -O2 harness.cpp -o harness && ./harness`
