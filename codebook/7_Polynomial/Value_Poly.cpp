struct Poly { // Prime-field mint with canonical .v; 1<=size<=P.
  mint base; // f(base+i)=poly[i]; inegfac[i]=(-1)^i/i!
  vector<mint> poly; // ifac, inegfac precomputed through size-1.
  Poly(mint b = 0, mint x = 0): base(b), poly(1, x) {}
  mint get_val(const mint &x) { // O(n) time/space
    int n = SZ(poly); mint t = x - base, rt = 0, suf = 1;
    if (t.v < n) return poly[t.v];
    vector<mint> pre(n, 1);
    for (int i = 1; i < n; ++i) pre[i] = pre[i - 1] * (t - (i - 1));
    for (int i = n - 1; i >= 0; --i) {
      rt += poly[i] * ifac[i] * inegfac[n - 1 - i] * pre[i] * suf;
      suf *= t - i;
    }
    return rt;
  }
  void raise() { // n<P; g(x)=sum_{k=base}^x f(k); O(n)
    if (SZ(poly) == 1 && poly[0] == 0) return;
    mint nw = get_val(base + SZ(poly)); poly.pb(nw);
    for (int i = 1; i < SZ(poly); ++i) poly[i] += poly[i - 1];
  }
};
