struct palindromic_tree { // lowercase; roots 0:0,1:-1; O(n*26)
  struct node {
    int next[26] = {}, fail = 0, len, cnt = 0, num = 0;
    // cnt: occurrences; num: number of palindromic suffixes
    node(int l = 0) : len(l) {}
  };
  vector<node> St;
  vector<char> s;
  int last, n;
  palindromic_tree() { St.reserve(2); clear(); }
  void clear() {
    St.clear(), s.clear(), last = 1, n = 0;
    St.pb(0), St.pb(-1), St[0].fail = 1, s.pb(-1);
  }
  int get_fail(int x) {
    while (s[n - St[x].len - 1] != s[n]) x = St[x].fail;
    return x;
  }
  void add(int c) {
    s.pb(c -= 'a'), ++n; int cur = get_fail(last);
    if (!St[cur].next[c]) { int now = SZ(St);
      St.pb(St[cur].len + 2);
      St[now].fail = St[get_fail(St[cur].fail)].next[c];
      St[cur].next[c] = now;
      St[now].num = St[St[now].fail].num + 1;
    } last = St[cur].next[c], ++St[last].cnt;
  }
  void count() { // call once after all add()
    for (int i = SZ(St) - 1; i >= 0; --i)
      St[St[i].fail].cnt += St[i].cnt;
  }
  int size() { return SZ(St) - 2; } // distinct palindromes
};
