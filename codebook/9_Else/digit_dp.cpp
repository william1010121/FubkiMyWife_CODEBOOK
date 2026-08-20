// Number of integers in [0, x] whose binary representation has no adjacent
// one bits.  This is a compact, reusable digit-DP example.
int calc(int x) {
  if (x < 0) return 0;
  long long dp[32][2][2]{};
  dp[31][0][1] = 1;
  for (int pos = 30; pos >= 0; --pos) {
    int bit = (x >> pos) & 1;
    for (int prev = 0; prev < 2; ++prev)
      for (int tight = 0; tight < 2; ++tight) {
        long long ways = dp[pos + 1][prev][tight];
        if (!ways) continue;
        int lim = tight ? bit : 1;
        for (int cur = 0; cur <= lim; ++cur) {
          if (prev && cur) continue;
          int ntight = tight && cur == bit;
          dp[pos][cur][ntight] += ways;
        }
      }
  }
  long long ret = 0;
  for (int prev = 0; prev < 2; ++prev)
    for (int tight = 0; tight < 2; ++tight) ret += dp[0][prev][tight];
  return (int)ret;
}
