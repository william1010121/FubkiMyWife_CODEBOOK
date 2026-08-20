# Berlekamp-Massey harness

- Template: `codebook/6_Math/Berlekamp-Massey.cpp`
- Problem URL: https://judge.yosupo.jp/problem/consecutive_terms_of_linear_recurrent_sequence
- Oracle: generate sequences from known shortest recurrences over the prime field `998244353`; verify the returned recurrence reproduces every term and has the expected shortest order.
- Coverage: zero/constant sequences, random recurrences of orders `1..12`, and truncations with leading zero coefficients.
- Command: `c++ -std=c++17 -O2 harness.cpp -o harness && ./harness`
