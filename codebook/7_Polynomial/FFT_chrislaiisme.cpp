using cd = complex<double>;
const double pi = acos(-1);

void fft(vector<cd> &vec, bool inv=false) {
  int n = vec.size();
  for(int i=0, j=0; i<n; ++i) { // j = 反過來模擬加法
    if(i<j) swap(vec[i],vec[j]);
    for(int bit=n>>1; (j^=bit)<bit; bit>>=1);
  }
  for(int len=2; len<=n; len<<=1) { // 循環長度 = 2pi
    double ang = 2*pi/len*(inv?-1:1); cd wlen(cos(ang), sin(ang));
    for(int i=0; i<n; i+=len) {
      cd w=1;
      for(int j=0; j<len/2; ++j) { // i = 定範圍, j = 去跑 merge
        cd a = vec[i+j], b = w*vec[i+len/2+j];
        vec[i+j]       = a+b;
        vec[i+len/2+j] = a-b;
        w *= wlen;
      }
    }
  }
  if(inv) for(auto &i : vec) i/=n;
}
#define ifft(vec) fft(vec,true)

vector<int> mult(const vector<int>& a, const vector<int>& b) {
  int sz = a.size()+b.size()-1, n=1; while(n<sz) n<<=1;
  vector<cd> ca(a.begin(),a.end()), cb(b.begin(),b.end());
  ca.resize(n), cb.resize(n), fft(ca), fft(cb);
  for(int i=0; i<n; ++i) ca[i] *= cb[i];
  ifft(ca);
  vector<int> res(sz); for(int i=0; i<sz; ++i) res[i] = round(ca[i].real());
  return res;
}
