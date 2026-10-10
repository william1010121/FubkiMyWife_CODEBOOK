// x=a[j], y=a[j+L/2]: OR y+=x*op; AND x+=y*op.
// XOR: (x,y)=(x+y,x-y); inverse divides both by 2.
// n=2^k, 0<=k<=29; op=1/-1; O(n log n); int arithmetic must fit.
void fwt(int *a, int n, int op) { // OR
  for (int L = 2; L <= n; L <<= 1)
    for (int i = 0; i < n; i += L)
      for (int j = i; j < i + L / 2; ++j) a[j + L / 2] += a[j] * op;
}
const int N = 21;
int f[N + 1][1 << N], g[N + 1][1 << N], h[N + 1][1 << N];
void subset_convolution(int *a, int *b, int *c, int L) {
  // c[k]=sum(a[i]*b[j]) for i|j=k, i&j=0; 0<=L<=N.
  // a,b,c: 2^L ints outside f/g/h; c may alias a or b.
  // O(2^L L^2) time; three (N+1)*2^N int arrays.
  int n = 1 << L;
  for (int i = 0; i <= L; ++i)
    fill_n(f[i], n, 0), fill_n(g[i], n, 0), fill_n(h[i], n, 0);
  for (int i = 0; i < n; ++i) {
    int k = popcount((unsigned)i); f[k][i] = a[i], g[k][i] = b[i];
  }
  for (int i = 0; i <= L; ++i) fwt(f[i], n, 1), fwt(g[i], n, 1);
  for (int i = 0; i <= L; ++i)
    for (int j = 0; j <= i; ++j)
      for (int x = 0; x < n; ++x) h[i][x] += f[j][x] * g[i - j][x];
  for (int i = 0; i <= L; ++i) fwt(h[i], n, -1);
  for (int i = 0; i < n; ++i) c[i] = h[popcount((unsigned)i)][i];
}
