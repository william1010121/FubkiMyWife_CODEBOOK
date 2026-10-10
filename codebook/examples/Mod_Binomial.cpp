ModBinomial comb(1000000, 1000000007);
comb.C(5, 2);  // 10; invalid k returns 0
comb.fac[5];   // 120
comb.ifac[5];  // inverse of 120 modulo p
ModBinomial small(6, 7);
small.lucas(10, 3); // 120 mod 7 = 1
