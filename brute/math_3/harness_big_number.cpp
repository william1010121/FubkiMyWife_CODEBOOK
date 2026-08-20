#include "common.hpp"
namespace tested {
#include "../../codebook/6_Math/Big_number.cpp"
}

using i128 = __int128_t;
static i128 parse(const string &s) { i128 x=0; int i=0,sg=1; if(s[0]=='-')sg=-1,i=1; for(;i<(int)s.size();++i)x=x*10+s[i]-'0'; return sg*x; }
static string print(i128 x) { if(!x)return "0"; bool neg=x<0;if(neg)x=-x;string s;while(x){s+=char('0'+x%10);x/=10;}if(neg)s+='-';reverse(s.begin(),s.end());return s; }
static void check_value(const tested::bigN &x, i128 y, const string &tag) {
  stringstream ss; ss << x;
  require_ok(ss.str() == print(y), tag + ": got=" + ss.str() + " want=" + print(y));
}
int main() {
  mt19937_64 rng(0xB16B00B5);
  vector<string> v = {"0", "1", "-1", "999999999", "1000000000", "-1000000001",
                      "123456789012345678901234567890", "-987654321098765432109876543210"};
  for (int it = 0; it < 320; ++it) {
    auto make = [&]() {
      int n = 1 + rng() % 18; string s;
      if (rng() & 1) s += '-';
      s += char('1' + rng() % 9);
      while (--n) s += char('0' + rng() % 10);
      return s;
    };
    v.pb(make()); v.pb(make());
  }
  for (const string &sa : v) for (const string &sb : v) {
    i128 a = parse(sa), b = parse(sb);
    tested::bigN A(sa), B(sb);
    check_value(A + B, a + b, "add");
    check_value(A - B, a - b, "sub");
    if (a >= -(i128)1000000000 && a <= (i128)1000000000 && b >= -(i128)1000000000 && b <= (i128)1000000000)
      check_value(A * B, a * b, "mul");
    if (b != 0) {
      check_value(A / B, a / b, "div");
      check_value(A % B, a % b, "mod");
    }
    require_ok((A < B) == (a < b), "lt");
    require_ok((A == B) == (a == b), "eq");
  }
  cout << "big_number: OK\n";
}
