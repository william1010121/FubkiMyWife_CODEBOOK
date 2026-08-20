#include "common.hpp"
namespace tested {
#include "../../codebook/6_Math/ax+by=gcd.cpp"
#include "../../codebook/6_Math/ax=b%n.cpp"
#include "../../codebook/6_Math/chineseRemainder.cpp"
#include "../../codebook/6_Math/floor_ceil.cpp"
#include "../../codebook/6_Math/floor_sum.cpp"
}
int main() {
  for (ll a=-30;a<=30;++a) for(ll b=-30;b<=30;++b) if(b){
    require_ok(tested::floor(a,b)==(ll)floor((long double)a/b),"floor division");
    require_ok(tested::ceil(a,b)==(ll)ceil((long double)a/b),"ceil division");
  }
  for (ll a=0;a<=40;++a) for(ll b=0;b<=40;++b) if(a||b){
    ll g=std::gcd(a,b); auto z=tested::exgcd(a,b);
    require_ok(a*z.X+b*z.Y==g,"exgcd");
  }
  for(ll a=0;a<30;++a) for(ll n=1;n<=30;++n) for(ll b=0;b<n;++b){
    auto got=tested::mod_leq(a,b,n); vector<ll> want;
    for(ll x=0;x<n;++x) if(a*x%n==b) want.pb(x);
    sort(got.begin(),got.end()); sort(want.begin(),want.end()); require_ok(got==want,"linear congruence");
  }
  for(ll m=1;m<=20;++m) for(ll n=1;n<=20;++n) for(ll x=0;x<m;++x) for(ll y=0;y<n;++y){
    ll l=std::lcm(m,n), want=-1; for(ll z=0;z<l;++z) if(z%m==x&&z%n==y){want=z;break;}
    require_ok(tested::solve(x,m,y,n)==want,"CRT");
  }
  for(ll n=0;n<=30;++n) for(ll m=1;m<=30;++m) for(ll a=0;a<60;++a) for(ll b=0;b<60;++b){
    ll want=0; for(ll i=0;i<n;++i) want+=(a*i+b)/m;
    require_ok(tested::floor_sum(n,m,a,b)==want,"floor sum");
  }
  cout << "floor_mod: OK\n";
}
