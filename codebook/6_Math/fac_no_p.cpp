// n! without p factors mod pk=p^k; p prime; O(pk+log_p n).
ll prod[MAXP];
ll fac_no_p(ll n, ll p, ll pk) { // n>=0,k>=1,MAXP>pk.
  prod[0] = 1; for (int i = 1; i <= pk; ++i)
    prod[i] = i % p ? prod[i - 1] * i % pk : prod[i - 1];
  ll rt = 1; for (; n; n /= p) {
    if ((n / pk) & 1) rt = rt * prod[pk] % pk; // (Product of units)^2=1.
    rt = rt * prod[n % pk] % pk;
  } return rt;
}
