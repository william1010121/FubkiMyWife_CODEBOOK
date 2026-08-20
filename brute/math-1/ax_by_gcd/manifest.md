# ax+by=gcd

- Template: `codebook/6_Math/ax+by=gcd.cpp`
- Harness: `harness.cpp`
- Oracle: `std::gcd(a,b)` plus direct Bézout identity check `a*x+b*y=gcd(a,b)`.
- Domain tested: all `0 <= a,b <= 200`, excluding `(0,0)`; this matches the snippet's nonnegative-input contract.
- Actual problem: [Luogu P5656](https://www.luogu.com.cn/problem/P5656) (two-variable linear Diophantine equation / exgcd).
