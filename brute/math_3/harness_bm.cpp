#include "common.hpp"
struct F { static const int P=1000000007; int x; F(long long y=0):x((y%P+P)%P){} F operator+(F b)const{return x+b.x;} F operator-(F b)const{return x-b.x;} F operator-()const{return F(-x);} F operator*(F b)const{return 1LL*x*b.x;} F operator/(F b)const{long long a=b.x,e=P-2,r=1;for(;e;e>>=1,a=a*a%P)if(e&1)r=r*a%P;return F(x*r);} F& operator+=(F b){return x=(x+b.x)%P,*this;} F& operator-=(F b){return x=(x-b.x+P)%P,*this;} bool operator==(F b)const{return x==b.x;} bool operator!=(F b)const{return x!=b.x;} };
namespace tested {
#include "../../codebook/6_Math/Berlekamp-Massey.cpp"
}
int main(){mt19937 rng(88);for(int tc=0;tc<200;++tc){int k=1+rng()%6;vector<F> c(k);for(auto &x:c)x=F(rng()%F::P);vector<F>a(80);for(int i=0;i<k;++i)a[i]=F(rng()%F::P);for(int i=k;i<80;++i)for(int j=0;j<k;++j)a[i]+=c[j]*a[i-j-1];auto got=tested::BerlekampMassey(a);require_ok(got.size()<=size_t(k),"BM minimal size");for(int i=got.size();i<80;++i){F z;for(int j=0;j<(int)got.size();++j)z+=got[j]*a[i-j-1];require_ok(z==a[i],"BM recurrence");}}cout<<"berlekamp_massey: OK\n";}
