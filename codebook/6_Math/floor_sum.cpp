// Sum floor((a*i+b)/m), 0<=i<n; n,a,b>=0,m>0. Log time.
ll floor_sum(ll n, ll m, ll a, ll b) {
  ll ans = 0;
  if (a >= m) ans += (n - 1) * n * (a / m) / 2, a %= m;
  if (b >= m) ans += n * (b / m), b %= m;
  ll y = a * n + b;
  return ans + (y < m ? 0 : floor_sum(y / m, a, m, y % m));
}
