#include <bits/stdc++.h>
using namespace std;using ll=long long;
#define SZ(a) int((a).size())
#define ALL(v) (v).begin(),(v).end()
#define pb push_back
#include "../../codebook/9_Else/CDQ_Dominance.cpp"
int main(){
 mt19937 rng(1204);
 for(int k=0;k<=8;++k)for(int tc=0;tc<200;++tc){
  int n=rng()%65;vector<vector<int>> a(n,vector<int>(k));
  for(auto &p:a)for(auto &x:p)x=int(rng()%9)-4;
  if(tc%3==0)for(auto &p:a)for(auto &x:p)x=x<0?INT_MIN:x>0?INT_MAX:0;
  vector<ll> want(n);
  for(int i=0;i<n;++i)for(int j=0;j<n;++j)if(i!=j){
   bool ok=1;for(int d=0;d<k;++d)ok&=a[j][d]<=a[i][d];want[i]+=ok;
  }
  assert(dominance(a)==want);
 }
 vector<vector<int>> a(200000,vector<int>(4));vector<ll> want(a.size());
 for(int i=0;i<SZ(a);++i){a[i]={i,i/7,i/11,i/13};want[i]=i;}
 shuffle(ALL(a),rng);for(int i=0;i<SZ(a);++i)want[i]=a[i][0];
 assert(dominance(a)==want);
 cout<<"PASS 1800 quadratic cases and shuffled 200k 4D tied chain\n";
}
