# Simultaneous_Equations

- Templates: `codebook/6_Math/Simultaneous_Equations.cpp` plus its required `Fraction.cpp`.
- Harness: `harness.cpp`
- Oracle: independently implemented exact rational Gauss-Jordan elimination; checks inconsistent/consistent classification, rank, and every unique solution.
- Domain tested: 9600 seeded random systems for each `1 <= n,m <= 4`, coefficients and RHS in `[-2,2]`.
- Actual problem: [Luogu U608563](https://www.luogu.com.cn/problem/U608563), linear equation systems with arbitrary equation/variable counts.
