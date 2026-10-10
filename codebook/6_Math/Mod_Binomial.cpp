// ModArithmetic; prime p, 0<=N<min(p,INT_MAX). O(N+log p) init.
struct ModBinomial { ModArithmetic mod; vector<ll> fac, ifac;
  ModBinomial(int N, ll p) : mod(p) {
    assert(p >= 2 && N >= 0 && N < p);
    fac.resize(N + 1), ifac.resize(N + 1), fac[0] = 1;
    auto mul = [p](ll x, int i) { return p < (1LL << 31) ?
      x*i%p : (ll)((__int128)x*i%p); };
    for (int i = 1; i <= N; ++i) fac[i] = mul(fac[i - 1], i);
    ifac[N] = mod.pow(fac[N], p - 2);
    for (int i = N; i; --i) ifac[i - 1] = mul(ifac[i], i);
  }
  ll C(int n, int k) const { // n<0 or k outside [0,n]: 0; else n<=N.
    if (n < 0 || k < 0 || k > n) return 0; assert(n < (int)fac.size());
    if (mod.m < (1LL << 31)) return fac[n]*ifac[k]%mod.m*ifac[n-k]%mod.m;
    return (__int128)fac[n]*ifac[k]%mod.m*ifac[n - k]%mod.m;
  }
  ll lucas(ll n, ll k) const { // N=p-1, small p; O(log_p n).
    if (n < 0 || k < 0 || k > n) return 0; assert((ll)fac.size() == mod.m);
    ll r = 1; for (; r && (n || k); n /= mod.m, k /= mod.m)
      r = r*C(n % mod.m, k % mod.m)%mod.m; // p<=INT_MAX
    return r;
  }
};
