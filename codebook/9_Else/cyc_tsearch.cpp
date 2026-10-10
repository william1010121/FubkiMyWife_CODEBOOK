// n>=1; cyclic-shift U-function with distinct values.
// pred(a,b) means f(a)<f(b); returns a minimum index, O(log n).
int cyc_tsearch(int n, auto pred) {
  if (n == 1) return 0;
  int l = 0, r = n; bool rv = pred(1, 0);
  while (r - l > 1) {
    int m = l + (r - l) / 2;
    if (pred(0, m) ? rv : pred(m, (m + 1 == n ? 0 : m + 1))) r = m;
    else l = m;
  } return pred(l, r % n) ? l : r % n;
}
