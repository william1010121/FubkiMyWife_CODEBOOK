// calc(x): count [0,x] with no adjacent 1 bits; O(log x), x<0 -> 0.
int calc(int x) {
  if (x < 0) return 0;
  int f[32] = {1, 2}, ans = 0, pre = 0;
  for (int i = 2; i < 32; ++i) f[i] = f[i-1] + f[i-2];
  for (int i = 30; i >= 0; --i) {
    if (x >> i & 1) {
      ans += f[i];
      if (pre) return ans;
      pre = 1;
    } else pre = 0;
  } return ans + 1;
}
