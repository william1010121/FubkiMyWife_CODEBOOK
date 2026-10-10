constexpr int MAXC = 10, MAXN = 1e5 + 10;
struct DBSeq { // FKM; C>=1, N>=0, 1<=K<=C^N, N<MAXC*MAXN
  // Valid out[0,L), disjoint from *this; L=N+K-1 fits int.
  // K>1,N>1 need O(N) stack.
  int C, N, K, L, buf[MAXC * MAXN];
  void dfs(int *out, int t, int p, int &ptr) {
    if (ptr >= L) return; if (t > N) {
      if (N % p) return; int k = min(p, L-ptr);
      copy_n(buf+1, k, out+ptr), ptr += k;
    } else { buf[t] = buf[t - p], dfs(out, t + 1, p, ptr);
      for (int j = buf[t - p] + 1; j < C && ptr < L; ++j)
        buf[t] = j, dfs(out, t + 1, t, ptr);
    }
  }
  void solve(int _c, int _n, int _k, int *out) {
    int p = 0; C = _c, N = _n, K = _k;
    L = N + (K-1), buf[0] = 0;
    if (N == 1) { iota(out, out + L, 0); return; }
    if (K == 1) { fill_n(out, L, 0); return; }
    dfs(out, 1, 1, p);
    if (p < L) fill(out + p, out + L, 0);
  }
} dbs;
