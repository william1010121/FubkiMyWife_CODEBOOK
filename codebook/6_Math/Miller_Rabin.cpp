// One witness; use the prime() wrapper in Usage.
bool Miller_Rabin(ll a, ll n) {
	if (n < 2) return 0;
	if ((a = a % n) == 0) return 1;
	if (n % 2 == 0) return n == 2;
	ll tmp = n - 1,
	  t = __builtin_ctzll((unsigned long long)tmp), x = 1;
	tmp >>= t;
		for (; tmp; tmp >>= 1, a = mul(a, a,
		  n)) if (tmp & 1) x = mul(x, a, n);
		if (x == 1 || x == n - 1) return 1;
		while (--t) if ((x = mul(x,
		  x, n)) == n - 1) return 1;
  return 0;
}
