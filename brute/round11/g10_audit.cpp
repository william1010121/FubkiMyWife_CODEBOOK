#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/6_Math/Gaussian_gcd.cpp"
#include "../../codebook/6_Math/floor_sum.cpp"
#include "../../codebook/9_Else/CDQ_Dominance.cpp"
namespace nt {
#include "../../codebook/7_Polynomial/NTT_chrislaiisme.cpp"
}
namespace dt {
constexpr int N = 65;
#include "../../codebook/2_Graph/Dominator_Tree.cpp"
dominator_tree solver;
}
namespace sat {
#include "../../codebook/2_Graph/SCC.cpp"
#include "../../codebook/2_Graph/2SAT.cpp"
}
namespace heap {
#include "../../codebook/3_Data_Structure/Leftist_Tree.cpp"
}
namespace db {
#include "../../codebook/5_String/De_Bruijn_sequence.cpp"
}
using wide = __int128_t;
using Z = complex<wide>;
mt19937_64 rng(0x110010);
void require(bool ok) { if (!ok) abort(); }
wide norm2(Z z) { return z.real()*z.real()+z.imag()*z.imag(); }
Z gaussian_oracle(Z a, Z b) {
  while (b != Z{}) {
    wide r = norm2(b);
    auto nearest = [&](wide x) {
      wide q = x/r, rem = x%r;
      if (2*(rem < 0 ? -rem : rem) >= r) q += rem < 0 ? -1 : 1;
      return q;
    };
    Z z = a*conj(b), q(nearest(z.real()), nearest(z.imag()));
    a -= q*b; swap(a,b);
  }
  return a;
}
void gaussian() {
  for (int t = 0; t < 20000; ++t) {
    auto value = [&] { return ll(rng()%2000000001)-1000000000; };
    complex<ll> a(value(),value()), b(value(),value());
    if (b == complex<ll>{}) continue;
    auto got = gaussian_gcd(a,b);
    Z az(a.real(),a.imag()), bz(b.real(),b.imag());
    Z gz(got.real(),got.imag());
    require(norm2(gz) == norm2(gaussian_oracle(az,bz)));
    for (Z z : {az,bz}) {
      Z product = z*conj(gz);
      require(product.real()%norm2(gz) == 0 &&
              product.imag()%norm2(gz) == 0);
    }
  }
  // Exact-divisor shortcut avoids otherwise overflowing rounding terms.
  for (ll x : {LLONG_MIN,LLONG_MAX}) {
    require(gaussian_gcd({x,0},{1,0}) == complex<ll>(1,0));
    require(gaussian_gcd({0,x},{0,1}) == complex<ll>(0,1));
  }
}
wide floor_oracle(wide n, wide m, wide a, wide b) {
  wide ans = 0;
  for (;;) {
    ans += n*(n-1)/2*(a/m)+n*(b/m); a %= m; b %= m;
    wide y = a*n+b;
    if (y < m) return ans;
    n = y/m; b = y%m; swap(m,a);
  }
}
void floors() {
  for (ll m : {1LL,2LL,LLONG_MAX/2,LLONG_MAX})
    for (ll a : {0LL,1LL,LLONG_MAX})
      for (ll b : {0LL,1LL,LLONG_MAX}) {
        require(floor_sum(0,m,a,b) == 0);
        // The reduced a+b evaluated at n=1 must fit, too.
        if (wide(a%m)+b%m <= LLONG_MAX)
          require(floor_sum(1,m,a,b) == wide(b)/m);
      }
  for (int t = 0; t < 20000; ++t) {
    ll n = rng()%1001, bound = LLONG_MAX/(n*n+n+2);
    ll m = 1+rng()%bound, a = rng()%bound, b = rng()%bound;
    require(floor_sum(n,m,a,b) == floor_oracle(n,m,a,b));
    ll cut = n ? rng()%(n+1) : 0;
    require(floor_sum(n,m,a,b) == floor_sum(cut,m,a,b)+
            floor_sum(n-cut,m,a,a*cut+b));
  }
}
void dominators() {
  for (int t = 0; t < 150; ++t) {
    int n = t%3 ? 1+rng()%64 : 64;
    dt::solver.init(n); vector<vector<int>> g(n+1);
    auto edge = [&](int u, int v) {
      g[u].pb(v); dt::solver.add_edge(u,v);
    };
    for (int i = 0; i < n*3; ++i) edge(1+rng()%n,1+rng()%n);
    for (int pass = 0; pass < 3; ++pass) {
      int root = 1+rng()%n; dt::solver.tarjan(root);
      auto reach = [&](int removed) {
        vector<bool> seen(n+1); vector<int> q;
        if (removed != root) seen[root] = true, q.pb(root);
        for (int i = 0; i < SZ(q); ++i) for (int v : g[q[i]])
          if (v != removed && !seen[v]) seen[v] = true, q.pb(v);
        return seen;
      };
      vector<int> parent(n+1); auto all = reach(0);
      for (int u = 1; u <= n; ++u) for (int v : dt::solver.tree[u]) {
        require(!parent[v] && all[u] && all[v]); parent[v] = u;
      }
      for (int removed = 1; removed <= n; ++removed) {
        auto seen = reach(removed);
        for (int v = 1; v <= n; ++v) {
          bool ancestor = false; int steps = 0;
          for (int u = v; u && all[v]; u = parent[u]) {
            require(++steps <= n); ancestor |= u == removed;
          }
          require(ancestor == bool(all[v] && !seen[v]));
        }
      }
      // A new solve must reflect edges added since the preceding solve.
      for (int v = 1; v <= n; ++v) edge(root,v);
    }
  }
}
void sats() {
  for (int t = 0; t < 500; ++t) {
    int n = t%9; sat::SAT s(n); vector<pii> clauses;
    if (n) for (int i = 0; i < 30; ++i) {
      int a = rng()%(2*n), b = rng()%(2*n);
      clauses.pb({a,b}); s.add_clause(a,b);
    }
    auto accepts = [&](const vector<bool> &values) {
      for (auto [a,b] : clauses) if (!values[a] && !values[b])
        return false;
      return true;
    };
    bool want = false;
    for (int mask = 0; mask < (1<<n); ++mask) {
      vector<bool> v(2*n);
      for (int i = 0; i < n; ++i) v[i] = mask>>i&1, v[i+n] = !v[i];
      want |= accepts(v);
    }
    for (int pass = 0; pass < 3; ++pass) {
      require(s.solve() == want);
      if (want) {
        require(accepts(s.istrue));
        for (int i = 0; i < n; ++i)
          require(s.istrue[i] != s.istrue[i+n]);
      }
    }
  }
}
void cdqs() {
  for (int t = 0; t < 500; ++t) {
    int n = rng()%33, k = rng()%9;
    vector<vector<int>> a(n,vector<int>(k));
    const int vals[] = {INT_MIN,-1,0,1,INT_MAX};
    for (auto &p : a) for (int &x : p) x = vals[rng()%5];
    vector<ll> want(n);
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
      bool yes = i != j;
      for (int d = 0; d < k; ++d) yes &= a[j][d] <= a[i][d];
      want[i] += yes;
    }
    require(dominance(a) == want);
    vector<int> perm(k); iota(ALL(perm),0); shuffle(ALL(perm),rng);
    auto b = a;
    for (int i = 0; i < n; ++i) {
      for (int d = 0; d < k; ++d) b[i][d] = a[i][perm[d]];
      b[i].pb(INT_MIN); b[i].pb(INT_MAX);
    }
    require(dominance(b) == want);
    for (int i = 0; i < n; ++i) b.pb(b[i]);
    auto got = dominance(b);
    for (int i = 0; i < 2*n; ++i) require(got[i] == 2*want[i%n]+1);
  }
}
void transforms() {
  for (int n : {1,2,4,8,32,256,1024}) {
    vector<ll> a(n); for (ll &x : a) x = rng()%nt::mod;
    auto b = a; nt::ntt(b); nt::ntt(b);
    // F(F(a))[i] = n*a[-i]; independently tests root sign/permutation.
    for (int i = 0; i < n; ++i)
      require(b[i] == n*a[(n-i)%n]%nt::mod);
    nt::intt(b); nt::intt(b); require(b == a);
    require(nt::mult(a,{nt::mod-1}) == [&] {
      auto c = a; for (ll &x : c) x = x ? nt::mod-x : 0; return c;
    }());
    require(nt::mult(a,{}).empty() && nt::mult({},a).empty());
  }
}
void necklaces() {
  for (int n : {2,3,31,255,2048,4096}) {
    vector<int> a(n+4,-7); fill_n(db::dbs.buf,db::MAXC*db::MAXN,-3);
    db::dbs.solve(INT_MAX,n,3,a.data()+1);
    require(a.front() == -7 && a.back() == -7);
    set<vector<int>> windows;
    for (int i = 0; i < 3; ++i)
      require(windows.insert(vector<int>(a.begin()+1+i,
                                         a.begin()+1+i+n)).second);
    for (int i = 1; i <= n+2; ++i) require(a[i] >= 0 && a[i] < INT_MAX);
  }
  vector<int> a(65,-7); db::dbs.solve(INT_MAX,1,64,a.data());
  for (int i = 0; i < 64; ++i) require(a[i] == i);
  require(a.back() == -7); db::dbs.solve(1,0,1,nullptr);
}
void heaps() {
  // All pop orders remain safe despite near-limit positive/negative sums.
  for (ll high : {LLONG_MAX/4,LLONG_MAX/3}) {
    heap::node *h = nullptr; multiset<ll> ref;
    for (ll x : {high,-high,high,-high,0LL,high,-high}) {
      h = heap::merge(h,new heap::node(x)); ref.insert(x);
    }
    while (h) {
      vector<heap::node *> q{h}; wide total = 0;
      for (int i = 0; i < SZ(q); ++i) {
        auto p = q[i]; total += p->data;
        require(heap::V(p->l) >= heap::V(p->r));
        require(p->sum == wide(heap::sum(p->l))+heap::sum(p->r)+p->data);
        if (p->l) q.pb(p->l);
        if (p->r) q.pb(p->r);
      }
      require(h->sz == SZ(ref) && total == h->sum);
      require(h->data == *ref.rbegin());
      ref.erase(prev(ref.end())); heap::pop(h);
    }
    require(ref.empty());
  }
}
int main() {
  gaussian(); floors(); dominators(); sats(); cdqs(); transforms();
  necklaces(); heaps(); cout << "PASS round11 g10 independent audit\n";
}
