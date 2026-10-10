#include <bits/stdc++.h>
using namespace std;
#define SZ(a) ((int)(a).size())
#define ALL(v) (v).begin(), (v).end()
#define pb push_back

const int len = 125000, sigma = 3, kN = 100001;
#include "../../codebook/5_String/Aho-Corasick_Automatan.cpp"
#include "../../codebook/5_String/SAIS-C++20.cpp"
#include "../../codebook/5_String/MainLorentz.cpp"

static void require(bool ok, const string &message) {
  if (!ok) throw runtime_error(message);
}

static void ac_stress() {
  // Patterns A^m B A^k C, 1<=k<=m, plus A^m.
  // O(m) trie nodes but Theta(m^2) explicit failure-link hops.
  const int m = 40000;
  ac.init();
  string prefix(m, 'A'); prefix += 'B';
  int v = ac.input(prefix);
  vector<int> leaves;
  for (int k = 1; k <= m; ++k) {
    int child = ac.newnode(); ac.nx[v][0] = child; v = child;
    int leaf = ac.newnode(); ac.nx[v][2] = leaf;
    leaves.push_back(leaf);
  }
  ac.make_fl();
  for (int k = 1; k <= m; ++k) {
    int leaf = leaves[k - 1];
    require(ac.fl[leaf - 1] == k + 1, "AC A-chain failure");
    require(ac.fl[leaf] == 1, "AC C-leaf failure");
  }
  string text = prefix + string(m, 'A') + 'C';
  int state = 1;
  for (char c : text) ++ac.cnt[state = ac.rnx[state][c - 'A']];
  ac.solve();
  for (int k = 0; k < m; ++k)
    require(ac.cnt[leaves[k]] == (k == m - 1), "AC counts");
}

static void sa_stress() {
  const int n = 200000;
  vector<int> s(n, 1);
  Suffix uniform(s.data(), n);
  for (int i = 0; i < n; ++i)
    require(uniform.sa[i] == n - i - 1 && uniform.hi[i] == i,
            "SA uniform order/LCP");
  // Every non-sentinel LMS substring has a distinct name.
  for (int i = 1; i < n; i += 2) s[i] = n - i;
  Suffix distinct(s.data(), n);
  vector<int> rank(n + 1, -1);
  for (int i = 0; i < n; ++i) {
    int p = distinct.sa[i];
    require(0 <= p && p < n && rank[p] == -1, "SA permutation");
    rank[p] = i;
  }
  for (int i = 1; i < n; ++i) {
    int a = distinct.sa[i - 1], b = distinct.sa[i];
    require(s[a] < s[b] || (s[a] == s[b] && rank[a+1] < rank[b+1]),
            "SA suffix order recurrence");
    int h = 0;
    while (a+h < n && b+h < n && s[a+h] == s[b+h]) ++h;
    require(distinct.hi[i] == h, "SA distinct-name LCP");
  }
}

static void lorentz_stress() {
  const int n = 200000;
  main_lorentz(string(n, 'a'), 13);
  for (int l = 1; l <= n / 2; ++l) {
    sort(rep[l].begin(), rep[l].end());
    int next = 13;
    for (auto [a, b] : rep[l]) {
      require(a == next && b >= a, "Lorentz gap/overlap");
      next = b + 1;
    }
    require(next == 13 + n - 2*l + 1, "Lorentz uniform coverage");
  }
  main_lorentz("ab");
  for (const auto &v : rep) require(v.empty(), "Lorentz reset");
}

int main() {
  try {
    auto run = [](const char *name, auto f) {
      auto start = chrono::steady_clock::now(); f();
      double elapsed = chrono::duration<double>(
        chrono::steady_clock::now() - start).count();
      cout << "PASS " << name << " seconds=" << elapsed << '\n';
    };
    run("AC 120002 nodes", ac_stress);
    run("SAIS 200000 symbols x2", sa_stress);
    run("MainLorentz 200000 bytes", lorentz_stress);
  } catch (const exception &e) {
    cerr << "FAIL g06 stress: " << e.what() << '\n'; return 1;
  }
}
