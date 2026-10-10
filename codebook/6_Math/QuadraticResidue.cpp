// Jacobi: positive odd modulus; O(log m).
int Jacobi(int a, int m) {
  a %= m; if (a < 0) a += m;
  int s = 1;
  for (; m > 1;) {
    a %= m; if (!a) return 0;
    int r = __builtin_ctz((unsigned)a);
    if ((r & 1) && ((m & 7) == 3 || (m & 7) == 5)) s = -s;
    a >>= r; if (a & m & 2) s = -s;
    swap(a, m);
  }
  return s;
}
// Cipolla: prime 2<=p<=INT_MAX; root or -1; expected O(log p).
int QuadraticResidue(int a, int p) {
  a %= p; if (a < 0) a += p;
  if (p == 2) return a;
  int jc = Jacobi(a, p), b, d;
  if (jc <= 0) return jc;
  do b = rand() % p, d = (1LL * b * b + p - a) % p;
  while (Jacobi(d, p) != -1);
  int f0 = b, f1 = 1, g0 = 1, g1 = 0, tmp;
  for (int e = (1LL + p) >> 1; e; e >>= 1) {
    if (e & 1) {
      tmp = (1LL * g0 * f0 + 1LL * d * (1LL * g1 * f1 % p)) % p;
      g1 = (1LL * g0 * f1 + 1LL * g1 * f0) % p, g0 = tmp;
    }
    tmp = (1LL * f0 * f0 + 1LL * d * (1LL * f1 * f1 % p)) % p;
    f1 = 2LL * f0 * f1 % p, f0 = tmp;
  }
  return g0;
}
