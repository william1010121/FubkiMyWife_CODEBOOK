struct SAT { // 0-base true=i,false=i+n; O(V+E)
  int n; vector<bool> istrue; // n>=0; 2*n fits int
  SCC scc; // Reverse-topological SCC; enough DFS stack.
  SAT(int _n): n(_n), istrue(n + n), scc(n + n) {}
  // Literals [0,2*n).
  int rv(int a) { return a >= n ? a - n : a + n; }
  void add_clause(int a, int b) {
    scc.add_edge(rv(a), b), scc.add_edge(rv(b), a); }
  bool solve() { // Clauses first; istrue only on success
    scc.solve(); for (int i = 0; i < n; ++i)
      if (scc.bln[i] == scc.bln[i + n]) return false;
      else istrue[i] = scc.bln[i] < scc.bln[i + n],
        istrue[i + n] = !istrue[i];
    return true; }
};
