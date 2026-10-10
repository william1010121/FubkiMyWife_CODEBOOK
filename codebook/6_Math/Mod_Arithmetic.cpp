// Signed inputs; positive ll modulus, results in [0,m).
struct ModArithmetic {
  using ll = long long;
  using i128 = __int128;
  ll m;
  explicit ModArithmetic(ll m) : m(m) { assert(m > 0); }
  ll norm(ll a) const { a %= m; return a < 0 ? a + m : a; }
  ll add(ll a, ll b) const {
    return (i128(norm(a)) + norm(b)) % m;
  }
  ll sub(ll a, ll b) const {
    return (i128(norm(a)) - norm(b) + m) % m;
  }
  ll mul(ll a, ll b) const {
    return i128(norm(a)) * norm(b) % m;
  }
  ll pow(ll a, ll e) const { // e >= 0; O(log e)
    assert(e >= 0);
    ll r = 1 % m; a = norm(a);
    for (; e; e >>= 1, a = mul(a, a))
      if (e & 1) r = mul(r, a);
    return r;
  }
  ll inv(ll a) const { // O(log m); -1 if m=1 or gcd(a,m)!=1
    if (m == 1) return -1;
    ll r = norm(a), s = m;
    i128 x = 1, y = 0;
    while (s) {
      ll q = r / s, t = r % s;
      r = s; s = t;
      i128 z = x - i128(q) * y; x = y; y = z;
    }
    if (r != 1) return -1;
    x %= m; return x < 0 ? x + m : x;
  }
};
