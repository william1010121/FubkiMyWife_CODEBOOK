// Prime int mod, 1<=N<mod; allocate inv[0..N]. O(N).
inv[1] = 1;
for (int i=2; i<=N; ++i) inv[i]=1LL*(mod-mod/i)*inv[mod%i]%mod;
