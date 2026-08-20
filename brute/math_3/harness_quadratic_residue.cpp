#include "common.hpp"
namespace tested {
#include "../../codebook/6_Math/QuadraticResidue.cpp"
}
int main() {
  srand(7);
  for (int p=2;p<=97;++p) {
    bool prime=p>1; for(int d=2;d*d<=p;++d) if(p%d==0) prime=false;
    if(!prime) continue;
    for(int a=0;a<p;++a){
      int want=-1; for(int x=0;x<p;++x) if(1LL*x*x%p==a){want=x;break;}
      int got=tested::QuadraticResidue(a,p);
      require_ok((want<0 ? got==-1 : (got>=0 && 1LL*got*got%p==a)),"quadratic residue");
      require_ok(tested::Jacobi(a,p)==(want<0?-1:(a==0?0:1)),"Jacobi");
    }
  }
  cout << "quadratic_residue: OK\n";
}
