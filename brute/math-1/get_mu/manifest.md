# getMu

- Template: `codebook/6_Math/getMu.cpp`
- Harness: `harness.cpp`
- Oracle: factor each integer directly; return `0` for a squared prime factor and `(-1)^k` otherwise.
- Domain tested: every prefix `1..n` for `1 <= n <= 1000`.
- Actual problem: [Luogu U405881](https://www.luogu.com.cn/problem/U405881), linear-sieve Möbius values and prefix queries.
