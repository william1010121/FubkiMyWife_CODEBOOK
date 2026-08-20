# floor_ceil

- Template: `codebook/6_Math/floor_ceil.cpp`
- Harness: `harness.cpp`
- Oracle: integer truncating quotient/remainder corrected by sign, independently written.
- Domain tested: `a in [-200,200]`, nonzero `b in [-30,30]`.
- Actual problem: [AtCoder ABC345 B](https://atcoder.jp/contests/abc345/tasks/abc345_b), which requires exact ceiling division for negative values.
