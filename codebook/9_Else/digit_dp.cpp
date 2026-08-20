int dfs(int pos, int pre, int lead, int limit, int cnt){
  if(pos == -1)return max(cnt,1ll);
  if(!lead && !limit && dp[pos][pre][cnt] != -1)
    return dp[pos][pre][cnt];
  int lb = 0, rb = (limit ? s[pos]-'0' : 1), ret = 1;
  for(int i=lb;i<=rb;i++){
    ret *= dfs(pos - 1, i, lead & (i == 0),
        limit & (i == rb), cnt + (i==1));
    ret %= mod;
  }
  if(!lead && !limit)dp[pos][pre][cnt] = ret;
  return ret;
}
int calc(int x){
  s.clear();
  if(x==0)return 0;
  while(x){s += (x%2 + '0');x/=2;}
  memset(dp,-1,sizeof dp);
  return dfs(sz(s)-1,0,1,1,0);
}