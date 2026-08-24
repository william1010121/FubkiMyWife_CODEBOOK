vector<int> rotatingSweepLineOrder(const
  vector<pair<int,int>> &ps){
  int n=int(ps.size()); if (!n) return {};
  vector<int> id(n),pos(n);
  vector<pair<int,int>> line(n*(n-1)/2);
  int m=0;
  for(int i=0;i<n;++i)for(int j=i+1;j<n;++j)line[m++]={
    i,j};
  sort(line.begin(),line.end(),[&](const pair<int,
    int> &a,const pair<int,int> &b){
    long long ax=1LL*ps[a.first].first-ps[a.second].first;
    long long ay=1LL*ps[a.first].second-
      ps[a.second].second;
    long long bx=1LL*ps[b.first].first-ps[b.second].first;
    long long by=1LL*ps[b.first].second-
      ps[b.second].second;
    // Pair directions are unoriented: canonicalize them before comparing
    // slopes.  Without this, a negative ax reverses the cross-multiplication
    // order and the comparator is not a strict weak ordering.
    if (ax < 0 || (ax == 0 && ay < 0)) ax = -ax, ay = -ay;
    if (bx < 0 || (bx == 0 && by < 0)) bx = -bx, by = -by;
    if (ax == 0 || bx == 0) return ax == 0 && bx != 0;
    return (__int128)ay * bx < (__int128)by * ax;
  });
  iota(id.begin(), id.end(), 0);
  sort(id.begin(),id.end(),[&](const int &a,
    const int &b){ return ps[a]<ps[b]; });
  for(int i=0;i<n;++i)pos[id[i]]=i;

  for(int i=0;i<m;++i){
    auto l=line[i];
    tie(pos[l.first],pos[l.second],id[pos[l.first]],
      id[pos[l.second]])=make_tuple(pos[l.second],
      pos[l.first],l.second,l.first);
  }
  return id;
}

void rotatingSweepLine(vector<pair<int,int>> &ps){
  (void)rotatingSweepLineOrder(ps);
}
