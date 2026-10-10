// 0<=n<INT_MAX; floor(n/i)=x on [l,r], O(sqrt(n)) blocks.
for (int l = 1, r; l <= n; l = r + 1) { int x = n/l; r = n/x; }
