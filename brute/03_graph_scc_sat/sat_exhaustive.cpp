#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#include "../../codebook/2_Graph/SCC.cpp"
#include "../../codebook/2_Graph/2SAT.cpp"

using Clause = pair<int, int>;

static bool value_of(int literal, int mask, int n) {
  bool value = (mask >> (literal % n)) & 1;
  return literal < n ? value : !value;
}

static bool oracle(int n, const vector<Clause> &formula) {
  if (n == 0) return formula.empty();
  for (int mask = 0; mask < (1 << n); ++mask) {
    bool ok = true;
    for (auto [a, b] : formula)
      ok &= value_of(a, mask, n) || value_of(b, mask, n);
    if (ok) return true;
  }
  return false;
}

static void fail(int tc, int n, const vector<Clause> &formula, const string &why) {
  cerr << "2SAT exhaustive failure tc=" << tc << ": " << why << '\n';
  cerr << "n=" << n << " m=" << formula.size() << '\n';
  for (auto [a, b] : formula) cerr << a << ' ' << b << '\n';
  exit(1);
}

static void check(int tc, int n, const vector<Clause> &formula) {
  SAT sat(n);
  for (auto [a, b] : formula) sat.add_clause(a, b);
  bool want = oracle(n, formula);
  bool got = sat.solve();
  if (got != want) fail(tc, n, formula, "satisfiability differs");
  if (!got) return;
  for (int i = 0; i < n; ++i)
    if (sat.istrue[i + n] == sat.istrue[i])
      fail(tc, n, formula, "returned literals are not complements");
  for (auto [a, b] : formula) {
    bool va = a < n ? sat.istrue[a] : !sat.istrue[a - n];
    bool vb = b < n ? sat.istrue[b] : !sat.istrue[b - n];
    if (!(va || vb)) fail(tc, n, formula, "witness violates a clause");
  }
}

int main() {
  int tc = 0;
  check(tc++, 0, {});

  // Every possible formula over one and two variables.  The clause universe
  // includes units (a,a), tautologies (a,!a), and all ordered literal pairs.
  for (int n = 1; n <= 2; ++n) {
    int literals = 2 * n;
    int clause_count = literals * literals;
    uint32_t limit = uint32_t(1) << clause_count;
    for (uint32_t mask = 0; mask < limit; ++mask) {
      vector<Clause> formula;
      for (int a = 0; a < literals; ++a)
        for (int b = 0; b < literals; ++b)
          if ((mask >> (a * literals + b)) & 1u) formula.push_back({a, b});
      check(tc++, n, formula);
    }
  }

  // Every ordered formula of length at most three over three variables.  This
  // explicitly exercises duplicate clauses as well as all literal signs.
  int n = 3, literals = 2 * n;
  vector<Clause> formula;
  function<void(int)> enumerate = [&](int depth) {
    check(tc++, n, formula);
    if (depth == 3) return;
    for (int a = 0; a < literals; ++a)
      for (int b = 0; b < literals; ++b) {
        formula.push_back({a, b});
        enumerate(depth + 1);
        formula.pop_back();
      }
  };
  enumerate(0);

  cout << "2SAT exhaustive PASS " << tc << " formulas\n";
}
