struct exSAM { // init, insert all, build/solve once
  // Root=0; N,CNUM>=1; states incl. clones fit 2*N.
  // 2*N, total length fit int; 0<=c-'a'<CNUM.
  int len[N * 2], link[N * 2], next[N * 2][CNUM], tot;
  int lenSorted[N * 2], cnt[N * 2]; // order, counts
  int newnode() { fill_n(next[tot], CNUM, 0);
    len[tot] = cnt[tot] = link[tot] = 0; return tot++;
  }
  void init() { tot = 0, newnode(), link[0] = -1; }
  int insertSAM(int last, int c) {
    int cur = next[last][c], p = link[last];
    len[cur] = len[last] + 1;
    while (p != -1 && !next[p][c])
      next[p][c] = cur, p = link[p];
    if (p == -1) return link[cur] = 0, cur;
    int q = next[p][c];
    if (len[p] + 1 == len[q]) return link[cur] = q, cur;
    int clone = newnode();
    for (int i = 0; i < CNUM; ++i) // built edges only
      next[clone][i] = len[next[q][i]] ? next[q][i] : 0;
    len[clone] = len[p] + 1, link[clone] = link[q];
    while (p != -1 && next[p][c] == q)
      next[p][c] = clone, p = link[p];
    link[cur] = link[q] = clone; return cur;
  }
  void insert(const string &s) {
    int cur = 0; for (char ch : s) {
      int &v = next[cur][ch - 'a']; if (!v) v = newnode();
      ++cnt[cur = v];
    }
  }
  void build() { // O(total input length * CNUM)
    int t = 1; lenSorted[0] = 0; // BFS, then length order
    for (int h = 0; h < t; ++h) { int u = lenSorted[h];
      for (int i = 0; i < CNUM; ++i)
        if (next[u][i]) lenSorted[t++] = insertSAM(u, i);
    } vector<int> lc(len[lenSorted[t-1]]+1); // max depth
    for (int i = 1; i < tot; ++i) ++lc[len[i]];
    partial_sum(ALL(lc), lc.begin());
    for (int i = 1; i < tot; ++i)
      lenSorted[--lc[len[i]]] = i;
  }
  void solve() {
    for (int i = tot - 2; i >= 0; --i)
      cnt[link[lenSorted[i]]] += cnt[lenSorted[i]];
  }
};
