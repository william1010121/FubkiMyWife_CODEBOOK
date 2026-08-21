// Number of integers in [0, x] whose binary
// representation has no adjacent
// one bits. This is a compact, reusable digit-DP
// example.
int calc(int x) {
  if (x < 0) return 0;
  int ways[31] = {1, 2}, ans = 0, prev = 0;
  for (int i = 2; i <=
    30; ++i) ways[i] = ways[i - 1] + ways[i - 2];
  for (int i = 30; i >= 0; --i) {
    if (x >> i & 1) {
      ans += ways[i];
      if (prev) return ans;
      prev = 1;
    } else prev = 0;
  }
  return ans + 1;
}
