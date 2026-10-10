void getMu() { // Zero flg,tot first; p is 1-indexed. O(n).
  mu[1] = 1; for (int i = 2; i <= n; ++i) {
    if (!flg[i]) p[++tot] = i, mu[i] = -1;
    for (int j = 1; j <= tot && 1LL*i*p[j] <= n; ++j) {
      int x = i * p[j]; flg[x] = 1;
      if (i % p[j] == 0) { mu[x] = 0; break; }
      mu[x] = -mu[i];
    } }
}
