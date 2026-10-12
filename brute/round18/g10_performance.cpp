#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define SZ(a) int((a).size())
#define pb push_back
namespace pal {
#include "../../codebook/5_String/PalTree.cpp"
}
namespace fac {
constexpr int MAXP = 1048577;
#include "../../codebook/6_Math/fac_no_p.cpp"
}
ll pal_count(const string &s) { // Independent Manacher oracle.
  int n = s.size(); vector<int> odd(n), even(n); ll total = 0;
  for (int i = 0, l = 0, r = -1; i < n; ++i) {
    int k = i > r ? 1 : min(odd[l+r-i], r-i+1);
    while (i-k >= 0 && i+k < n && s[i-k] == s[i+k]) ++k;
    total += odd[i] = k--;
    if (i+k > r) l = i-k, r = i+k;
  }
  for (int i = 0, l = 0, r = -1; i < n; ++i) {
    int k = i > r ? 0 : min(even[l+r-i+1], r-i+1);
    while (i-k-1 >= 0 && i+k < n && s[i-k-1] == s[i+k]) ++k;
    total += even[i] = k--;
    if (i+k > r) l = i-k-1, r = i+k;
  }
  return total;
}
int main() {
  clock_t start = clock(); mt19937 rng(18010);
  for (int alpha : {1, 3, 26}) {
    string s(200000, 'a'); for (char &c : s) c += rng() % alpha;
    ll expected = pal_count(s); pal::palindromic_tree t;
    for (int run = 0; run < 2; ++run) {
      for (char c : s) t.add(c); t.count(); ll actual = 0;
      for (int i = 2; i < SZ(t.St); ++i) actual += t.St[i].cnt;
      assert(actual == expected && t.St[0].cnt == SZ(s));
      assert(t.St[1].cnt == SZ(s)); // Public root counts retained.
      if (alpha == 1) assert(t.size() == SZ(s));
      t.clear(); assert(t.size() == 0 && t.last == 1);
    }
  }
  for (auto [p, pk] : vector<pair<ll,ll>>{
         {2, 1 << 20}, {3, 531441}, {999983, 999983}}) {
    ll previous = fac::fac_no_p(LLONG_MAX-1, p, pk);
    ll current = fac::fac_no_p(LLONG_MAX, p, pk), unit = LLONG_MAX;
    while (unit % p == 0) unit /= p;
    assert(current == (__int128)previous * (unit % pk) % pk);
    assert(fac::fac_no_p(0, p, pk) == 1);
    ll pref = 1; assert(fac::prod[0] == pref);
    for (int i = 1; i <= pk; ++i) {
      if (i % p) pref = (__int128)pref * i % pk;
      assert(fac::prod[i] == pref);
    }
    ll exact = 1;
    for (ll i = 1; i <= 1000; ++i) {
      ll u = i; while (u % p == 0) u /= p;
      exact = (__int128)exact * u % pk;
    }
    assert(fac::fac_no_p(1000, p, pk) == exact);
  }
  cout << "PASS round18 g10: Manacher totals, full unit prefixes; "
       << 1000.0 * (clock()-start) / CLOCKS_PER_SEC << " ms CPU\n";
}
