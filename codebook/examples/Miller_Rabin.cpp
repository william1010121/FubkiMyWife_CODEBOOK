bool prime(ll n) {
  if (n < 2) return false;
  for (ll a : {2,325,9375,28178,450775,9780504,1795265022})
    if (!Miller_Rabin(a, n)) return false;
  return true;
}
