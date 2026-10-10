// Needs ModArithmetic; p prime, 0 <= N < p.
struct ModBinomial {
  ModArithmetic mod;
  std::vector<long long> fac, ifac;
  ModBinomial(int N, long long p) : mod(p) {
    assert(p >= 2 && N >= 0 && N < p);
    fac.resize(N + 1); ifac.resize(N + 1); fac[0] = 1;
    for (int i = 1; i <= N; ++i)
      fac[i] = mod.mul(fac[i - 1], i);
    ifac[N] = mod.pow(fac[N], p - 2);
    for (int i = N; i; --i)
      ifac[i - 1] = mod.mul(ifac[i], i);
  }
  long long C(int n, int k) const {
    if (n < 0 || k < 0 || k > n) return 0;
    assert(n < (int)fac.size());
    return mod.mul(fac[n], mod.mul(ifac[k], ifac[n - k]));
  }
  long long lucas(long long n, long long k) const {
    // Requires N=p-1 (small prime p); O(log_p n).
    if (n < 0 || k < 0 || k > n) return 0;
    assert((long long)fac.size() == mod.m);
    long long r = 1;
    for (; n || k; n /= mod.m, k /= mod.m)
      r = mod.mul(r, C(n % mod.m, k % mod.m));
    return r;
  }
};
