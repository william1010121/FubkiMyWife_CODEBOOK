#include "../../codebook/1_Basic/Default_code.cpp"
namespace gh {
constexpr int MAXN = 8, INF = INT_MAX;
#include "../../codebook/4_Flow_Matching/Dinic.cpp"
#include "../../codebook/4_Flow_Matching/Gomory_Hu_tree.cpp"
}
namespace lcs {
#include "../../codebook/9_Else/All_LCS.cpp"
}
namespace gas {
#include "../../codebook/6_Math/Gaussian_Eliminatin.cpp"
}
namespace hld {
constexpr int N = 65;
#include "../../codebook/3_Data_Structure/Dynamic_2D_Segment_Tree.cpp"
#include "../../codebook/3_Data_Structure/Heavy_light_Decomposition.cpp"
}
namespace bcc {
#include "../../codebook/2_Graph/BCC_Vertex.cpp"
}
namespace fourier {
#include "../../codebook/7_Polynomial/FFT_chrislaiisme.cpp"
}
namespace cover {
constexpr int N = 12;
void fwt(int *a, int n, int) {
  for (int bit = 1; bit < n; bit *= 2)
    for (int mask = 0; mask < n; ++mask)
      if (mask & bit) a[mask] += a[mask ^ bit];
}
#include "../../codebook/2_Graph/Minimum_Clique_Cover.cpp"
}
namespace mp {
#include "../../codebook/9_Else/min_plus_convolution.cpp"
}
mt19937_64 rng(1705);
void require(bool ok, const char *why) {
  if (!ok) throw runtime_error(why);
}

void gh_case(int n, vector<tuple<int,int,int>> es) {
  gh::Dinic.init(n);
  vector<vector<ll>> cap(n, vector<ll>(n));
  shuffle(ALL(es), rng);
  for (auto [u,v,c] : es) {
    gh::Dinic.add_edge(u,v,c), gh::Dinic.add_edge(v,u,c);
    cap[u][v] += c, cap[v][u] += c;
  }
  // A previous flow must be erased before generating the tree.
  if (n > 1) gh::Dinic.maxflow(n-1,0);
  for (int repeat = 0; repeat < 2; ++repeat) {
    gh::GomoryHu(n);
    vector<vector<pii>> tree(n);
    for (int u = 1; u < n; ++u) {
      require(0 <= gh::g[u] && gh::g[u] < n, "GH parent domain");
      tree[u].pb({gh::g[u],gh::gh_w[u]});
      tree[gh::g[u]].pb({u,gh::gh_w[u]});
    }
    for (int s = 0; s < n; ++s) {
      vector<int> q{s}, seen(n), bottleneck(n,INT_MAX); seen[s] = 1;
      for (int at = 0; at < SZ(q); ++at)
        for (auto [v,w] : tree[q[at]]) if (!seen[v]) {
          seen[v] = 1, q.pb(v);
          bottleneck[v] = min(bottleneck[q[at]],w);
        }
      require(SZ(q) == n, "GH connected acyclic parent tree");
      for (int t = s+1; t < n; ++t) {
        ll best = LLONG_MAX;
        for (int mask = 0; mask < (1<<n); ++mask)
          if ((mask>>s&1) && !(mask>>t&1)) {
            ll cut = 0;
            for (int u = 0; u < n; ++u) if (mask>>u&1)
              for (int v = 0; v < n; ++v)
                if (!(mask>>v&1)) cut += cap[u][v];
            require(cut <= INT_MAX, "GH oracle int-cut precondition");
            best = min(best,cut);
          }
        require(bottleneck[t] == best, "GH all separating subsets");
      }
    }
  }
}
void audit_gh() {
  gh_case(0,{}), gh_case(1,{}), gh_case(2,{{0,1,INT_MAX}});
  // Exhaust all K5 supports at both unit and large fitting capacities.
  for (int mask = 0; mask < 1024; ++mask) {
    vector<tuple<int,int,int>> es;
    for (int u = 0, bit = 0; u < 5; ++u)
      for (int v = u+1; v < 5; ++v, ++bit) if (mask>>bit&1) {
        int c = mask&1 ? INT_MAX/8 : 1;
        es.pb({u,v,c/2}), es.pb({v,u,c-c/2});
      }
    gh_case(5,es);
  }
}

set<string> subsequences(const string &s) {
  set<string> ret;
  for (int mask = 0; mask < (1<<SZ(s)); ++mask) {
    string x;
    for (int i = 0; i < SZ(s); ++i) if (mask>>i&1) x += s[i];
    ret.insert(x);
  }
  return ret;
}
void audit_lcs() {
  vector<string> strings;
  for (int n = 0; n <= 6; ++n) for (int mask = 0; mask < (1<<n);
                                  ++mask) {
    string s(n, '\0');
    for (int i = 0; i < n; ++i) if (mask>>i&1) s[i] = char(255);
    strings.pb(s);
  }
  vector<set<string>> subs;
  for (auto &s : strings) subs.pb(subsequences(s));
  for (int i = 0; i < SZ(strings); ++i)
    for (int j = 0; j < SZ(strings); ++j) {
      vector<string> want; size_t best = 0;
      // No reconstruction recurrence is shared with the template.
      for (auto &x : subs[i]) if (subs[j].count(x)) {
        if (x.size() > best) best = x.size(), want.clear();
        if (x.size() == best) want.pb(x);
      }
      require(lcs::all_lcs(strings[i],strings[j]) == want,
              "LCS exhaustive binary-byte subsequence intersection");
    }
  string equal(200000,'\0');
  for (int i = 0; i < SZ(equal); ++i) equal[i] = char(i%256);
  require(lcs::all_lcs(equal,equal) == vector<string>{equal},
          "LCS equal-input linear-space stress");
}

void audit_gas() {
  vector<vector<double>> empty; gas::GAS(empty);
  for (int tc = 0; tc < 1000; ++tc) {
    int n = 1+rng()%8, w = n+rng()%5;
    vector<int> piv;
    for (int c = 0; c < n; ++c) if (rng()%2) piv.pb(c);
    vector<vector<double>> want(n,vector<double>(w));
    for (int r = 0; r < SZ(piv); ++r) {
      want[r][piv[r]] = 1;
      for (int c = piv[r]+1; c < w; ++c)
        if (find(ALL(piv),c) == piv.end())
          want[r][c] = int(rng()%9)-4;
    }
    auto a = want;
    // Exact row operations create rank gaps and arbitrary row orders.
    for (int step = 0; step < 4*n; ++step) {
      int u = rng()%n, v = rng()%n;
      if (u != v) for (int c = 0; c < w; ++c) a[u][c] += a[v][c];
    }
    for (auto &row : a) {
      int e = int(rng()%7)-3;
      for (double &x : row) x = ldexp(x,e);
    }
    shuffle(ALL(a),rng); gas::GAS(a);
    for (int r = 0; r < n; ++r) for (int c = 0; c < w; ++c)
      require(isfinite(a[r][c]) && abs(a[r][c]-want[r][c]) < 1e-6,
              "GAS known RREF rank gaps/RHS/dyadic scaling");
  }
  // Huge, finite, exactly reducible rows: no rounding-sensitive rank test.
  vector<vector<double>> diagonal(8,vector<double>(10));
  for (int i = 0; i < 8; ++i) {
    diagonal[i][i] = 0x1p600;
    diagonal[i][8] = (i-4)*0x1p600;
    diagonal[i][9] = (i+1)*0x1p600;
  }
  gas::GAS(diagonal);
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j)
      require(diagonal[i][j] == (i == j), "GAS huge diagonal");
    require(diagonal[i][8] == i-4 && diagonal[i][9] == i+1,
            "GAS huge multiple RHS");
  }
  for (double x : {1e-12,-1e-12,nextafter(1e-12,0.),
                   -nextafter(1e-12,0.),0.,0x1p600}) {
    vector<vector<double>> a{{x,2*x}};
    gas::GAS(a);
    auto want = abs(x) < 1e-12 ? vector<double>{x,2*x}
                              : vector<double>{1,2};
    require(a[0] == want, "GAS exact epsilon and huge dyadic pivot");
  }
}
ll power(ll a, int e, int p) {
  ll r = 1;
  for (; e; e /= 2, a = a*a%p) if (e&1) r = r*a%p;
  return r;
}
void inverse_case(int mod, int N) {
  vector<int> inv(N+2,-1234567);
#include "../../codebook/6_Math/Mod_Inverse.cpp"
  require(inv[0] == -1234567 && inv[N+1] == -1234567,
          "inverse untouched guard entries");
  for (int i = 1; i <= N; ++i)
    require(inv[i] == power(i,mod-2,mod), "inverse Fermat oracle");
}
void audit_inverse() {
  for (int p = 2; p <= 2000; ++p) {
    bool prime = true;
    for (int d = 2; d*d <= p; ++d) if (p%d == 0) prime = false;
    if (prime) inverse_case(p,p-1);
  }
  inverse_case(INT_MAX,65536), inverse_case(1000000007,65536);
}

void dispose(hld::Seg::Node *p) {
  if (p) { dispose(p->l); dispose(p->r); delete p; }
}
void audit_hld() {
  hld::Heavy_light_Decomposition h;
  for (int tc = 0; tc < 80; ++tc) {
    int n = tc%4 == 0 ? 64 : 1+rng()%64; h.init(n);
    vector<vector<int>> g(n+1); vector<int> value(n+1);
    vector<pii> es;
    for (int u = 2; u <= n; ++u) {
      int v = tc%3 == 0 ? 1 : tc%3 == 1 ? u-1 : 1+rng()%(u-1);
      es.pb({u,v}), g[u].pb(v), g[v].pb(u);
    }
    shuffle(ALL(es),rng);
    for (auto [u,v] : es) h.add_edge(u,v);
    for (int u = 1; u <= n; ++u)
      value[u] = h.val[u] = u%2 ? 1000 : -1000;
    h.build();
    for (int step = 0; step < 12; ++step) {
      int l = rng()%(n+1), r = rng()%(n+1); if (l > r) swap(l,r);
      int delta = int(rng()%201)-100;
      if (step == 0) l = 0, r = n;
      if (step == 1) r = l; // empty update
      h.seg.update(l,r,delta);
      for (int u = 1; u <= n; ++u)
        if (l <= h.pl[u] && h.pl[u] < r) value[u] += delta;
      for (int s = 1; s <= n; ++s) {
        vector<int> q{s}, parent(n+1), sum(n+1);
        parent[s] = s, sum[s] = value[s];
        for (int at = 0; at < SZ(q); ++at)
          for (int v : g[q[at]]) if (v != parent[q[at]])
            parent[v] = q[at], sum[v] = sum[q[at]]+value[v], q.pb(v);
        for (int t = 1; t <= n; ++t)
          require(h.query(s,t) == sum[t], "HLD all paths after updates");
      }
    }
    dispose(h.seg.root); h.seg.root = nullptr;
  }
  for (int x : {INT_MIN,INT_MAX,0}) {
    h.init(1), h.val[1] = x, h.build();
    require(h.query(1,1) == x, "HLD signed extreme singleton");
    dispose(h.seg.root); h.seg.root = nullptr;
  }
}

vector<int> reachable(const vector<vector<int>> &g, int s, int skip) {
  vector<int> seen(SZ(g)), q{s}; seen[s] = 1;
  for (int at = 0; at < SZ(q); ++at)
    for (int v : g[q[at]]) if (v != skip && !seen[v])
      seen[v] = 1, q.pb(v);
  return seen;
}
void audit_bcc() {
  for (int tc = 0; tc < 1000; ++tc) {
    int n = tc%11; bcc::BCC b(n); vector<vector<int>> g(n);
    for (int u = 0; u < n; ++u) for (int v = u; v < n; ++v)
      if (rng()%3 == 0) for (int copies = 1+rng()%4; copies; --copies)
        b.add_edge(v,u), g[u].pb(v), g[v].pb(u);
    b.solve(), b.block_cut_tree();
    // Deleting an articulation's forest node must exactly describe
    // connectivity after deleting that original vertex.
    for (int removed = -1; removed < n; ++removed)
      for (int s = 0; s < n; ++s) if (s != removed) {
        auto original = reachable(g,s,removed);
        int skip = removed >= 0 && b.is_ap[removed]
                 ? b.bln[removed] : -1;
        auto forest = reachable(b.nG,b.bln[s],skip);
        for (int t = 0; t < n; ++t) if (t != removed)
          require(original[t] == forest[b.bln[t]],
                  "BCC block-cut vertex-deletion connectivity");
      }
  }
}

vector<int> product(const vector<int> &a, const vector<int> &b) {
  if (a.empty() || b.empty()) return {};
  vector<ll> wide(a.size()+b.size()-1);
  for (int i = 0; i < SZ(a); ++i) for (int j = 0; j < SZ(b); ++j)
    wide[i+j] += ll(a[i])*b[j];
  vector<int> ret;
  for (ll x : wide) {
    require(INT_MIN <= x && x <= INT_MAX, "FFT oracle int bounds");
    ret.pb(x);
  }
  return ret;
}
void fft_case(vector<int> a, vector<int> b) {
  auto old_a = a, old_b = b; auto want = product(a,b);
  require(fourier::mult(a,b) == want, "FFT quadratic convolution");
  require(fourier::mult(b,a) == want, "FFT operand symmetry");
  require(a == old_a && b == old_b, "FFT input immutability");
}
void audit_fft() {
  fft_case({INT_MIN},{1}), fft_case({INT_MAX},{1});
  fft_case({INT_MIN},{}), fft_case({},{});
  // One sparse large coefficient exercises negative rounding and
  // cancellation; equal objects and almost-equal final bytes diverge.
  for (int n : {1,2,3,7,8,9,31,32,33,127,128,129,511,512,513}) {
    vector<int> a(n), b(n);
    for (int &x : a) x = int(rng()%11)-5;
    b = a; fft_case(a,b);
    require(fourier::mult(a,a) == product(a,a), "FFT alias square");
    ++b.back(); fft_case(a,b);
    fill(ALL(a),0), fill(ALL(b),0);
    a[n/2] = -100000000, b.front() = 7, b.back() = -7;
    fft_case(a,b);
  }
  // Every single spectral mode, complex input, both transform signs.
  for (int n : {1,2,4,8,16,32,64}) for (int k = 0; k < n; ++k) {
    vector<fourier::cd> a(n);
    for (int j = 0; j < n; ++j)
      a[j] = polar(1.,-2*fourier::pi*k*j/n);
    fourier::fft(a);
    for (int j = 0; j < n; ++j)
      require(abs(a[j]-fourier::cd(j == k ? n : 0)) < 1e-9,
              "FFT isolated spectral mode");
    fourier::ifft(a);
    for (int j = 0; j < n; ++j)
      require(abs(a[j]-polar(1.,-2*fourier::pi*k*j/n)) < 1e-10,
              "FFT spectral mode inverse scaling");
  }
}

void audit_cover() {
  static cover::Clique_Cover c;
  for (int tc = 0; tc < 1500; ++tc) {
    int n = tc%13, m = 1<<n; c.init(n);
    vector<int> adjacent(n), exact(m,n+1), clique(m,1); exact[0] = 0;
    for (int u = 0; u < n; ++u) for (int v = u+1; v < n; ++v)
      if (rng()%2) {
        c.add_edge(u,v), c.add_edge(v,u);
        adjacent[u] |= 1<<v, adjacent[v] |= 1<<u;
      }
    for (int mask = 1; mask < m; ++mask) {
      for (int u = 0; u < n; ++u) if (mask>>u&1)
        for (int v = u+1; v < n; ++v) if (mask>>v&1)
          clique[mask] &= bool(adjacent[u]>>v&1);
      // Independent exact partition DP, no transform or residue math.
      int first = mask&-mask;
      for (int sub = mask; sub; sub = (sub-1)&mask)
        if ((sub&first) && clique[sub])
          exact[mask] = min(exact[mask],1+exact[mask^sub]);
    }
    fill_n(c.co,m,INT_MIN), fill_n(c.dp,m,UINT_MAX);
    require(c.solve() == exact.back() && c.solve() == exact.back(),
            "cover exact partition oracle and poisoned scratch reuse");
  }
}

void audit_minplus() {
  for (int n = 0; n <= 7; ++n) for (int m = 0; m <= 7; ++m)
    for (int tc = 0; tc < 150; ++tc) {
      vector<ll> a(n), b(m); vector<int> slopes;
      for (int i = 1; i < n; ++i) slopes.pb(int(rng()%9)-4);
      sort(ALL(slopes));
      if (n) a[0] = -20;
      for (int i = 1; i < n; ++i) a[i] = a[i-1]+slopes[i-1];
      for (ll &x : b) x = int(rng()%41)-20;
      // Near either ll endpoint, every pair sum still fits exactly.
      ll shift = tc%3 == 0 ? LLONG_MIN+100
               : tc%3 == 1 ? LLONG_MAX-100 : 0;
      for (ll &x : a) x += shift;
      vector<ll> want(n && m ? n+m-1 : 0,LLONG_MAX);
      for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) {
        __int128 sum = (__int128)a[i]+b[j];
        require(LLONG_MIN <= sum && sum <= LLONG_MAX,
                "minplus exact oracle preconditions");
        want[i+j] = min(want[i+j],ll(sum));
      }
      require(mp::min_plus_convolution(a,b) == want,
              "minplus convex slopes/ties/extreme sums");
    }
}
int main() {
  audit_gh(); audit_lcs(); audit_gas(); audit_inverse(); audit_hld();
  audit_bcc(); audit_fft(); audit_cover(); audit_minplus();
  cout << "PASS round17 g05: nine independent correctness audits\n";
}
