#include "../codebook/1_Basic/Default_code.cpp"
namespace hull {
#include "../codebook/9_Else/DynamicConvexTrick.cpp"
}
namespace lp {
#include "../codebook/6_Math/Simplex_Algorithm.cpp"
}
#include "../codebook/9_Else/BitsetLCS.cpp"
#include "../codebook/9_Else/AdaptiveSimpson.cpp"
#include "../codebook/6_Math/QuadraticResidue.cpp"
#include "../codebook/6_Math/floor_sum.cpp"
namespace wm {
#include "../codebook/4_Flow_Matching/Maximum_Weight_Matching.cpp"
}
namespace mo {
#include "../codebook/9_Else/Mos_Algorithm_On_Tree.cpp"
}
struct Point {
  double x, y;
  Point(double x = 0, double y = 0) : x(x), y(y) {}
  Point operator-(Point q) const { return {x-q.x, y-q.y}; }
};
double Cross(Point a, Point b) { return a.x*b.y-a.y*b.x; }
#include "../codebook/8_Geometry/center.cpp"
mt19937_64 rng(1711);

void hull_test() {
  for (int t = 0; t < 300; ++t) {
    hull::DynamicHull h; vector<pll> lines;
    for (int i = 0; i < 80; ++i) {
      ll a = int(rng()%9)-4;
      ll b = ll(rng()%8000000000000000001ULL)-4000000000000000000LL;
      h.addline(a,b); lines.pb({a,b});
      for (ll x : {-h.kInf+1, -2LL, -1LL, 0LL, 1LL, 2LL, h.kInf}) {
        __int128 want = -((__int128)1<<126);
        for (auto [a,b] : lines) want = max(want, (__int128)a*x+b);
        assert(h.query(x) == want);
      }
    }
  }
  // Every rounded crossing, including negative nonintegral quotients.
  for (ll b = -13; b <= 13; ++b) for (ll a = -7; a <= 7; ++a)
    for (bool rev : {false,true}) {
      hull::DynamicHull h;
      if (rev) h.addline(a,b), h.addline(0,0);
      else h.addline(0,0), h.addline(a,b);
      for (ll x = -20; x <= 20; ++x)
        assert(h.query(x) == max(0LL,a*x+b));
    }
}
int lcs_dp(const vector<int> &a, const vector<int> &b) {
  vector<int> dp(b.size()+1);
  for (int x : a) {
    int old = 0;
    for (int j = 0; j < SZ(b); ++j) {
      int save = dp[j+1];
      dp[j+1] = x == b[j] ? old+1 : max(dp[j],dp[j+1]);
      old = save;
    }
  }
  return dp.back();
}
void lcs_test() {
  vector<vector<int>> words(511);
  for (int i = 1; i < SZ(words); ++i) {
    words[i] = words[(i-1)/2];
    words[i].pb(i%2 ? INT_MIN : INT_MAX);
  }
  for (auto &a : words) for (auto &b : words)
    assert(bitset_lcs(a,b) == lcs_dp(a,b));
  for (int n : {63,64,65,127,128,129,255,256,257})
    for (int t = 0; t < 40; ++t) {
      vector<int> a(n), b(n+1);
      for (int &v : a) v = rng()%3;
      for (int &v : b) v = rng()%3;
      if (t%3 == 0) sort(ALL(a));
      if (t%3 == 1) sort(ALL(b));
      assert(bitset_lcs(a,b) == lcs_dp(a,b));
      assert(bitset_lcs(b,a) == lcs_dp(a,b));
      assert(bitset_lcs(a,a) == n);
    }
}
ll power(ll a, int e, int p) {
  ll r = 1;
  for (; e; e >>= 1, a = a*a%p) if (e&1) r = r*a%p;
  return r;
}
void qr_test() {
  srand(1711);
  for (int p : {2,3,5,13,17,97,65537,998244353,2147483629,INT_MAX}) {
    for (int t = 0; t < 3000; ++t) {
      int a = t == 0 ? INT_MIN : t == 1 ? INT_MAX : int(rng());
      ll v = (ll(a)%p+p)%p;
      int r = QuadraticResidue(a,p);
      bool square = p == 2 || v == 0 || power(v,(p-1)/2,p) == 1;
      assert(square ? r >= 0 && r < p && ll(r)*r%p == v : r == -1);
    }
  }
  // An odd perfect-square modulus has Jacobi +1 on all coprime inputs.
  for (int k : {1,3,9,105,46339}) for (int t = 0; t < 300; ++t) {
    int a = int(rng()), m = k*k;
    assert(Jacobi(a,m) == (gcd(ll(a),ll(m)) == 1 ? 1 : 0));
  }
}
void floor_test() {
  for (int t = 0; t < 20000; ++t) {
    ll n = rng()%65, m = 1+rng()%1000000000000000LL;
    ll a = rng()%1000000000000000LL, b = rng()%1000000000000000LL;
    __int128 want = 0;
    for (ll i = 0; i < n; ++i) want += ((__int128)a*i+b)/m;
    assert(floor_sum(n,m,a,b) == want);
  }
  ll f = 1, g = 1;
  for (int k = 0; k < 70; ++k) {
    for (ll n : {0LL,1LL,2LL,17LL,1000LL}) {
      __int128 want = 0;
      for (ll i = 0; i < n; ++i) want += ((__int128)f*i+g-1)/g;
      assert(floor_sum(n,g,f,g-1) == want);
    }
    tie(f,g) = pair{g,f+g};
  }
  for (ll n : {0LL,1LL,LLONG_MAX})
    assert(floor_sum(n,LLONG_MAX,0,LLONG_MAX-1) == 0);
  assert(floor_sum(1,1,LLONG_MAX,LLONG_MAX) == LLONG_MAX);
}
void center_test() {
  vector<Point> base{{0,0},{1,0},{4,0},{4,1},{4,3},{2,3},{0,3},{0,2}};
  for (int e : {-20,0,20}) for (int dir = 0; dir < 2; ++dir) {
    double scale = ldexp(1.,e), shift = ldexp(1.,e+30);
    auto p = base;
    for (auto &q : p) q = {shift+q.x*scale,-shift+q.y*scale};
    if (dir) reverse(ALL(p));
    for (int start = 0; start < SZ(p); ++start) {
      auto got = BaryCenter(p,INT_MIN);
      assert(got.x == shift+2*scale && got.y == -shift+1.5*scale);
      rotate(p.begin(),p.begin()+1,p.end());
    }
  }
  vector<Point> triangle{{0,0},{1000000,1},{999999,1}};
  auto got = BaryCenter(triangle,0);
  assert(abs(got.x-1999999./3) < 1e-9 && abs(got.y-2./3) < 1e-12);
}
void simpson_test() {
  auto s = make_simpson([](double x) { return x*x*x*x; });
  for (auto [l,r] : vector<pll>{{-3,2},{2,-3},{0,0}}) {
    double want = (pow(double(r),5)-pow(double(l),5))/5;
    assert(abs(s.eval(l,r,1e-9)-want) < 1e-8);
    assert(abs(s.eval2(l,r,1e-9,31)-want) < 1e-8);
  }
  for (double width : {.1,.01}) {
    auto g = make_simpson([&](double x) {
      return exp(-pow((x-.137)/width,2));
    });
    double want = width*sqrt(acos(-1.))*.5*
      (erf((1-.137)/width)-erf((0-.137)/width));
    assert(abs(g.eval2(0,1,1e-10,1000)-want) < 1e-9);
    assert(abs(g.eval2(1,0,1e-10,1000)+want) < 1e-9);
  }
}
void lp_test() {
  for (int t = 0; t < 400; ++t) {
    int m = t%7; vector<double> target(m), objective(m);
    for (int j = 0; j < m; ++j)
      target[j] = rng()%6, objective[j] = int(rng()%11)-5;
    vector<vector<double>> A; vector<double> b;
    for (int j = 0; j < m; ++j) for (int sign : {-1,1}) {
      vector<double> row(m); row[j] = sign;
      A.pb(row); b.pb(sign*target[j]);
      A.pb(row); b.pb(sign*target[j]);
    }
    for (int i = 0; i < 10; ++i) {
      vector<double> row(m);
      for (double &v : row) v = int(rng()%9)-4;
      A.pb(row); b.pb(inner_product(ALL(row),target.begin(),0.));
    }
    vector<int> perm(A.size()); iota(ALL(perm),0);
    for (int rep = 0; rep < 3; ++rep) {
      shuffle(ALL(perm),rng);
      for (int i = 0; i < SZ(A); ++i) {
        copy(ALL(A[perm[i]]),lp::a[i]); lp::b[i] = b[perm[i]];
      }
      copy(ALL(objective),lp::c); fill_n(lp::x,lp::MAXM,-999);
      double want = inner_product(ALL(target),objective.begin(),0.);
      assert(abs(lp::simplex(SZ(A),m)-want) < 1e-7);
      for (int j = 0; j < m; ++j) assert(abs(lp::x[j]-target[j]) < 1e-7);
    }
  }
}
ll wm_oracle(const vector<vector<int>> &a) {
  int n = SZ(a); vector<ll> dp(1<<n);
  for (unsigned s = 1; s < dp.size(); ++s) {
    int u = countr_zero(s); unsigned rest = s^(1U<<u);
    dp[s] = dp[rest];
    for (int v = u+1; v < n; ++v) if (rest>>v&1)
      dp[s] = max(dp[s],dp[rest^(1U<<v)]+a[u][v]);
  }
  return dp.back();
}
void matching_test() {
  for (int t = 0; t < 180; ++t) {
    int n = t%13; wm::WeightGraph g(n);
    vector<vector<int>> a(n,vector<int>(n));
    for (int phase = 0; phase < 6; ++phase) {
      // Alternate full tight seeds, no edges, and blossom-heavy ties.
      for (int u = 0; u < n; ++u) for (int v = u+1; v < n; ++v) {
        int w = phase%3 == 0 ? 100000000 : phase%3 == 1 ? 0 :
          int(rng()%5)*25000000;
        a[u][v] = a[v][u] = w; g.add_edge(u+1,v+1,w);
      }
      for (int rep = 0; rep < 2; ++rep) {
        auto [weight,count] = g.solve(); ll sum = 0; int pairs = 0;
        assert(weight == wm_oracle(a));
        for (int u = 1; u <= n; ++u) if (int v = g.match[u]) {
          assert(1 <= v && v <= n && u != v && g.match[v] == u);
          assert(a[u-1][v-1] > 0);
          if (u < v) sum += a[u-1][v-1], ++pairs;
        }
        assert(sum == weight && pairs == count);
      }
    }
  }
}
void mo_test() {
  // All labelled trees up to n=5 via Prufer codes, every root and path.
  mo::solve({}, {}, [](int){abort();}, [](auto){abort();});
  for (int n = 1; n <= 5; ++n) {
    int codes = 1; for (int i = 0; i < n-2; ++i) codes *= n;
    for (int code = 0; code < codes; ++code) {
      vector<vector<int>> g(n); vector<int> seq, deg(n,1);
      for (int k = 0, z = code; k < n-2; ++k, z /= n)
        seq.pb(z%n), ++deg[z%n];
      auto edge = [&](int u,int v) { g[u].pb(v); g[v].pb(u); };
      for (int v : seq) {
        int u = find(ALL(deg),1)-deg.begin();
        edge(u,v); --deg[u]; --deg[v];
      }
      if (n > 1) {
        int u = find(ALL(deg),1)-deg.begin(); deg[u] = 0;
        int v = find(ALL(deg),1)-deg.begin(); edge(u,v);
      }
      for (int root = 0; root < n; ++root) {
        vector<int> in(n),out(n),ord,par(n,-1),dep(n);
        auto dfs = [&](auto &&self,int u) -> void {
          in[u] = SZ(ord); ord.pb(u);
          for (int v : g[u]) if (v != par[u])
            par[v] = u, dep[v] = dep[u]+1, self(self,v);
          out[u] = SZ(ord); ord.pb(u);
        }; dfs(dfs,root);
        auto lca = [&](int u,int v) {
          while (dep[u] > dep[v]) u = par[u];
          while (dep[v] > dep[u]) v = par[v];
          while (u != v) u = par[u], v = par[v];
          return u;
        };
        for (int block : {1,2,2*n,INT_MAX}) {
          vector<mo::Query> qs; vector<vector<int>> want;
          for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v) {
            vector<int> prev(n,-1), path(n); queue<int> bfs;
            prev[u] = u; bfs.push(u);
            while (!bfs.empty()) {
              int x = bfs.front(); bfs.pop();
              for (int y : g[x]) if (prev[y] == -1)
                prev[y] = x, bfs.push(y);
            }
            path[u] = 1;
            for (int x = v; x != u; x = prev[x]) path[x] = 1;
            qs.emplace_back(u,v,block,in,out,lca,SZ(want)); want.pb(path);
          }
          shuffle(ALL(qs),rng); vector<int> active(n); int count = 0;
          mo::solve(qs,ord,[&](int u){active[u] ^= 1;},
            [&](const mo::Query &q) {
              assert(active == want[q.id]); ++count;
            });
          assert(count == n*n);
          sort(ALL(qs)); vector<int> final(n);
          for (int i = qs.back().L; i <= qs.back().R; ++i)
            final[ord[i]] ^= 1;
          assert(active == final);
        }
      }
    }
  }
}
int main() {
  auto run = [](auto f, const char *name) {
    f(); cout << "PASS " << name << '\n' << flush;
  };
  run(hull_test,"hull"); run(lcs_test,"LCS"); run(qr_test,"roots");
  run(floor_test,"floor sum"); run(center_test,"centroid");
  run(simpson_test,"Simpson"); run(lp_test,"simplex");
  run(matching_test,"weighted matching"); run(mo_test,"tree Mo");
  cout << "PASS round17 g11: all nine adversarial audits\n";
}
