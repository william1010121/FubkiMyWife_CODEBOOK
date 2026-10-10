int bit[N + 1]; // 1-base, N > 0, nonnegative frequencies
int query_kth(int k) { // 1 <= k <= total, sums fit int; O(log N)
  int res = 0;
  for (int i = 1 << __lg(N); i; i >>= 1)
    if (res + i <= N && bit[res + i] < k) k -= bit[res += i];
  return res + 1;
}
