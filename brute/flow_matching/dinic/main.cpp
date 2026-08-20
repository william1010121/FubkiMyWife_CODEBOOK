#include <bits/stdc++.h>
using namespace std;

#define MAXN 16
#define pb push_back
const int INF = 1'000'000'000;
#include "../../../codebook/4_Flow_Matching/Dinic.cpp"

static int edmonds_karp(const vector<vector<int>> &initial, int s, int t) {
  int n = (int)initial.size(), ans = 0;
  vector<vector<int>> cap = initial;
  for (;;) {
    vector<int> pre(n, -1), aug(n);
    queue<int> q;
    q.push(s), pre[s] = s, aug[s] = INF;
    while (!q.empty() && pre[t] == -1) {
      int u = q.front(); q.pop();
      for (int v = 0; v < n; ++v) if (pre[v] == -1 && cap[u][v]) {
        pre[v] = u;
        aug[v] = min(aug[u], cap[u][v]);
        q.push(v);
      }
    }
    if (pre[t] == -1) return ans;
    ans += aug[t];
    for (int v = t; v != s; v = pre[v]) {
      int u = pre[v];
      cap[u][v] -= aug[t];
      cap[v][u] += aug[t];
    }
  }
}

int main() {
  mt19937 rng(0xD1A1C);
  for (int tc = 0; tc < 3000; ++tc) {
    int n = 2 + (rng() % 9), s = 0, t = n - 1;
    vector<vector<int>> cap(n, vector<int>(n));
    MaxFlow mf;
    mf.init(n);
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
      if (u != v && rng() % 3 == 0) {
        int c = 1 + (rng() % 7);
        cap[u][v] += c;
        mf.add_edge(u, v, c);
      }
    int want = edmonds_karp(cap, s, t);
    int got = mf.maxflow(s, t);
    if (got != want) {
      cerr << "Dinic mismatch on case " << tc << ": got=" << got
           << " want=" << want << '\n';
      return 1;
    }
    mf.reset();
    if (mf.maxflow(s, t) != want) {
      cerr << "Dinic reset mismatch on case " << tc << '\n';
      return 1;
    }
  }
  cout << "PASS 3000 random graphs + reset checks\n";
}
