ll PrimeCount(ll n) { // n<=10^13; n<=1 -> 0; O(sqrt(n)) space.
  if (n <= 1) return 0;
  int v = sqrt(n), s = (v + 1) / 2, pc = 0;
  vector<int> smalls(s), roughs(s); vector<char> skip(v + 1);
  vector<ll> larges(s); // smalls[j] has odd bound 2*j+1.
  for (int i = 1; i < s; ++i) smalls[i] = i + 1;
  for (int i = 0; i < s; ++i)
    roughs[i] = 2 * i + 1, larges[i] = (n / roughs[i] + 1) / 2;
  for (int p = 3; p <= v; p += 2) {
    if (skip[p]) continue; int q = p * p; ++pc;
    if (1LL * q * q > n) break;
    skip[p] = 1;
    for (int i = q; i <= v; i += 2 * p) skip[i] = 1;
    int ns = 0; for (int k = 0; k < s; ++k) {
      int i = roughs[k]; if (skip[i]) continue;
      ll d = 1LL * i * p;
      larges[ns] = larges[k] - (d <= v ?
        larges[smalls[d / 2] - pc] : smalls[(n / d - 1) / 2]) + pc;
      roughs[ns++] = i;
    }
    s = ns; for (int j = (v / p - 1) / 2; j >= p / 2; --j) {
      int c = smalls[j] - pc, e = min((j + 1) * p + p / 2, (v + 1) / 2);
      for (int i = j * p + p / 2; i < e; ++i) smalls[i] -= c;
    }
  }
  for (int k = 1; k < s; ++k) {
    ll m = n / roughs[k], t = larges[k] - (pc + k - 1);
    for (int l = 1; l < k && 1LL * roughs[l] * roughs[l] <= m; ++l)
      t -= smalls[(m / roughs[l] - 1) / 2] - (pc + l - 1);
    larges[0] -= t;
  } return larges[0];
}
