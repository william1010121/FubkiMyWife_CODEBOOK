void getMu() { // 1<=n<INT_MAX; zero flg,tot; p 1-indexed. O(n).
  // mu,flg[0..n]; p[1..number of primes] must have capacity.
  mu[1] = 1; for (int i = 2; i <= n; ++i) {
    if (!flg[i]) p[++tot] = i, mu[i] = -1;
    for (int j = 1; j <= tot && 1LL*i*p[j] <= n; ++j) {
      int x = i * p[j]; flg[x] = 1;
      if (i % p[j] == 0) { mu[x] = 0; break; }
      mu[x] = -mu[i];
    } }
}
