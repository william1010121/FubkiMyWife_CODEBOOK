#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/6_Math/ax+by=gcd.cpp"
#include "../../codebook/6_Math/Linear_Congruence.cpp"
#include "../../codebook/6_Math/chineseRemainder.cpp"
#include "../../codebook/6_Math/ModMin.cpp"
#include "../../codebook/6_Math/floor_sum.cpp"
#include "../../codebook/6_Math/Fraction.cpp"
constexpr int MAXN = 8, MAXP = 10000;
#include "../../codebook/6_Math/Simultaneous_Equations.cpp"
#include "../../codebook/6_Math/Gaussian_gcd.cpp"
#include "../../codebook/6_Math/Mod_Arithmetic.cpp"
#include "../../codebook/6_Math/Mod_Binomial.cpp"
#include "../../codebook/6_Math/fac_no_p.cpp"
namespace integer_division {
#include "../../codebook/6_Math/floor_ceil.cpp"
}
ll mul(ll a, ll b, ll m) { return (__int128)a * b % m; }
#include "../../codebook/6_Math/Miller_Rabin.cpp"
bool prime(ll n) {
  if (n < 2) return false;
  for (ll a : {2,325,9375,28178,450775,9780504,1795265022})
    if (!Miller_Rabin(a, n)) return false;
  return true;
}
#include "../../codebook/6_Math/Pollard_Rho.cpp"
namespace mobius {
int n, tot;
vector<int> mu, flg, p;
#include "../../codebook/6_Math/getMu.cpp"
}
void sieve_and_inverses() {
  for (int n : {1,2,100,10000,1}) {
    mobius::n=n; mobius::tot=0;
    mobius::mu.assign(n+1,1234567); // mu need not start at zero
    mobius::flg.assign(n+1,0); mobius::p.assign(n+1,0);
    mobius::getMu();
    vector<int> primes;
    for (int x=1; x<=n; ++x) {
      int y=x, want=1;
      for (int p=2; p<=y/p; ++p) if (y%p==0) {
        y/=p; want=-want;
        if (y%p==0) want=0;
        while (y%p==0) y/=p;
      }
      if (y>1) want=-want;
      assert(mobius::mu[x]==want);
      if (prime(x)) primes.push_back(x);
    }
    assert(mobius::tot==(int)primes.size());
    for (int i=0; i<mobius::tot; ++i)
      assert(mobius::p[i+1]==primes[i]);
  }
  for (int mod : {2,3,97,2147483647}) {
    int N=1;
    vector<ll> inv(N+1,-7);
#include "../../codebook/6_Math/Mod_Inverse.cpp"
    assert(inv[0]==-7 && inv[1]==1);
  }
}

ll normalize(__int128 x, ll m) { return (x % m + m) % m; }
void bezout(ll a, ll b) {
  auto [x,y] = exgcd(a,b);
  assert((__int128)a*x + (__int128)b*y == gcd(a,b));
}
void congruence(ll a, ll b, ll n) {
  ll g = gcd(normalize(a,n),n);
  // Output must be affordable; also test inconsistent a=0 at large n.
  if (normalize(b,n) % g == 0 && g > 1000) return;
  auto v = mod_leq(a,b,n);
  if (normalize(b,n) % g) { assert(v.empty()); return; }
  assert((ll)v.size() == g && v[0] >= 0 && v[0] < n/g);
  for (ll i = 0; i < g; ++i) {
    assert(v[i] == v[0]+i*(n/g) && v[i] < n);
    assert(normalize((__int128)a*v[i]-b,n) == 0);
  }
}
void rationals() {
  for (ll n : {0LL,1LL,-1LL,LLONG_MAX,-LLONG_MAX})
    for (ll d : {1LL,-1LL,LLONG_MAX,-LLONG_MAX}) {
      fraction x(n,d);
      assert(x.d > 0 && gcd(x.n,x.d) == 1);
      assert((__int128)x.n*d == (__int128)n*x.d);
    }
  fraction a(LLONG_MAX), b(-LLONG_MAX);
  assert((a+b).n == 0 && (a+b).d == 1);
  assert((a/a).n == 1 && (a/a).d == 1);
  assert((fraction(0,-LLONG_MAX)).d == 1);
}
void exact_systems() {
  for (int n = 0; n <= MAXN; ++n)
    for (int m = 0; m <= MAXN; ++m) {
      matrix a; a.n=n; a.m=m;
      fill_n(a.sol,MAXN,fraction(17,19));
      assert(a.solve() == 0);
      for (int j = 0; j < m; ++j)
        assert(a.sol[j].n == 0 && a.sol[j].d == 1);
      for (int j = m; j < MAXN; ++j)
        assert(a.sol[j].n == 17 && a.sol[j].d == 19);
      if (n) {
        fill_n(a.sol,MAXN,fraction(17,19)); a.M[n-1][m]=1;
        assert(a.solve() == -1);
        for (auto x : a.sol) assert(x.n == 17 && x.d == 19);
      }
    }
  matrix a; a.n=4; a.m=4;
  for (int i = 0; i < 4; ++i) {
    a.M[i][1]=fraction(i+1,3); a.M[i][3]=fraction(i+1,5);
    a.M[i][4]=fraction(2*(i+1),3);
  }
  assert(a.solve()==1 && a.sol[1].n==2 && a.sol[1].d==1);
  assert(!a.sol[0].n && !a.sol[2].n && !a.sol[3].n);
  assert(a.solve()==1 && a.sol[1].n==2); // repeated solve
  a.M[3][4]=1; assert(a.solve()==-1);
}
void binomials() {
  for (ll p : {2LL,3LL,97LL,9223372036854775783LL}) {
    ModBinomial z(0,p);
    assert(z.C(0,0)==1 && z.C(-1,0)==0 && z.C(0,1)==0);
    assert(z.lucas(-1,0)==0 && z.lucas(0,-1)==0);
  }
  // Independent digit criterion: C(n,k) is odd iff k is a submask.
  ModBinomial z(1,2);
  mt19937_64 rng(507);
  for (int i = 0; i < 5000; ++i) {
    ll n=rng()&LLONG_MAX, k=rng()&LLONG_MAX;
    assert(z.lucas(n,k)==(k<=n && (k&n)==k));
  }
  ModBinomial c(50,9223372036854775783LL);
  unsigned long long row[51]={1};
  for (int n = 0; n <= 50; ++n) {
    for (int k = 0; k <= n; ++k) assert(c.C(n,k)==(ll)row[k]);
    if (n<50) for (int k=n+1; k>0; --k) row[k]+=row[k-1];
  }
}
void gaussian_integers() {
  using Z=complex<ll>;
  auto unit = [](Z z) { return norm(z)==1; };
  for (Z u : {Z(1,0),Z(-1,0),Z(0,1),Z(0,-1)}) {
    assert(gaussian_gcd(Z(0,0),u)==u);
    assert(unit(gaussian_gcd(Z(999999937,-999999938),u)));
    Z b(1000000000,1000000000);
    assert(gaussian_gcd(b*u,b)==b);
  }
  // Large coprime real inputs exercise safe rounding near ll limits.
  assert(unit(gaussian_gcd(Z(1500000000,0),Z(1499999999,0))));
}
int main() {
  bezout(0,0);
  ll f=1,g=1;
  while (f<=LLONG_MAX-g) { bezout(f,g); tie(f,g)=pll(g,f+g); }
  const vector<ll> edges={LLONG_MIN,LLONG_MIN+1,-1,0,1,
                          LLONG_MAX-1,LLONG_MAX};
  for (ll a : edges) for (ll b : edges) {
    for (ll n : {1LL,2LL,3LL,97LL,LLONG_MAX,LLONG_MAX-1})
      congruence(a,b,n);
  }
  mt19937_64 rng(5007);
  for (int i=0; i<20000; ++i) {
    bezout(rng()&LLONG_MAX,rng()&LLONG_MAX);
    ll a=(ll)rng(), b=(ll)rng(), n=1+rng()%LLONG_MAX;
    congruence(a,b,n);
    ll m1=1+rng()%3000000000LL, m2=1+rng()%3000000000LL;
    ll lcm=m1/gcd(m1,m2)*m2, x=solve(a,m1,b,m2);
    bool possible=normalize((__int128)b-a,gcd(m1,m2))==0;
    if (!possible) assert(x==-1);
    else {
      assert(x>=0 && x<lcm);
      assert(x%m1==normalize(a,m1) && x%m2==normalize(b,m2));
    }
    // Singleton interval oracle from the Bezout inverse, no k scan.
    ll m=1+rng()%2000000000LL, t=rng()%m, l=rng()%m;
    ll d=gcd(t,m), want=-1;
    if (l%d==0) want=normalize((__int128)exgcd(t,m).X*(l/d),m/d);
    assert(mod_min(t,m,l,l)==want);
  }
  for (int a : {INT_MIN,INT_MIN+1,-1,0,1,INT_MAX-1,INT_MAX})
    for (int b : {INT_MIN,-1,1,INT_MAX}) {
      if (a==INT_MIN && b==-1) continue;
      ll q=(ll)a/b, r=(ll)a%b;
      assert(integer_division::floor(a,b)==q-(r && (a<0)!=(b<0)));
      assert(integer_division::ceil(a,b)==q+(r && (a<0)==(b<0)));
    }
  assert(mod_min(0,LLONG_MAX,1,LLONG_MAX-1)==-1);
  assert(mod_min(1,LLONG_MAX,LLONG_MAX-1,LLONG_MAX-1)==LLONG_MAX-1);
  assert(floor_sum(LLONG_MAX,LLONG_MAX,0,LLONG_MAX)==LLONG_MAX);
  assert(floor_sum(0,1,LLONG_MAX,LLONG_MAX)==0);
  assert(floor_sum(1,1,LLONG_MAX,LLONG_MAX)==LLONG_MAX);
  assert(floor_sum(3000000000LL,1,1,0)==4499999998500000000LL);
  rationals(); exact_systems(); binomials(); gaussian_integers();
  sieve_and_inverses();
  // Distinguish a single witness from the complete prime wrapper.
  assert(Miller_Rabin(0,9) && Miller_Rabin(9,9));
  assert(Miller_Rabin(2,2047) && !prime(2047));
  assert(!Miller_Rabin(LLONG_MAX,LLONG_MIN));
  for (ll n : {1LL,9LL,27LL,LLONG_MAX}) {
    cnt.clear(); PollardRho(n); auto once=cnt;
    PollardRho(1); assert(cnt==once);
    PollardRho(n);
    for (auto [p,e] : once) assert(cnt[p]==2*e && prime(p));
    __int128 product=1;
    for (auto [p,e] : once) for (int i=0; i<e; ++i) product*=p;
    assert(product==n);
  }
  assert(fac_no_p(0,2,8192)==1);
  assert(fac_no_p(1,2,8192)==1);
  cout << "PASS round5 full-width arithmetic, empty/repeated systems, "
       << "large Gaussian integers, Lucas, witness/factor semantics\n";
}
