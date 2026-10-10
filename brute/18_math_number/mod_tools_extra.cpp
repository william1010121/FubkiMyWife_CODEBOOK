#include <cassert>
#include <vector>
#include <utility>
#include <numeric>
#include <random>
#include <iostream>
#include <climits>
#include <cstdlib>
#include <algorithm>
using namespace std;
#include "../../codebook/6_Math/Mod_Arithmetic.cpp"
#include "../../codebook/6_Math/Mod_Binomial.cpp"
#include "../../codebook/6_Math/QuadraticResidue.cpp"
using ll = long long;
ll norm(ll a,ll m){ __int128 t=a; return (t%m+m)%m; }
ll mul_oracle(ll a,ll b,ll m){
  unsigned long long x=norm(a,m),y=norm(b,m),r=0;
  for(;y;y>>=1,x=(x+x)%m)if(y&1)r=(r+x)%m;
  return r;
}
int legendre(int a,int p){
  a=norm(a,p); if(!a)return 0;
  for(int x=1;x<p;x++)if(1LL*x*x%p==a)return 1;
  return -1;
}
int jacobi_oracle(int a,int m){
  int r=1;
  for(int p=3;p<=m;p+=2) while(m%p==0){r*=legendre(a,p);m/=p;}
  return r;
}
int main(){
  mt19937_64 rng(20261009);
  for(ll m=1;m<=150;m++){
    ModArithmetic z(m);
    for(ll a=-200;a<=200;a++){
      ll want=-1;
      if(m>1)for(ll x=0;x<m;x++)if(norm(a,m)*x%m==1){want=x;break;}
      assert(z.inv(a)==want);
      ll power=1%m;
      for(int e=0;e<=20;e++){assert(z.pow(a,e)==power);power=mul_oracle(power,a,m);}
      for(ll b=-40;b<=40;b++){
        assert(z.norm(a)==norm(a,m));
        assert(z.add(a,b)==norm(a+b,m));
        assert(z.sub(a,b)==norm(a-b,m));
        assert(z.mul(a,b)==mul_oracle(a,b,m));
      }
    }
  }
  for(ll m: {1LL,2LL,12LL,1000000007LL,LLONG_MAX,LLONG_MAX-1}){
    ModArithmetic z(m);
    for(int i=0;i<4000;i++){
      ll a=(ll)rng(),b=(ll)rng();
      assert(z.mul(a,b)==mul_oracle(a,b,m));
      assert(z.add(a,b)==((__int128)norm(a,m)+norm(b,m))%m);
      assert(z.sub(a,b)==((__int128)norm(a,m)-norm(b,m)+m)%m);
      ll inverse=z.inv(a);
      if(m==1||gcd(norm(a,m),m)!=1)assert(inverse==-1);
      else assert(inverse>=0&&mul_oracle(a,inverse,m)==1);
    }
    assert(z.mul(LLONG_MIN,LLONG_MIN)==mul_oracle(LLONG_MIN,LLONG_MIN,m));
  }
  for(int p: {2,3,5,7,11,97,1000000007}){
    int N=min(p-1,300); ModBinomial c(N,p);
    vector<ll> row(302); row[0]=1;
    for(int n=0;n<=300;n++){
      for(int k=0;k<=n;k++){
        if(n<=N)assert(c.C(n,k)==row[k]);
        if(N==p-1)assert(c.lucas(n,k)==row[k]);
      }
      for(int k=n+1;k>0;k--)row[k]=(row[k]+row[k-1])%p;
      assert(c.C(n,-1)==0);assert(c.C(n,n+1)==0);
    }
  }
  srand(7);
  for(int m=1;m<=151;m+=2)
    for(int a=-300;a<=300;a++)assert(Jacobi(a,m)==jacobi_oracle(a,m));
  for(int p=2;p<=1000;p++){
    bool prime=true; for(int d=2;d*d<=p;d++)if(p%d==0)prime=false;
    if(!prime)continue;
    vector<bool> has(p);for(int x=0;x<p;x++)has[1LL*x*x%p]=true;
    for(int a=-2*p;a<=2*p;a++){
      int r=QuadraticResidue(a,p),v=norm(a,p);
      assert(has[v]?(r>=0&&1LL*r*r%p==v):r==-1);
    }
  }
  for(int i=0;i<1000;i++){
    int p=INT_MAX, x=rng()%p,a=1LL*x*x%p;
    int r=QuadraticResidue(a,p);
    assert(r>=0&&1LL*r*r%p==a);
    assert(Jacobi(a,p)==(a?1:0));
  }
  int r=QuadraticResidue(INT_MIN,INT_MAX);
  assert(r==-1); // -1 is a non-residue for p = 3 mod 4.
  cout<<"PASS modular arithmetic, prime binomial/Lucas, Jacobi/Cipolla boundaries\n";
}
