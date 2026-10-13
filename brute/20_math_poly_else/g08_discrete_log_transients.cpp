#include <bits/stdc++.h>
using namespace std;
int fpow(int a, int e, int m) {
  long long r = 1 % m;
  for (; e; e >>= 1, a = 1LL * a * a % m)
    if (e & 1) r = r * a % m;
  return r;
}
#include "../../codebook/6_Math/DiscreteLog.cpp"

int main() {
  int checked = 0;
  for (int k : {1, 5, 20, 21, 28, 30})
    for (int q : {1, 3, 7, 1019}) {
      long long modulus = (1LL << k) * q;
      if (modulus > INT_MAX) continue;
      int m = modulus;
      for (int x : {0, 2, 4, 6, 10}) {
        if (x >= m) continue;
        // Independent finite orbit: transient and cycle, no BSGS.
        unordered_map<int, int> first;
        vector<int> orbit;
        for (int y = 1; !first.count(y); y = 1LL * y * x % m) {
          first[y] = orbit.size(); orbit.push_back(y);
          if (orbit.size() > 5000) return 1;
        }
        set<int> targets = {0, 1, m - 1};
        for (int i : {0, 1, 28, 29, 30, 31, 32, 99, 100, 101})
          if (i < (int)orbit.size()) targets.insert(orbit[i]);
        targets.insert(orbit.back());
        for (int y : targets) {
          auto it = first.find(y);
          int want = it == first.end() ? -1 : it->second;
          int got = DiscreteLog(x, y, m);
          if (got != want) {
            cerr << "transient mismatch: " << x << ' ' << y << ' '
                 << m << " want=" << want << " got=" << got << '\n';
            return 1;
          }
          ++checked;
        }
      }
    }
  cout << "g08_discrete_log_transients: PASS " << checked
       << " large prime-power transient/cycle cases\n";
}
