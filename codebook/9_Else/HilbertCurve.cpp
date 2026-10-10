ll hilbert(int n, int x, int y) {
  ll res = 0;
  for (int s = n / 2; s; s >>= 1) {
    int rx = (x & s) > 0, ry = (y & s) > 0;
    res += s * 1ll * s * ((3 * rx) ^ ry);
    if (!ry && rx) x = s - 1 - x, y = s - 1 - y;
    if (!ry) swap(x, y);
  } return res;
} // n: positive int power of 2; 0<=x,y<n; O(log n).
