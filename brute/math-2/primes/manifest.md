# Primes-list harness

- Template: `codebook/6_Math/Primes.cpp`
- Problem URL: https://judge.yosupo.jp/problem/enumerate_primes
- Oracle: deterministic trial/Miller-Rabin primality check using `__int128` multiplication.
- Coverage: parses the current source file, checks every listed value is prime, checks strict increasing order and uniqueness.
- Command: `c++ -std=c++17 -O2 harness.cpp -o harness && ./harness ../../../codebook/6_Math/Primes.cpp`
