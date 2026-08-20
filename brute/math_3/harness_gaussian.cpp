#include "common.hpp"
template<class T> using V = vector<T>;
using cpx = complex<ll>;
namespace gauss {
#include "../../codebook/6_Math/Gaussian_Eliminatin.cpp"
}
namespace ggcd {
#include "../../codebook/6_Math/Gaussian_gcd.cpp"
}
static bool divides_cpx(cpx a,cpx b){ll n=a.real()*a.real()+a.imag()*a.imag();ll cr=b.real()*a.real()+b.imag()*a.imag(),ci=b.imag()*a.real()-b.real()*a.imag();return cr%n==0&&ci%n==0;}
int main(){
  for(int tc=0;tc<100;++tc){int n=1+tc%5;vector<vector<double>> a(n,vector<double>(n+1));vector<double>x(n);for(int i=0;i<n;++i)x[i]=i-2;for(int i=0;i<n;++i){for(int j=0;j<n;++j)a[i][j]=(i==j); a[i][n]=x[i];}gauss::GAS(a);for(int i=0;i<n;++i)require_ok(abs(a[i][i]-1)<1e-9&&abs(a[i][n]-x[i])<1e-7,"Gaussian elimination");}
  for(int ar=-8;ar<=8;++ar)for(int ai=-8;ai<=8;++ai)for(int br=-8;br<=8;++br)for(int bi=-8;bi<=8;++bi)if(br||bi){cpx a(ar,ai),b(br,bi),g=ggcd::gaussian_gcd(a,b);require_ok(divides_cpx(g,a)&&divides_cpx(g,b),"Gaussian gcd divisibility");}
  cout<<"gaussian: OK\n";
}
