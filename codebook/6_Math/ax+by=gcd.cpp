pll exgcd(ll a, ll b) { // a,b>=0; all ll intermediates must fit.
  if (!b) return {1, 0};
  auto p = exgcd(b, a % b); return {p.Y, p.X - p.Y * (a / b)};
}
// ax+by=res, a>=0,b>0; g=gcd(a,b) must divide res.
// (x,y)=exgcd(a,b)*res/g; x'=x mod (b/g) in [0,b/g).
// y' = y - (x'-x)/(b/g)*(a/g).
