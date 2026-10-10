// O(p^k + log_p n), p prime, pk = p^k (k >= 1)
ll prod[MAXP];
ll fac_no_p(ll n, ll p, ll pk) {
	prod[0] = 1; for (int i = 1; i <= pk; ++i)
		if (i % p) prod[i] = prod[i - 1] * i % pk;
		else prod[i] = prod[i - 1];
	ll rt = 1; for (; n; n /= p) {
		// The product of all units has square 1: inversion permutes them.
		if ((n / pk) & 1) rt = rt * prod[pk] % pk;
		rt = rt * prod[n % pk] % pk;
		} return rt;
} // (n! without factor p) % p^k
