#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128;
#define pb push_back
namespace determinant {
constexpr int MAXN = 8;
ll P;
#include "../../codebook/6_Math/Determinant.cpp"
}
namespace covering {
constexpr int N = 14;
void fwt(int *a, int n, int) {
  for (int bit = 1; bit < n; bit *= 2)
    for (int s = 0; s < n; ++s)
      if (s & bit) a[s] += a[s ^ bit];
}
#include "../../codebook/2_Graph/Minimum_Clique_Cover.cpp"
}
namespace rational {
#include "../../codebook/6_Math/Fraction.cpp"
}
namespace components {
#include "../../codebook/2_Graph/SCC.cpp"
}
namespace dominators {
constexpr int N = 65;
#include "../../codebook/2_Graph/Dominator_Tree.cpp"
}
namespace mo {
#include "../../codebook/9_Else/Mos_Algorithm_With_modification.cpp"
}
namespace binomial {
#include "../../codebook/6_Math/Mod_Arithmetic.cpp"
#include "../../codebook/6_Math/Mod_Binomial.cpp"
}
namespace rounding {
#include "../../codebook/6_Math/floor_ceil.cpp"
}

void require(bool ok, const char *why) {
  if (!ok) { cerr << "FAIL round14 " << why << '\n'; exit(1); }
}

void determinant_audit() {
  using namespace determinant;
  mt19937_64 rng(140201);
  for (ll p : {1LL, 2LL, 8LL, 27LL, 1000000006LL,
               LLONG_MAX - 1, LLONG_MAX}) {
    P = p;
    for (int tc = 0; tc < 240; ++tc) {
      Matrix a{}; a.n = a.m = tc % 7;
      ll choices[] = {0, 1 % p, p / 2, (p - 1) / 2, p - 1};
      for (int i = 0; i < a.n; ++i)
        for (int j = 0; j < a.n; ++j)
          a.M[i][j] = tc & 1 ? choices[rng() % 5] : rng() % p;
      if (tc % 11 == 0 && a.n > 1)
        copy_n(a.M[0], a.n, a.M[a.n - 1]);
      vector<int> perm(a.n); iota(perm.begin(), perm.end(), 0);
      ll want = 0;
      do {
        ll product = 1 % p; int parity = 0;
        for (int i = 0; i < a.n; ++i) {
          product = i128(product) * a.M[i][perm[i]] % p;
          for (int j = 0; j < i; ++j) parity ^= perm[j] > perm[i];
        }
        want = (i128(want) + (parity ? p - product : product)) % p;
      } while (next_permutation(perm.begin(), perm.end()));
      require(a.det() == want, "det extreme residues/permutation oracle");
      for (int i = 0; i < a.n; ++i)
        for (int j = 0; j < a.n; ++j)
          require(0 <= a.M[i][j] && a.M[i][j] < p,
                  "det keeps canonical residues on singular exits");
    }
  }
}

void cover_audit() {
  // Triangle-free graphs: a clique has at most two vertices, so minimum
  // cover = n - maximum matching; use independent subset matching DP.
  static covering::Clique_Cover cc;
  mt19937 rng(140202);
  for (int tc = 0; tc < 400; ++tc) {
    int n = tc % 15; cc.init(n);
    vector<int> adj(n), order(n);
    iota(order.begin(), order.end(), 0);
    shuffle(order.begin(), order.end(), rng);
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      if (u % 2 != v % 2 && rng() % 4) {
        int a = order[u], b = order[v];
        cc.add_edge(a, b), cc.add_edge(b, a);
        adj[a] |= 1 << b, adj[b] |= 1 << a;
      }
    vector<int> match(1 << n);
    for (int s = 1; s < (1 << n); ++s) {
      int u = __builtin_ctz(unsigned(s)), rest = s ^ (1 << u);
      match[s] = match[rest];
      for (int t = rest & adj[u]; t; t &= t - 1)
        match[s] = max(match[s], 1 + match[rest ^ (t & -t)]);
    }
    for (int repeat = 0; repeat < 2; ++repeat) {
      fill_n(cc.co, 1 << covering::N, INT_MIN);
      fill_n(cc.dp, 1 << covering::N, UINT_MAX);
      require(cc.solve() == n - match.back(),
              "cover vs matching, shuffled labels/poisoned scratch");
    }
  }
}

bool fits(i128 x) { return -i128(LLONG_MAX) <= x && x <= LLONG_MAX; }
void rational_audit() {
  using rational::fraction;
  vector<ll> values{0, 1, -1, 2, -2, 3037000499LL, -3037000499LL,
                    LLONG_MAX / 2, -LLONG_MAX / 2,
                    LLONG_MAX - 1, 1 - LLONG_MAX, LLONG_MAX, -LLONG_MAX};
  vector<fraction> fs;
  for (ll n : values) for (ll d : values) if (d) {
    fraction f(n, d); fs.pb(f);
    require(f.d > 0 && gcd(f.n, f.d) == 1 &&
            i128(f.n) * d == i128(n) * f.d, "fraction canonicalization");
    ostringstream out;
    auto *old = cout.rdbuf(out.rdbuf()); f.print(); cout.rdbuf(old);
    string want = to_string(f.n);
    if (f.d != 1) want += "/" + to_string(f.d);
    require(out.str() == want, "fraction print");
  }
  int operations = 0;
  for (auto a : fs) for (auto b : fs) {
    i128 cross1 = i128(a.n) * b.d, cross2 = i128(b.n) * a.d;
    i128 den = i128(a.d) * b.d, num = i128(a.n) * b.n;
    auto check = [&](fraction f, i128 n, i128 d) {
      require(f.d > 0 && gcd(f.n, f.d) == 1 &&
              i128(f.n) * d == n * f.d, "fraction operation oracle");
      ++operations;
    };
    // Check every evaluated ll intermediate before reducing the result.
    if (fits(cross1) && fits(cross2) && fits(den)) {
      if (fits(cross1 + cross2)) check(a + b, cross1 + cross2, den);
      if (fits(cross1 - cross2)) check(a - b, cross1 - cross2, den);
    }
    if (fits(num) && fits(den)) check(a * b, num, den);
    if (b.n && fits(cross1) && fits(cross2))
      check(a / b, cross1, cross2);
  }
  require(operations > 10000, "fraction substantial legal edge coverage");
}

vector<char> reach(const vector<vector<int>> &g, int root, int blocked) {
  vector<char> seen(g.size()); vector<int> q;
  if (root != blocked) seen[root] = 1, q.pb(root);
  for (int i = 0; i < (int)q.size(); ++i)
    for (int v : g[q[i]]) if (v != blocked && !seen[v])
      seen[v] = 1, q.pb(v);
  return seen;
}
void directed_audit() {
  components::SCC empty(0); empty.solve(); empty.solve();
  require(empty.nscc == 0 && empty.stk.empty(), "empty SCC");
  static dominators::dominator_tree dt;
  mt19937 rng(140203);
  for (int tc = 0; tc < 400; ++tc) {
    int n = 1 + tc % 64;
    vector<int> order(n); iota(order.begin(), order.end(), 0);
    shuffle(order.begin(), order.end(), rng);
    vector<vector<int>> g(n); components::SCC scc(n); dt.init(n);
    auto add = [&](int u, int v) {
      g[u].pb(v); scc.add_edge(u, v); dt.add_edge(u + 1, v + 1);
    };
    for (int i = 0; i < n; ++i) {
      add(order[i], order[i]);
      if (i + 1 < n && i % 7 != 6) {
        add(order[i], order[i + 1]); add(order[i], order[i + 1]);
        if (tc & 1) add(order[i + 1], order[i]);
      }
    }
    for (int i = 0; i < n; ++i) {
      int u = rng() % n, v = rng() % n;
      if (tc % 3) add(u, v);
    }
    scc.solve(); auto bln = scc.bln; scc.solve();
    require(bln == scc.bln && scc.stk.empty(), "SCC repeated solve");
    for (int u = 0; u < n; ++u) {
      auto from = reach(g, u, -1);
      require(!scc.instack[u], "SCC empties active markers");
      for (int v = 0; v < n; ++v) {
        auto back = reach(g, v, -1);
        require((scc.bln[u] == scc.bln[v]) == (from[v] && back[u]),
                "SCC block-chain reachability oracle");
      }
      for (int v : g[u]) require(scc.bln[u] >= scc.bln[v],
                                "SCC reverse topological IDs");
    }
    // Same object: change roots, then insert edges between tarjan calls.
    for (int pass = 0; pass < 3; ++pass) {
      int root = order[pass == 0 ? 0 : pass == 1 ? n - 1 : n / 2];
      if (pass == 2) for (int v = 0; v < n; ++v) {
        g[root].pb(v); dt.add_edge(root + 1, v + 1);
      }
      dt.tarjan(root + 1); vector<int> parent(n, -1);
      auto seen = reach(g, root, -1);
      for (int u = 1; u <= n; ++u) for (int v : dt.tree[u]) {
        require(1 <= v && v <= n && parent[v - 1] == -1,
                "dominator unique valid parent");
        parent[v - 1] = u - 1;
      }
      for (int v = 0; v < n; ++v) {
        require(bool(dt.dfn[v + 1]) == bool(seen[v]),
                "dominator reachable filtering");
        if (seen[v] && v != root)
          require(dt.id[dt.idom[dt.dfn[v + 1]]] == parent[v] + 1,
                  "dominator DFS-index/vertex-index mapping");
        else require(parent[v] == -1, "unreachable/root has no parent");
      }
      for (int x = 0; x < n; ++x) {
        auto without = reach(g, root, x);
        for (int v = 0; v < n; ++v) {
          bool ancestor = false;
          if (seen[v]) for (int u = v, steps = 0; u != -1; u = parent[u]) {
            require(++steps <= n, "dominator tree acyclic");
            ancestor |= u == x;
          }
          require(ancestor == bool(seen[v] && !without[v]),
                  "dominator deletion oracle after root/edge changes");
        }
      }
    }
  }
}

void mo_audit() {
  auto unused = [](auto...) { require(false, "empty Mo callback"); };
  mo::solve({}, unused, unused, unused, unused, unused);
  mt19937 rng(140204);
  for (int n : {1, 2, 7, 32}) for (int block : {1, 3, INT_MAX}) {
    vector<int> initial(n), a, pos(97), before(97), after(97);
    for (int &x : initial) x = rng() % 5;
    a = initial;
    for (int t = 1; t <= 96; ++t) {
      pos[t] = t % 3 ? 0 : rng() % n;
      before[t] = a[pos[t]], after[t] = t % 4 ? rng() % 5 : before[t];
      a[pos[t]] = after[t];
    }
    vector<mo::Query> qs;
    for (int i = 0; i < 1000; ++i) {
      int l = rng() % n, r = rng() % n; if (l > r) swap(l, r);
      qs.emplace_back(l, r, rng() % 97, block, qs.size());
      if (i % 11 == 0) {
        auto copy = qs.back(); copy.id = qs.size(); qs.pb(copy);
      }
    }
    shuffle(qs.begin(), qs.end(), rng);
    a = initial; vector<int> active(n), counts(5), seen(qs.size());
    int time = 0;
    auto change = [&](int t, int l, int r, bool forward) {
      require(t == time + forward, "Mo time sequencing");
      for (int p = 0; p < n; ++p)
        require(active[p] == (l <= p && p <= r), "Mo current range");
      int p = pos[t], next = forward ? after[t] : before[t];
      require(a[p] == (forward ? before[t] : after[t]),
              "Mo overwritten value before apply/undo");
      if (active[p]) --counts[a[p]], ++counts[next];
      a[p] = next; time += forward ? 1 : -1;
    };
    mo::solve(std::move(qs),
      [&](int t, int l, int r) { change(t, l, r, true); },
      [&](int t, int l, int r) { change(t, l, r, false); },
      [&](int p) {
        require(!active.at(p), "Mo add inactive position");
        active[p] = 1, ++counts[a[p]];
      },
      [&](int p) {
        require(active.at(p), "Mo remove active position");
        active[p] = 0, --counts[a[p]];
      },
      [&](const mo::Query &q) {
        vector<int> want = initial, freq(5);
        for (int t = 1; t <= q.T; ++t) want[pos[t]] = after[t];
        for (int p = q.L; p <= q.R; ++p) ++freq[want[p]];
        require(time == q.T && a == want && counts == freq,
                "Mo nonlinear frequency state/no-op updates");
        require(++seen.at(q.id) == 1, "Mo answers every duplicate once");
      });
    require(all_of(seen.begin(), seen.end(), [](int x) { return x == 1; }),
            "Mo all IDs answered");
  }
}

void binomial_audit() {
  // Pascal addition at a prime near LLONG_MAX stresses both wide products.
  for (ll p : {2LL, 3LL, 97LL, 9223372036854775783LL}) {
    int N = min(512LL, p - 1); binomial::ModBinomial b(N, p);
    vector<ll> row(N + 1); row[0] = 1;
    for (int n = 0; n <= N; ++n) {
      for (int k = 0; k <= n; ++k)
        require(b.C(n, k) == row[k], "C near maximum ll prime vs Pascal");
      for (int k = min(n + 1, N); k; --k)
        row[k] = (i128(row[k]) + row[k - 1]) % p;
    }
    require(b.C(INT_MIN, INT_MAX) == 0 && b.C(INT_MAX, INT_MIN) == 0,
            "C invalid inputs before table indexing");
  }
  for (int p : {2, 3, 5, 7, 31, 97}) {
    binomial::ModBinomial b(p - 1, p);
    // Dense middle k, independently computed beyond multiple prime powers.
    vector<ll> row(1025); row[0] = 1;
    for (int n = 0; n <= 1024; ++n) {
      for (int k = 0; k <= n; ++k)
        require(b.lucas(n, k) == row[k], "Lucas carries vs Pascal");
      for (int k = min(n + 1, 1024); k; --k)
        row[k] = (row[k] + row[k - 1]) % p;
    }
    // p^d-1 has every digit p-1: C(n,k) = (-1)^k for odd p.
    ll power = p;
    while (power <= LLONG_MAX / p) power *= p;
    ll n = power - 1;
    for (ll k : {0LL, 1LL, n / 2, n - 1, n}) {
      ll want = p == 2 || !(k & 1) ? 1 : p - 1;
      require(b.lucas(n, k) == want, "Lucas full-width dense k identity");
    }
    require(b.lucas(LLONG_MIN, 0) == 0 &&
            b.lucas(LLONG_MAX, LLONG_MIN) == 0,
            "Lucas invalid full-width inputs");
  }
}

void rounding_audit() {
  vector<int> v{INT_MIN, INT_MIN + 1, -65536, -3, -2, -1, 0,
                1, 2, 3, 65536, INT_MAX - 1, INT_MAX};
  for (int a : v) for (int b : v) {
    if (!b || (a == INT_MIN && b == -1)) continue;
    ll f = rounding::floor(a, b), c = rounding::ceil(a, b);
    if (b > 0)
      require(f * b <= a && (f + 1) * b > a &&
              c * b >= a && (c - 1) * b < a, "floor/ceil positive b");
    else
      require(f * b >= a && (f + 1) * b < a &&
              c * b <= a && (c - 1) * b > a, "floor/ceil negative b");
  }
}

struct FloorInput {
  int value, left = 0, quotient = 0, count = 0;
  bool pending = false;
  friend bool operator<=(int l, FloorInput &n) {
    if (n.pending) {
      int r = l - 1;
      require(n.left <= r && n.quotient == n.value / n.left &&
              n.quotient == n.value / r, "floor enumeration endpoints");
      require(r == n.value || n.value / (r + 1) != n.quotient,
              "floor enumeration maximal right endpoint");
      ++n.count;
    }
    n.left = l; n.pending = false;
    return l <= n.value;
  }
  friend int operator/(FloorInput &n, int d) {
    int x = n.value / d;
    if (!n.pending) n.quotient = x, n.pending = true;
    return x;
  }
};
void floor_enumeration_audit() {
  auto check = [](int value) {
    FloorInput n{value};
#include "../../codebook/6_Math/floor_enumeration.cpp"
    ll s = sqrt(value);
    while ((s + 1) * (s + 1) <= value) ++s;
    while (s * s > value) --s;
    ll count = value ? 2 * s - (value < s * (s + 1)) : 0;
    require(!n.pending && n.left == value + 1 && n.count == count,
            "floor enumeration full coverage/distinct-count identity");
  };
  check(0);
  // Sweep both transition families systematically beyond existing samples.
  for (int k = 1; k <= 1024; ++k) for (int delta : {-1, 0, 1}) {
    check(k * k + delta); check(k * (k + 1) + delta);
  }
  for (int k : {32767, 46339, 46340}) for (int delta : {-1, 0, 1}) {
    check(k * k + delta); check(k * (k + 1) + delta);
  }
  check(INT_MAX - 1);
}

int main() {
  determinant_audit(); cover_audit(); rational_audit(); directed_audit();
  mo_audit(); binomial_audit(); rounding_audit();
  floor_enumeration_audit();
  cout << "PASS round14 g02: extreme arithmetic, graph reuse, Mo rollback"
       << " and floor enumeration\n";
}
