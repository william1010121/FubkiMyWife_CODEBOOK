vector<int> a={3,4,5,1,2};
int at=cyc_tsearch(a.size(),
  [&](int i,int j){return a[i]<a[j];}); // at=3
