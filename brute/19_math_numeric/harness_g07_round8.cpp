#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/6_Math/ax+by=gcd.cpp"
#include "../../codebook/6_Math/Linear_Congruence.cpp"
#include "../../codebook/6_Math/chineseRemainder.cpp"
#include "../../codebook/6_Math/ModMin.cpp"
#include "../../codebook/6_Math/floor_sum.cpp"
#include "../../codebook/6_Math/Fraction.cpp"
constexpr int MAXN = 8, MAXP = 20000;
#include "../../codebook/6_Math/Simultaneous_Equations.cpp"
#include "../../codebook/6_Math/Gaussian_gcd.cpp"
#include "../../codebook/6_Math/Gaussian_Eliminatin.cpp"
#include "../../codebook/6_Math/Mod_Arithmetic.cpp"
#include "../../codebook/6_Math/Mod_Binomial.cpp"
#include "../../codebook/6_Math/fac_no_p.cpp"
namespace division {
#include "../../codebook/6_Math/floor_ceil.cpp"
}
namespace mobius {
int n, tot;
vector<int> mu, flg, p;
#include "../../codebook/6_Math/getMu.cpp"
}
ll mul(ll a, ll b, ll m) {
  assert(m > 0 && a >= 0 && a < m && b >= 0 && b < m);
  return (__int128)a*b%m;
}
#include "../../codebook/6_Math/Miller_Rabin.cpp"
bool prime(ll n) {
  for (ll a : {2,325,9375,28178,450775,9780504,1795265022})
    if (!Miller_Rabin(a,n)) return false;
  return true;
}
#include "../../codebook/6_Math/Pollard_Rho.cpp"
mt19937_64 rng(80807);
ll norm_mod(__int128 a, ll m) { return (a%m+m)%m; }

void modular_intervals() {
  // Large modulus, short period, intervals between reachable residues.
  for (int tc = 0; tc < 20000; ++tc) {
    ll period=1+rng()%500, scale=1+rng()%2000000;
    ll m=period*scale, a=(rng()%period)*scale;
    ll l=rng()%m, r=l+rng()%(m-l), want=-1;
    for (ll k=0; k<period; ++k) {
      ll v=a*k%m;
      if (v>=l && v<=r) { want=k; break; }
    }
    assert(mod_min(a,m,l,r)==want);
    ll g=gcd(a,m);
    // Skip consistent queries whose output is too large for this audit.
    if (l%g || g<=1000) {
      auto v=mod_leq(a,l,m);
      if (l%g) assert(v.empty());
      else assert(!v.empty() && a*v[0]%m==l);
    }
  }
}

void division_and_sums() {
  for (int tc=0; tc<20000; ++tc) {
    int a=(int)rng(), b=(int)rng();
    if (!b || (a==INT_MIN && b==-1)) continue;
    ll f=division::floor(a,b), c=division::ceil(a,b);
    if (b>0) {
      assert(f*b<=a && (f+1)*b>a);
      assert(c*b>=a && (c-1)*b<a);
    } else {
      assert(f*b>=a && (f+1)*b<a);
      assert(c*b<=a && (c-1)*b>a);
    }
    ll n=rng()%51, limit=LLONG_MAX/(n*n+n+1);
    ll m=1+rng()%limit, x=rng()%limit, y=rng()%limit;
    __int128 want=0;
    for (ll i=0; i<n; ++i) want+=((__int128)x*i+y)/m;
    assert(floor_sum(n,m,x,y)==want);
  }
}

void diophantine_and_crt() {
  for (int tc=0; tc<20000; ++tc) {
    ll a=rng()%2000000001, b=1+rng()%2000000000;
    ll g=gcd(a,b), res=((ll)(rng()%200001)-100000)*g;
    auto [x,y]=exgcd(a,b); x*=res/g; y*=res/g;
    ll step=b/g, shifted=norm_mod(x,step);
    y-=(shifted-x)/step*(a/g);
    assert((__int128)a*shifted+(__int128)b*y==res);
    assert(shifted>=0 && shifted<step);
    ll m1=(1+rng()%100000)*g, m2=(1+rng()%100000)*g;
    // Cap the lcm so the merge meets its printed ll precondition.
    if ((__int128)m1/gcd(m1,m2)*m2>LLONG_MAX) continue;
    ll z=(ll)rng(), w=(ll)rng(), got=solve(z,m1,w,m2);
    if (((__int128)w-z)%gcd(m1,m2)) assert(got==-1);
    else {
      assert(got>=0 && got<m1/gcd(m1,m2)*m2);
      assert(got%m1==norm_mod(z,m1) && got%m2==norm_mod(w,m2));
    }
  }
}

ll binomial_unit_oracle(ll n, int k, ll p) {
  // Cancel p-adic valuations before division; independent of Lucas digits.
  ll num=1, den=1; int exponent=0;
  for (int i=0; i<k; ++i) {
    ll a=n-i, b=i+1;
    while (a%p==0) a/=p, ++exponent;
    while (b%p==0) b/=p, --exponent;
    num=num*(a%p)%p; den=den*b%p;
  }
  assert(exponent>=0);
  if (exponent) return 0;
  for (ll inv=1; inv<p; ++inv) if (den*inv%p==1)
    return num*inv%p;
  abort();
}
void binomials_and_factorials() {
  for (ll p : {2,3,5,7,31,97}) {
    ModBinomial c(p-1,p);
    for (int tc=0; tc<1500; ++tc) {
      ll n=tc%2 ? LLONG_MAX-tc : rng()&LLONG_MAX;
      int k=min(n,(ll)(rng()%301));
      ll want=binomial_unit_oracle(n,k,p);
      assert(c.lucas(n,k)==want && c.lucas(n,n-k)==want);
    }
  }
  for (auto [p,pk] : {pll{2,2},pll{2,4},pll{2,8},pll{2,4096},
                      pll{3,6561},pll{5,3125},pll{97,9409}}) {
    ll want=1;
    for (ll n=0; n<=3*pk+1; ++n) {
      if (n) { ll x=n; while (x%p==0) x/=p; want=want*x%pk; }
      if (n<=2 || n%pk<=1 || n%pk==pk-1)
        assert(fac_no_p(n,p,pk)==want);
    }
  }
}

void sieve_and_inverses() {
  for (int mod : {2,3,97,65537,INT_MAX}) {
    int N=min(1000,mod-1);
    vector<ll> inv(N+1,-19);
#include "../../codebook/6_Math/Mod_Inverse.cpp"
    assert(inv[0]==-19);
    for (int i=1; i<=N; ++i) assert(i*inv[i]%mod==1);
  }
  for (int n : {1,2,3,1023,1024,1025,8191,8192,8193}) {
    mobius::n=n; mobius::tot=0;
    mobius::mu.assign(n+1,91); mobius::flg.assign(n+1,0);
    mobius::p.assign(n+1,0); mobius::getMu();
    vector<int> sum(n+1);
    for (int d=1; d<=n; ++d)
      for (int x=d; x<=n; x+=d) sum[x]+=mobius::mu[d];
    for (int x=1; x<=n; ++x) assert(sum[x]==(x==1));
  }
}

void fractions_and_gaussian_integers() {
  for (int tc=0; tc<20000; ++tc) {
    ll an=(ll)(rng()%2000000001)-1000000000;
    ll bn=(ll)(rng()%2000000001)-1000000000;
    fraction a(an,1+rng()%1000000000), b(bn,1+rng()%1000000000);
    for (int op=0; op<4; ++op) {
      if (op==3 && !bn) continue;
      __int128 n, d;
      fraction got;
      if (op<2) {
        n=(__int128)a.n*b.d+(op ? -1 : 1)*(__int128)b.n*a.d;
        d=(__int128)a.d*b.d; got=op ? a-b : a+b;
      } else if (op==2) {
        n=(__int128)a.n*b.n; d=(__int128)a.d*b.d; got=a*b;
      } else {
        n=(__int128)a.n*b.d; d=(__int128)a.d*b.n; got=a/b;
      }
      assert((__int128)got.n*d==n*got.d);
      assert(got.d>0 && gcd(got.n,got.d)==1);
    }
    using Z=complex<ll>;
    Z g((ll)(rng()%20001)-10000,(ll)(rng()%20001)-10000);
    if (g==Z{}) continue;
    Z z((ll)(rng()%20001)-10000,(ll)(rng()%20001)-10000);
    if (z==Z(-1,0)) continue; // b must be nonzero.
    Z a1=g*z, b1=g*(z+Z(1,0)), got=gaussian_gcd(a1,b1);
    // gcd(z,z+1) is a unit, so this tests maximality at large norms.
    assert(norm(got)==norm(g));
    for (Z v : {a1,b1}) {
      ll dot=v.real()*got.real()+v.imag()*got.imag();
      ll cross=v.imag()*got.real()-v.real()*got.imag();
      assert(dot%norm(got)==0 && cross%norm(got)==0);
    }
  }
}

void rank_deficient_systems() {
  for (int n=0; n<=MAXN; ++n) for (int m=0; m<=MAXN; ++m)
    for (int rank=0; rank<=min(n,m); ++rank)
      for (int repetition=0; repetition<10; ++repetition) {
    matrix a; a.n=n; a.m=m;
    vector<int> cols(a.m); iota(ALL(cols),0);
    shuffle(ALL(cols),rng); cols.resize(rank); sort(ALL(cols));
    vector<int> x(a.m); for (int j : cols) x[j]=(int)(rng()%7)-3;
    vector<vector<ll>> basis(rank,vector<ll>(a.m+1));
    for (int i=0; i<rank; ++i) {
      basis[i][cols[i]]=1; basis[i][a.m]=x[cols[i]];
      for (int j=cols[i]+1; j<a.m; ++j)
        if (!binary_search(ALL(cols),j)) basis[i][j]=(int)(rng()%5)-2;
    }
    vector<vector<ll>> rows(a.n,vector<ll>(a.m+1));
    for (int i=0; i<a.n && rank; ++i) {
      int last=min(i,rank-1);
      for (int k=0; k<=last; ++k) {
        ll t=k==last ? 1 : (int)(rng()%3)-1;
        for (int j=0; j<=a.m; ++j) rows[i][j]+=t*basis[k][j];
      }
    }
    shuffle(ALL(rows),rng);
    for (int i=0; i<a.n; ++i) for (int j=0; j<=a.m; ++j)
      a.M[i][j]=fraction(rows[i][j],1LL<<(i%3));
    fill_n(a.sol,MAXN,fraction(17,19));
    assert(a.solve()==rank);
    for (int j=0; j<a.m; ++j)
      assert(a.sol[j].n==x[j] && a.sol[j].d==1);
    for (int j=a.m; j<MAXN; ++j)
      assert(a.sol[j].n==17 && a.sol[j].d==19);
    assert(a.solve()==rank);
    if (rank<a.n) {
      a.M[rank][a.m]=1;
      auto old=a;
      assert(a.solve()==-1);
      for (int j=0; j<MAXN; ++j)
        assert(a.sol[j].n==old.sol[j].n && a.sol[j].d==old.sol[j].d);
    }
  }
  // Known RREF, skipped coefficient columns, duplicates and two RHSs.
  for (int tc=0; tc<1000; ++tc) {
    vector<vector<double>> a={{0,1,2,0,3,5},{0,0,0,1,-2,7},
                             {0,2,4,1,4,17},{0,1,2,-1,5,-2}};
    shuffle(ALL(a),rng);
    for (int i=0; i<4; ++i) for (double &v : a[i])
      v*=ldexp(tc%2 ? -1.0 : 1.0,i-2);
    GAS(a);
    vector<vector<double>> want={{0,1,2,0,3,5},{0,0,0,1,-2,7},
                                {0,0,0,0,0,0},{0,0,0,0,0,0}};
    for (int i=0; i<4; ++i) for (int j=0; j<6; ++j)
      assert(abs(a[i][j]-want[i][j])<1e-11);
  }
}

void witnesses_and_factors() {
  for (ll n=0; n<=200; ++n) for (ll a=0; a<=n+2; ++a) {
    bool want=false;
    if (n>=2) {
      if (a%n==0) want=true; // Congruent-zero witness is ignored.
      else if (n%2==0) want=n==2;
      else {
        ll d=n-1; while (d%2==0) d/=2;
        ll v=1; for (ll i=0; i<d; ++i) v=v*a%n;
        want=v==1;
        for (; d<n-1; d*=2, v=v*v%n) want|=v==n-1;
      }
    }
    assert(Miller_Rabin(a,n)==want);
  }
  vector<ll> ps={2,3,5,7,11,97,65537,1000000007,2147483647};
  // Squares/products near the signed limit and mixed repeated factors.
  for (ll p : ps) for (ll q : ps) for (int e : {1,2,3}) {
    __int128 n=(__int128)p*q; map<ll,int> want;
    ++want[p]; ++want[q];
    for (int i=1; i<e; ++i) n*=3, ++want[3];
    if (n>LLONG_MAX) continue;
    cnt.clear(); PollardRho((ll)n); assert(cnt==want);
  }
}
int main() {
  modular_intervals(); division_and_sums(); diophantine_and_crt();
  binomials_and_factorials(); sieve_and_inverses();
  fractions_and_gaussian_integers(); rank_deficient_systems();
  witnesses_and_factors();
  cout << "PASS round8: structured intervals, p-adic binomials, "
       << "large Gaussian gcds, rank-deficient systems and factors\n";
}
