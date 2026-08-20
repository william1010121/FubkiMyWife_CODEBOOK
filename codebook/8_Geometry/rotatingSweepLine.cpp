vector<int> rotatingSweepLineOrder(const vector<pair<int,int>> &ps){
  int n=int(ps.size());
  if (n == 0) return {};
  vector<int> id(n),pos(n);
  vector<pair<int,int>> line(n*(n-1)/2);
  int m=-1;
  for(int i=0;i<n;++i)for(int j=i+1;j<n;++j)line[++m]=make_pair(i,j); ++m;
  sort(line.begin(),line.end(),[&](const pair<int,int> &a,const pair<int,int> &b){
    long long ax=ps[a.first].first-ps[a.second].first;
    long long ay=ps[a.first].second-ps[a.second].second;
    long long bx=ps[b.first].first-ps[b.second].first;
    long long by=ps[b.first].second-ps[b.second].second;
    if (ax == 0 || bx == 0) return ax == 0 && bx != 0;
    return ay * bx < by * ax;
  });
  for(int i=0;i<n;++i)id[i]=i;
  sort(id.begin(),id.end(),[&](const int &a,const int &b){ return ps[a]<ps[b]; });
  for(int i=0;i<n;++i)pos[id[i]]=i;

  for(int i=0;i<m;++i){
    auto l=line[i];
    tie(pos[l.first],pos[l.second],id[pos[l.first]],id[pos[l.second]])=make_tuple(pos[l.second],pos[l.first],l.second,l.first);
  }
  return id;
}

void rotatingSweepLine(vector<pair<int,int>> &ps){
  (void)rotatingSweepLineOrder(ps);
}
