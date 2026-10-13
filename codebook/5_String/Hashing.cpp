struct Hash { // mod 2^61-1; random base B >= 256
  using u = unsigned long long;
  static const u M = (1ULL << 61) - 1; vector<u> h, p;
  u mul(u a, u b) { __uint128_t c = (__uint128_t)a * b;
    c = (c & M) + (c >> 61); return c >= M ? c - M : c; }
  Hash(const string &s, u B) : h(SZ(s) + 1), p(h) {
    p[0] = 1; for (int i = 0; i < SZ(s); ++i)
      p[i + 1] = mul(p[i], B),
      h[i + 1] = (mul(h[i], B) + (uint8_t)s[i]) % M; }
  u get(int l, int r) { // hash of s[l,r)
    return (h[r] + M - mul(h[l], p[r - l])) % M; } };
