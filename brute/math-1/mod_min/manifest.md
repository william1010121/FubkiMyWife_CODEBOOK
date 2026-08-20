# ModMin

- Template: `codebook/6_Math/ModMin.cpp`
- Harness: `harness.cpp`
- Oracle: scan `k=0..2m+2` and return the first value satisfying the interval; the period makes this exhaustive for the tested contract.
- Domain tested: `1 <= m <= 80`, `0 <= a < m`, `0 <= l <= r < m`.
- Actual problem reference: [ICPC 2019–2020 North-Western Russia Regional Contest tutorial](https://codeforces.com/gym/104874/attachments/download/23438/nerc-19-20-tutorial.pdf), whose solution explicitly reduces a subproblem to `l <= (a*k) mod m <= r`.
