#include "common.hpp"
namespace tested {
ll mul(ll a,ll b,ll m){return (ll)((__int128)a*b%m);}
ll fpow(ll a,ll e,ll m){ll r=1;for(;e;e>>=1,a=mul(a,a,m))if(e&1)r=mul(r,a,m);return r;}
bool prime(ll n){if(n<2)return false;for(ll d=2;d*d<=n;++d)if(n%d==0)return false;return true;}
#include "../../codebook/6_Math/Miller_Rabin.cpp"
#include "../../codebook/6_Math/Pollard_Rho.cpp"
#include "../../codebook/6_Math/DiscreteLog.cpp"
#include "../../codebook/6_Math/PiCount.cpp"
}
int main(){
  const ll bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
  for(ll n=2;n<=100000;++n){
    bool got = true;
    for (ll a : bases) got &= tested::Miller_Rabin(a,n);
    require_ok(got==tested::prime(n),"Miller-Rabin");
  }
  for(ll n=2;n<=5000;++n){tested::cnt.clear();tested::PollardRho(n);ll z=1;for(auto [p,e]:tested::cnt)for(int i=0;i<e;++i)z*=p;require_ok(z==n,"Pollard-Rho");}
  for(ll m=1;m<=12;++m)for(ll x=0;x<m;++x)for(ll y=0;y<m;++y){ll want=-1,v=1%m;for(ll k=0;k<=2*m+2;++k){if(v==y){want=k;break;}v=v*x%m;}ll got=tested::DiscreteLog(x,y,m);if(got!=want){cerr<<"discrete mismatch m="<<m<<" x="<<x<<" y="<<y<<" got="<<got<<" want="<<want<<'\n';return 1;}}
  mt19937 rng(0xD15C); for (int tc=0; tc<1500; ++tc) {
    ll m=1+rng()%500,x=rng()%m,y=rng()%m,want=-1,v=1%m;
    for(ll k=0;k<=2*m+2;++k){if(v==y){want=k;break;}v=v*x%m;}
    ll got=tested::DiscreteLog(x,y,m);if(got!=want){cerr<<"random discrete mismatch m="<<m<<" x="<<x<<" y="<<y<<" got="<<got<<" want="<<want<<'\n';return 1;}
  }
  vector<int> isp(10001,1);isp[0]=isp[1]=0;for(int i=2;i*i<=10000;++i)if(isp[i])for(int j=i*i;j<=10000;j+=i)isp[j]=0;for(int n=0;n<=10000;++n){int w=accumulate(isp.begin(),isp.begin()+n+1,0);require_ok(tested::PrimeCount(n)==w,"prime count");}
  cout<<"number_theory: OK\n";
}
