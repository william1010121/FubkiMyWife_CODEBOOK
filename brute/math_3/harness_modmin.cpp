#include "common.hpp"
namespace tested {
#include "../../codebook/6_Math/ModMin.cpp"
}
int main(){
  for(ll m=1;m<=100;++m) for(ll a=0;a<2*m;++a) for(ll l=0;l<m;++l) for(ll r=l;r<m;++r){
    ll want=-1; for(ll k=0;k<=m+1;++k) if(l<=a*k%m&&a*k%m<=r){want=k;break;}
    require_ok(tested::mod_min(a,m,l,r)==want,"mod_min");
  }
  cout << "mod_min: OK\n";
}
