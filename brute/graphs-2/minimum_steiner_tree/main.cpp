#include <bits/stdc++.h>
using namespace std;
#define N 8
#define T 4
#define SZ(v) ((int)(v).size())
#define copy_n std::copy_n
const int INF = 1000000000;
#include "../../../codebook/2_Graph/MinimumSteinerTree.cpp"

struct Edge { int u, v, w; };
static int oracle(int n, const vector<Edge> &es, const vector<int> &term,
                  const vector<int> &vc) {
  int ans = INF, m = (int)es.size();
  for (int mask = 0; mask < (1 << m); ++mask) {
    int cost = 0;
    vector<char> used(n);
    for (int i = 0; i < (int)term.size(); ++i) used[term[i]] = 1;
    for (int i = 0; i < m; ++i) if (mask >> i & 1) {
      cost += es[i].w;
      used[es[i].u] = used[es[i].v] = 1;
    }
    for (int i = 0; i < n; ++i) if (used[i]) cost += vc[i];
    if (cost >= ans) continue;
    vector<vector<int>> g(n);
    for (int i = 0; i < m; ++i) if (mask >> i & 1) g[es[i].u].push_back(es[i].v);
    for (int root = 0; root < n; ++root) {
      bool ok = true;
      for (int s : term) {
        vector<char> vis(n);
        queue<int> q;
        q.push(s), vis[s] = 1;
        while (!q.empty()) {
          int u = q.front(); q.pop();
          for (int v : g[u]) if (!vis[v]) vis[v] = 1, q.push(v);
        }
        if (!vis[root]) { ok = false; break; }
      }
      if (ok) { ans = cost; break; }
    }
  }
  return ans;
}

int main() {
  mt19937 rng(0x53544549);
  int cases = 0;
  for (int n = 2; n <= 7; ++n) for (int tc = 0; tc < 260; ++tc) {
    vector<int> vc(n);
    for (int &x : vc) x = rng() % 6;
    vector<Edge> es;
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
      if (u != v && (int)(rng() % 100) < 22 && (int)es.size() < 9)
        es.push_back({u, v, (int)(rng() % 9) + 1});
    int t = 1 + rng() % min(T, n);
    vector<int> term(n);
    iota(term.begin(), term.end(), 0);
    shuffle(term.begin(), term.end(), rng);
    term.resize(t);
    SteinerTree st;
    st.init(n);
    for (int i = 0; i < n; ++i) st.vcst[i] = vc[i];
    for (auto e : es) st.add_edge(e.u, e.v, e.w);
    int got = st.solve(term), want = oracle(n, es, term, vc);
    ++cases;
    if (got != want) {
      cerr << "minimum_steiner_tree mismatch case=" << cases << " n=" << n
           << " got=" << got << " want=" << want << " terminals=";
      for (int x : term) cerr << x << ',';
      cerr << " edges=" << es.size() << '\n';
      return 1;
    }
  }
  cout << "PASS cases=" << cases << " oracle=enumerated-edge-subsets-and-roots\n";
}
