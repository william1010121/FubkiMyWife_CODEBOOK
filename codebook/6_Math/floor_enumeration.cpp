// 0<=n<INT_MAX; floor(n/i)=x on [l,r], O(sqrt(n)) blocks.
// l<=x implies r=l; skip one division before sqrt(n).
for (int l=1,r; l<=n; l=r+1) { int x=n/l; r=l<=x ? l : n/x; }
