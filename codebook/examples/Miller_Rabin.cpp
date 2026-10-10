// Signed ll domain (through LLONG_MAX).
// Place mul() before Miller_Rabin.cpp; prime() after it.
ll mul(ll a, ll b, ll m) { // 0 <= a,b < m, m > 0
  return (ll)((__int128)a * b % m);
}
bool prime(ll n) { // negatives/0/1 are not prime
  if (n < 2) return false;
  for (ll a : {2,325,9375,28178,450775,9780504,1795265022})
    if (!Miller_Rabin(a, n)) return false;
  return true;
}
