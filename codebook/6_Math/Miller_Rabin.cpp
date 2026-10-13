// Witness a>=0; exact mul for 0<=x,y<n; use prime().
bool Miller_Rabin(ll a, ll n) {
  if (n < 2) return 0;
  if ((a %= n) == 0) return 1;
  if (n % 2 == 0) return n == 2;
  ll d = n - 1, x = a; int s = __builtin_ctzll(d);
  // d>>s is odd; x=a consumes its low bit.
  for (d >>= s + 1; d; d >>= 1) { a = mul(a, a, n);
    if (d & 1) x = mul(x, a, n); }
  if (x == 1 || x == n - 1) return 1;
  while (--s) if ((x = mul(x, x, n)) == n - 1) return 1;
  return 0;
}
