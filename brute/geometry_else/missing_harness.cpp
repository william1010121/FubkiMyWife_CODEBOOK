#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

namespace mo_update {
#include "../../codebook/9_Else/Mos_Algorithm_With_modification.cpp"
}
namespace mo_tree {
#include "../../codebook/9_Else/Mos_Algorithm_On_Tree.cpp"
}

#include "../../codebook/9_Else/All_LCS.cpp"
#include "../../codebook/9_Else/AdaptiveSimpson.cpp"
#include "../../codebook/9_Else/simulated_annealing.cpp"
#include "../../codebook/9_Else/tree_hash.cpp"
#include "../../codebook/9_Else/BinarySearchOnFraction.cpp"
#include "../../codebook/9_Else/BitsetLCS.cpp"
#include "../../codebook/9_Else/digit_dp.cpp"

static void req(bool ok, const string &msg) {
  if (!ok) { cerr << "FAIL " << msg << '\n'; exit(1); }
}
static bool subseq(const string &a, const string &b) {
  int j=0; for (char c:a) if (j<(int)b.size() && c==b[j]) ++j;
  return j==(int)b.size();
}
static vector<string> brute_lcs(const string &a, const string &b) {
  set<string> s; int n=a.size();
  for (int mask=0; mask<(1<<n); ++mask) {
    string x; for (int i=0;i<n;++i) if (mask>>i&1) x+=a[i];
    if (subseq(b,x)) s.insert(x);
  }
  int best=0; for (auto &x:s) if ((int)x.size()>best) best=x.size();
  vector<string> ret; for (auto &x:s) if ((int)x.size()==best) ret.push_back(x);
  return ret;
}

int main() {
  for (int n=0;n<=7;++n) for (int m=0;m<=7;++m) {
    string a,b;
    for (int i=0;i<n;++i) a += "abc"[i%3];
    for (int j=0;j<m;++j) b += "bac"[j%3];
    req(all_lcs(a,b)==brute_lcs(a,b), "all LCS");
  }

  auto simpson=make_simpson([](double x){return x*x;});
  req(fabs(simpson.eval(0.0,1.0,1e-12)-1.0/3)<1e-10, "adaptive Simpson square");
  auto sine=make_simpson([](double x){return sin(x);});
  req(fabs(sine.eval(0.0,acos(-1.0),1e-11)-2.0)<1e-9, "adaptive Simpson sine");

  vector<vector<int>> path(5), star(5), path_reordered(5);
  for (int i=1;i<5;++i) path[i-1].push_back(i), path[i].push_back(i-1);
  for (int i=1;i<5;++i) star[0].push_back(i), star[i].push_back(0);
  path_reordered[0]={1}; path_reordered[1]={2,0}; path_reordered[2]={3,1};
  path_reordered[3]={4,2}; path_reordered[4]={3};
  req(rooted_tree_hash(path)==rooted_tree_hash(path_reordered), "tree hash order");
  req(rooted_tree_hash(path)!=rooted_tree_hash(star), "tree hash shape");

  for (ll N=2;N<=20;++N) for (ll A=1;A<=N;++A) for (ll B=1;B<=N;++B) {
    auto pred=[=](Q x){ return x.q==0 || x.p*B>=A*x.q; };
    Q got=frac_bs(N,pred);
    ll bp=-1,bq=1;
    for (ll q=1;q<=N;++q) for (ll p=0;p<=N;++p)
      if (p*B>=A*q && (bp<0 || p*bq<bp*q)) bp=p,bq=q;
    req(bp>=0 && got.p*bq==bp*got.q, "fraction Stern-Brocot");
  }

  mt19937 rng(19260817);
  for (int tc=0;tc<500;++tc) {
    int n=rng()%70, m=rng()%70; vector<int>a(n),b(m);
    for (int &x:a) x=rng()%6; for (int &x:b) x=rng()%6;
    vector<vector<int>> dp(n+1,vector<int>(m+1));
    for(int i=1;i<=n;++i)for(int j=1;j<=m;++j)
      dp[i][j]=a[i-1]==b[j-1]?dp[i-1][j-1]+1:max(dp[i-1][j],dp[i][j-1]);
    req(bitset_lcs(a,b)==dp[n][m], "bitset LCS");
  }
  for (int x=0;x<=20000;++x) {
    int want=0;
    for (int y=0;y<=x;++y) if ((y&(y<<1))==0) ++want;
    req(calc(x)==want, "binary digit DP");
  }

  struct Update { int at, oldv, newv; };
  vector<int> arr={4,1,7,2,6,3,5};
  vector<Update> us={{1,1,8},{5,3,0},{0,4,9},{3,2,10}};
  vector<mo_update::Query> qs;
  qs.emplace_back(0,6,0,2,0); qs.emplace_back(1,4,2,2,1);
  qs.emplace_back(0,3,4,2,2); qs.emplace_back(2,6,1,2,3);
  vector<int> got(4); int sum=0;
  auto add_time=[&](int t,int L,int R){auto &u=us[t-1];if(L<=u.at&&u.at<=R)sum+=u.newv-u.oldv;arr[u.at]=u.newv;};
  auto sub_time=[&](int t,int L,int R){auto &u=us[t-1];if(L<=u.at&&u.at<=R)sum+=u.oldv-u.newv;arr[u.at]=u.oldv;};
  auto add=[&](int i){sum+=arr[i];}; auto sub=[&](int i){sum-=arr[i];};
  auto answer=[&](const mo_update::Query&q){got[q.id]=sum;};
  mo_update::solve(qs,add_time,sub_time,add,sub,answer);
  for(int qi=0;qi<4;++qi){auto a0=vector<int>{4,1,7,2,6,3,5};for(int t=0;t<qs[qi].T;++t)a0[us[t].at]=us[t].newv;int w=0;for(int i=qs[qi].L;i<=qs[qi].R;++i)w+=a0[i];req(got[qi]==w,"Mo with modification");}

  int N=7; vector<vector<int>> g(N); auto edge=[&](int a,int b){g[a].push_back(b);g[b].push_back(a);};
  edge(0,1);edge(0,2);edge(1,3);edge(1,4);edge(2,5);edge(2,6);
  vector<int> in(N),out(N),ord; vector<int> dep(N),par(N,-1);
  auto dfs=[&](auto&&self,int u,int p)->void{par[u]=p;in[u]=ord.size();ord.push_back(u);for(int v:g[u])if(v!=p)dep[v]=dep[u]+1,self(self,v,u);out[u]=ord.size();ord.push_back(u);}; dfs(dfs,0,-1);
  auto lca=[&](int a,int b){while(dep[a]>dep[b])a=par[a];while(dep[b]>dep[a])b=par[b];while(a!=b)a=par[a],b=par[b];return a;};
  vector<int> weight={3,5,7,11,13,17,19}, active(N), tree_got(N*N); int tree_sum=0;
  vector<mo_tree::Query> tq; for(int u=0;u<N;++u)for(int v=0;v<N;++v)tq.emplace_back(u,v,3,in,out,lca,u*N+v);
  auto flip=[&](int u){active[u]^=1;tree_sum+=active[u]?weight[u]:-weight[u];};
  auto tanswer=[&](const mo_tree::Query&q){tree_got[q.id]=tree_sum;};
  mo_tree::solve(tq,ord,flip,tanswer);
  for(int u=0;u<N;++u)for(int v=0;v<N;++v){int x=u,y=v,w=0;while(dep[x]>dep[y])w+=weight[x],x=par[x];while(dep[y]>dep[x])w+=weight[y],y=par[y];while(x!=y)w+=weight[x]+weight[y],x=par[x],y=par[y];w+=weight[x];req(tree_got[u*N+v]==w,"Mo on tree");}

  mt19937_64 arng(123); auto best=simulated_annealing(-10.0,[](double x){return (x-3)*(x-3);},[](double x,mt19937_64&r){return x+(int)(r()%7)-3;},arng,50000,10.0,.9998);
  req((best-3)*(best-3)<=1.0,"simulated annealing");
  cout<<"else missing PASS: LCS, integration, hash, fraction, bitset, digit DP, Mo, annealing\n";
}
