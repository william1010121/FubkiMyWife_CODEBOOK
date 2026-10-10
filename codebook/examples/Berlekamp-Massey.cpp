// mint: prime-modulus field with division.
vector<mint> seq = {0,1,1,2,3,5,8};
auto coef = BerlekampMassey(seq); // {1,1}
// a[i] = coef[0]*a[i-1] + coef[1]*a[i-2].
