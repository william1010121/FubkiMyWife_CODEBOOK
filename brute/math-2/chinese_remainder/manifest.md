# Chinese-remainder harness

- Template: `codebook/6_Math/chineseRemainder.cpp`
- Problem URL: https://open.kattis.com/problems/generalchineseremainder
- Oracle: enumerate one complete lcm period for small moduli and verify both the no-solution and normalized-solution cases.
- Coverage: all residues and moduli `1..24`, including non-coprime moduli and negative residues.
- Command: `c++ -std=c++17 -O2 harness.cpp -o harness && ./harness`
