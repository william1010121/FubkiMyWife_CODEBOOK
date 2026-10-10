constexpr int MAXC = 10, MAXN = 1e5 + 10;
struct DBSeq { // FKM; C>=1, N>=0, 1<=K<=C^N, N<MAXC*MAXN
  int C, N, K, L, buf[MAXC * MAXN];
  void dfs(int *out, int t, int p, int &ptr) {
    if (ptr >= L) return; if (t > N) {
      if (N % p) return;
      for (int i = 1; i <= p && ptr < L; ++i) out[ptr++] = buf[i];
    } else { buf[t] = buf[t - p], dfs(out, t + 1, p, ptr);
      for (int j = buf[t - p] + 1; j < C && ptr < L; ++j)
        buf[t] = j, dfs(out, t + 1, t, ptr);
    }
  }
  void solve(int _c, int _n, int _k, int *out) { // L=N+K-1 output
    int p = 0; C = _c, N = _n, K = _k, L = N + K - 1, buf[0] = 0;
    if (C == 1) { fill_n(out, L, 0); return; }
    dfs(out, 1, 1, p); if (p < L) fill(out + p, out + L, 0);
  }
} dbs;
