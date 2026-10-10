// One witness a>=0; exact mul(x,y,n) for 0<=x,y<n; see prime() in Usage.
bool Miller_Rabin(ll a, ll n) {
  if (n < 2) return 0;
  if ((a %= n) == 0) return 1;
  if (n % 2 == 0) return n == 2;
  ll d = n - 1, x = 1; int s = __builtin_ctzll(d);
  for (d >>= s; d; d >>= 1, a = mul(a, a, n))
    if (d & 1) x = mul(x, a, n);
  if (x == 1 || x == n - 1) return 1;
  while (--s) if ((x = mul(x, x, n)) == n - 1) return 1;
  return 0;
}
