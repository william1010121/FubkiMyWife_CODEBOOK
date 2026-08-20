# Mod_Inverse

- Template: `codebook/6_Math/Mod_Inverse.cpp`
- Harness: `harness.cpp`
- Oracle: verify `i * inv[i] % p == 1` for every generated inverse.
- Domain tested: every prime `p <= 300` and every `1 <= i < p`.
- Actual problem: [Luogu P3811](https://www.luogu.com.cn/problem/P3811), linear inverse recurrence under a prime modulus.
