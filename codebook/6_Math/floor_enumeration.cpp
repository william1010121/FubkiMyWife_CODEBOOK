// 0<=n<INT_MAX; floor(n/i)=x on [l,r], O(sqrt(n)) blocks.
// l<=x gives r=l; next l>=q gives next quotient q-1.
for (int l=1,r,q=n; l<=n; l=r+1,q=l<q ? n/l : q-1) {
  int x=q; r=l<=x ? l : n/x; }
