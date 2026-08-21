#define int long long
const int mod=998244353, g=3;

int pwr(int a, int b, int ret=1) {
  do ret = b&1?ret*a%mod:ret, a=a*a%mod;
  while(b>>=1); return ret;
}

void ntt(vector<int> &vec, bool inv=false) {
  int n = vec.size();
  for(int i=0, j=0; i<n; ++i) { // j = 反過來模擬加法
    if(i<j) swap(vec[i],vec[j]);
    for(int bit=n>>1; (j^=bit)<bit; bit>>=1);
  }
  for(int len=2; len<=n; len<<=1) { // 循環長度 = mod-1
    int wlen = pwr(g,(mod-1)/len);
    if(inv) wlen = pwr(wlen,mod-2);
    for(int i=0; i<n; i+=len) {
      int w=1;
      for(int j=0; j<len/2; ++j) {
      // i = 定範圍, j = 去跑 merge
        int a = vec[i+j], b = w*vec[i+len/2+j]%mod;
        vec[i+j]       = a+b<mod ? a+b : a+b-mod;
        vec[i+len/2+j] = a-b>=0  ? a-b : a-b+mod;
        w = w*wlen%mod;
      }
    }
  }
  if(inv) {
    int invn = pwr(n,mod-2);
    for(auto &i : vec) i = i*invn%mod;
  }
}
#define intt(vec) ntt(vec,true)

vector<int> mult(vector<int> a, vector<int> b) {
  int sz = a.size()+b.size()-1, n=1; while(n<sz) n<<=1;
  a.resize(n), b.resize(n), ntt(a), ntt(b);
  for(int i=0; i<n; ++i) a[i] = a[i]*b[i] % mod;
  intt(a), a.resize(sz); return a;
}