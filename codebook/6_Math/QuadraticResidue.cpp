// Jacobi: positive odd modulus; O(log m).
int Jacobi(int a, int m) {
  a %= m; if (a < 0) a += m; int s = 1;
  for (; m > 1;) {
    a %= m; if (!a) return 0;
    int r = __builtin_ctz((unsigned)a);
    if ((r & 1) && ((m & 7) == 3 || (m & 7) == 5)) s = -s;
    a >>= r; if (a & m & 2) s = -s;
    swap(a, m);
  } return s;
}
// Prime 2<=p<=INT_MAX; root or -1; expected O(log p).
int QuadraticResidue(int a, int p) {
  a %= p; if (a < 0) a += p; if (p == 2) return a;
  if (p % 4 == 3) { ll r = 1, b = a; // a^((p+1)/4).
    for (int e = (1LL+p)/4; e; e >>= 1, b = b*b%p)
      if (e & 1) r = r*b%p;
    return r*r%p == a ? r : -1; }
  int jc = Jacobi(a, p), b, d;
  if (jc <= 0) return jc;
  do b = rand() % p, d = (1LL * b * b + p - a) % p;
  while (Jacobi(d, p) != -1);
  auto mul = [&](pair<ll, ll> x, pair<ll, ll> y) {
    auto [u, v] = x; auto [s, t] = y;
    return pair{(u*s + d*(v*t%p))%p, (u*t + v*s)%p}; };
  pair<ll, ll> f{b, 1}, g{1, 0}; // x + y*sqrt(d).
  for (int e = (1LL + p) >> 1; e; e >>= 1, f = mul(f, f))
    if (e & 1) g = mul(g, f);
  return g.first; }
