#include <bits/stdc++.h>
using namespace std; using ll=long long; using ull=unsigned long long;
#define X first
#define Y second
#define SZ(x) ((int)(x).size())
#define ALL(x) (x).begin(),(x).end()
#define pb push_back
const int INF=1e9;
#include "../../codebook/9_Else/cyc_tsearch.cpp"
#include "../../codebook/9_Else/HilbertCurve.cpp"
#include "../../codebook/9_Else/min_plus_convolution.cpp"
#include "../../codebook/9_Else/NQueens.cpp"
#include "../../codebook/9_Else/DynamicConvexTrick.cpp"

template<class F> bool eqall(F f,int n){for(int i=0;i<n;i++)for(int j=0;j<n;j++)if(f(i,j)!=f(j,i))return false;return true;}
static void req(bool x,string s){if(!x){cerr<<"FAIL "<<s<<'\n';exit(1);}}

int main(){
  mt19937 rng(998244353);
  for(int k=1;k<=5;k++){int n=1<<k;vector<int> seen(n*n);for(int x=0;x<n;x++)for(int y=0;y<n;y++){ll z=hilbert(n,x,y);req(0<=z&&z<(ll)n*n,"Hilbert range");seen[z]++;}req(count(seen.begin(),seen.end(),1)==n*n,"Hilbert bijection");}
  for(int n=1;n<=64;n++){
    int shift=rng()%n; auto pred=[&](int a,int b){int va=(a-shift+n)%n,vb=(b-shift+n)%n;return va<vb;};
    int got=cyc_tsearch(n,pred);req(got==shift,"cyclic ternary search");
  }
  // Contract: strict cyclic U-shape (all values pairwise distinct).
  {
    vector<int> f={3,0,1,2};
    auto pred=[&](int a,int b){return f[a]<f[b];};
    req(cyc_tsearch((int)f.size(),pred)==1,
        "cyclic search strict-distinct contract");
  }
  for(int tc=0;tc<100;tc++){
    int n=1+rng()%6,m=1+rng()%6;vector<int>a(n),b(m);int cur=-5,d=0;for(int&i:a){cur+=d;i=cur;d+=rng()%3;}cur=-5;for(int&i:b){cur+=rng()%8-2;i=cur;}
    vector<int> got=min_plus_convolution(a,b),want(n+m-1,INF);for(int i=0;i<n;i++)for(int j=0;j<m;j++)want[i+j]=min(want[i+j],a[i]+b[j]);req(got==want,"min-plus convolution");
  }
  {
    vector<int> a={1000000000}, b={1000000000};
    req(min_plus_convolution(a,b)==vector<int>{2000000000},
        "min-plus values beyond INF sentinel");
    vector<int> empty;
    req(min_plus_convolution(empty,b).empty() &&
        min_plus_convolution(a,empty).empty() &&
        min_plus_convolution(empty,empty).empty(), "empty min-plus input");
    vector<long long> x={4000000000000000000LL}, y={4000000000000000000LL};
    req(min_plus_convolution(x,y)==vector<long long>{8000000000000000000LL},
        "long-long min-plus convolution");
  }
  for(int n=1;n<=30;n++){vector<int> q;solve(q,n);if(n==2||n==3){req(q.empty(),"NQueens no solution");continue;}req((int)q.size()==n,"NQueens size");vector<int> c(n+1);for(int x:q){req(1<=x&&x<=n&&++c[x]==1,"NQueens columns");}for(int i=0;i<n;i++)for(int j=0;j<i;j++)req(abs(i-j)!=abs(q[i]-q[j]),"NQueens diagonals");}
  for(int tc=0;tc<100;tc++){DynamicHull h;vector<pair<ll,ll>> ls;for(int i=0;i<30;i++){ll a=(int)(rng()%21)-10,b=(int)(rng()%41)-20;h.addline(a,b);ls.push_back({a,b});for(ll x=-20;x<=20;x++){ll w=-4e18;for(auto [aa,bb]:ls)w=max(w,aa*x+bb);req(h.query(x)==w,"dynamic convex hull");}}}
  cout<<"else PASS: exhaustive-small brute checks completed\n";
}
