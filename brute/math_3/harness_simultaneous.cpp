#include "common.hpp"
#define MAXN 8
namespace tested {
#include "../../codebook/6_Math/Fraction.cpp"
#include "../../codebook/6_Math/Simultaneous_Equations.cpp"
}
int main(){
  tested::matrix z;z.n=2;z.m=2;z.M[0][0]=tested::fraction(0);z.M[0][1]=tested::fraction(1);z.M[0][2]=tested::fraction(2);z.M[1][0]=tested::fraction(1);z.M[1][1]=tested::fraction(0);z.M[1][2]=tested::fraction(3);require_ok(z.solve()==2&&z.sol[0].n==3&&z.sol[1].n==2,"row pivot");
  mt19937 rng(66);for(int tc=0;tc<200;++tc){int n=1+rng()%5,m=1+rng()%5;tested::matrix a;a.n=n;a.m=m;for(int i=0;i<n;++i)for(int j=0;j<=m;++j)a.M[i][j]=tested::fraction((int)(rng()%7)-3);auto original=a;int got=a.solve(); // exact rank/inconsistency is checked by a long-double oracle
    vector<vector<long double>> q(n,vector<long double>(m+1));for(int i=0;i<n;++i)for(int j=0;j<=m;++j)q[i][j]=original.M[i][j].n/(long double)original.M[i][j].d;int row=0;for(int c=0;c<m&&row<n;++c){int p=row;while(p<n&&fabsl(q[p][c])<1e-12)p++;if(p==n)continue;swap(q[p],q[row]);for(int i=0;i<n;++i)if(i!=row){long double t=q[i][c]/q[row][c];for(int j=c;j<=m;++j)q[i][j]-=t*q[row][j];}row++;}bool bad=0;for(auto&r:q){bool zero=1;for(int j=0;j<m;++j)zero&=fabsl(r[j])<1e-9;if(zero&&fabsl(r[m])>1e-9)bad=1;}require_ok((got==-1)==bad,"linear system consistency");if(!bad)require_ok(got==row,"linear system rank");}
  cout<<"simultaneous: OK\n";
}
