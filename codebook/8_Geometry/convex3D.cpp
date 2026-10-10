double absvol(P a, P b, P c, P d) {
  return abs(((b-a)^(c-a))*(d-a))/6; }
struct convex3D { // n <= 1010, m=0; permutes p; O(n^2).
  // f[0..m): outward triangles; degenerate input gives m=0.
  static const int maxn=1010;
  struct T{
    int a,b,c; bool res; T(){}
    T(int a,int b,int c,bool res=1):a(a),b(b),c(c),res(res){}
  };
  int n,m; P p[maxn];
  T f[maxn*8]; int id[maxn][maxn];
  bool on(T &t,P &q) { return ((p[t.c]-
    p[t.b])^(p[t.a]-p[t.b]))*(q-p[t.a])>eps; }
  void add(T t){
    id[t.a][t.b]=id[t.b][t.c]=id[t.c][t.a]=m; f[m++]=t; }
  void meow(int q,int a,int b){
    int g=id[a][b];
    if(f[g].res){
      if(on(f[g],p[q]))dfs(q,g);
      else add(T(b,a,q));
    }
  }
  void dfs(int p,int i){ f[i].res=0;
    meow(p,f[i].b,f[i].a);
    meow(p,f[i].c,f[i].b); meow(p,f[i].a,f[i].c); }
  void operator()(){
    if(n<4)return;
    auto choose = [&](int k, auto test) {
      for(int i=k;i<n;++i)if(test(p[i])){
        swap(p[k],p[i]); return true;
      } return false;
    };
    if(!choose(1,[&](P q){return abs(p[0]-q)>eps;}) ||
       !choose(2,[&](P q){return abs((p[0]-q)^(p[1]-q))>eps;}) ||
       !choose(3,[&](P q){
         return abs(((p[1]-p[0])^(p[2]-p[0]))*(q-p[0]))>eps;
       }))return;
    for(int i=0;i<4;++i){
      T t((i+1)%4,(i+2)%4,(i+3)%4,1);
      if(on(t,p[i]))swap(t.b,t.c);
      add(t);
    } for(int i=4;i<n;++i){
      for(int j=0;j<m;++j)
        if(on(f[j],p[i])){ dfs(i,j); break; }
      int mm=m; m=0;
      for(int j=0;j<mm;++j)if(f[j].res)add(f[j]);
    }
  }
  bool same(int i,int j){
    for(int k : {f[j].a,f[j].b,f[j].c})
      if(absvol(p[f[i].a],p[f[i].b],p[f[i].c],p[k])>eps)
        return false;
    return true;
  }
  int faces(){
    int r=0;
    for(int i=0;i<m;++i){
      int iden=1;
      for(int j=0;j<i;++j)if(same(i,j)){iden=0; break;}
      r+=iden;
    } return r;
  }
} tb;
