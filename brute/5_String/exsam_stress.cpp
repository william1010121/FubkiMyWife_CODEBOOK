#include <bits/stdc++.h>
using namespace std;
#define ALL(v) (v).begin(), (v).end()
const int N = 200001, CNUM = 3;
#include "../../codebook/5_String/exSAM.cpp"
static exSAM ex;

static void require(bool ok, const char *message) {
  if (!ok) throw runtime_error(message);
}

static void check_order() {
  vector<bool> seen(ex.tot);
  int previous = 0;
  for (int i = 0; i < ex.tot - 1; ++i) {
    int v = ex.lenSorted[i];
    require(0 < v && v < ex.tot && !seen[v], "length permutation");
    seen[v] = true;
    require(previous <= ex.len[v], "length order");
    previous = ex.len[v];
    require(0 <= ex.link[v] && ex.link[v] < ex.tot &&
            ex.len[ex.link[v]] < ex.len[v], "suffix link");
  }
}

int main() {
  try {
    const int n = 200000;
    string s(n, 'a'), prefix(n / 2, 'a');
    ex.init(); ex.insert(s); ex.insert(prefix); ex.build();
    check_order(); ex.solve();
    require(ex.tot == n + 1, "unary states");
    for (int v = 1; v < ex.tot; ++v) {
      int l = ex.len[v];
      require(ex.cnt[v] == n-l+1 + max(0, n/2-l+1),
              "unary occurrences");
    }

    mt19937 rng(601002);
    for (char &c : s) c = 'a' + rng() % CNUM;
    prefix = s.substr(0, n / 2);
    ex.init(); ex.insert(s); ex.insert(s); ex.insert(prefix);
    ex.build(); check_order(); ex.solve();
    for (int len : {1, 2, 3, 7, 31, 61})
      for (int start : {0, n / 4, n / 2, n - 100}) {
        string sub = s.substr(start, len);
        int v = 0;
        for (char c : sub) {
          v = ex.next[v][c - 'a'];
          require(v != 0, "substring transition");
        }
        auto occurrences = [&](const string &text) {
          int count = 0;
          for (size_t p = text.find(sub); p != string::npos;
               p = text.find(sub, p + 1)) ++count;
          return count;
        };
        require(ex.cnt[v] == 2 * occurrences(s) + occurrences(prefix),
                "random occurrences");
      }
    // Many short documents: max depth is much smaller than state count.
    ex.init();
    for (int i = 0; i < n; i += 20) ex.insert(s.substr(i, 20));
    ex.build(); check_order(); ex.solve();
    require(ex.cnt[0] == n, "short-document root count");
    for (int v = 0; v < ex.tot; ++v)
      require(ex.len[v] <= 20, "short-document max length");
    for (int len : {1, 3, 7, 20})
      for (int start : {0, n / 4, n / 2, n - 100}) {
        string sub = s.substr(start, len); int v = 0, count = 0;
        for (char c : sub) v = ex.next[v][c - 'a'];
        require(v != 0, "short-document transition");
        for (int i = 0; i < n; i += 20) {
          string_view doc(s.data() + i, 20);
          for (size_t p = doc.find(sub); p != string_view::npos;
               p = doc.find(sub, p + 1)) ++count;
        }
        require(ex.cnt[v] == count, "short-document occurrences");
      }
    cout << "PASS exSAM stress unary/random/short-document tries\n";
  } catch (const exception &e) {
    cerr << "FAIL exSAM stress: " << e.what() << '\n'; return 1;
  }
}
