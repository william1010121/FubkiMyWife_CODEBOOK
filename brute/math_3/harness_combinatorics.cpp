#include "common.hpp"
namespace cantor {
#include "../../codebook/6_Math/cantor_expansion.cpp"
}
namespace fac {
const int MAXP=1000;
ll mpow(ll a,ll e,ll m){ll r=1;for(;e;e>>=1,a=a*a%m)if(e&1)r=r*a%m;return r;}
#include "../../codebook/6_Math/fac_no_p.cpp"
}
namespace mobius {
int n,tot,p[1000]; bool flg[1000]; int mu[1000];
#include "../../codebook/6_Math/getMu.cpp"
}
namespace inverse {
const int N=100; const int mod=101; int inv[N+1];
void init() {
#include "../../codebook/6_Math/Mod_Inverse.cpp"
}
}
int main(){
  cantor::init();
  for(int n=1;n<=7;++n){vector<int> p(n);iota(p.begin(),p.end(),0);int cnt=0;do{int z=cantor::encode(p);require_ok(cantor::decode(z,n)==p,"Cantor roundtrip");++cnt;}while(next_permutation(p.begin(),p.end()));require_ok(cnt==cantor::factorial[n],"Cantor count");}
  for(ll p=2;p<=11;++p){for(ll n=0;n<=100;++n){ll pk=1;for(int i=0;i<3;++i)pk*=p;ll x=1;for(ll i=1;i<=n;++i){ll t=i;while(t%p==0)t/=p;x=x*(t%pk)%pk;}fac::prod[0]=0;require_ok(fac::fac_no_p(n,p,pk)==x,"factorial without p");}}
  mobius::n=200;mobius::tot=0;fill(mobius::flg,mobius::flg+201,0);fill(mobius::mu,mobius::mu+201,0);mobius::getMu();
  for(int x=1;x<=200;++x){int c=0,y=x;bool square=false;for(int p=2;p*p<=y;++p)if(y%p==0){int e=0;while(y%p==0)y/=p,++e;if(e>=2)square=true;++c;}if(y>1)++c;int w=square?0:(c&1?-1:1);require_ok(mobius::mu[x]==w,"Mobius");}
  inverse::init(); for(int i=1;i<=100;++i) require_ok(1LL*i*inverse::inv[i]%inverse::mod==1,"linear inverse");
  cout<<"combinatorics: OK\n";
}
