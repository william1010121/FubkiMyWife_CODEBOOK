#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#include "../../codebook/2_Graph/SCC.cpp"
#include "../../codebook/2_Graph/2SAT.cpp"

using Clause = pair<int, int>;

static bool eval_literal(int literal, int mask, int n) {
  bool value = (mask >> (literal % n)) & 1;
  return literal < n ? value : !value;
}

static bool brute_sat(int n, const vector<Clause> &formula) {
  if (n == 0) return formula.empty();
  for (int mask = 0; mask < (1 << n); ++mask) {
    bool ok = true;
    for (auto [a, b] : formula)
      if (!eval_literal(a, mask, n) && !eval_literal(b, mask, n)) {
        ok = false;
        break;
      }
    if (ok) return true;
  }
  return false;
}

static void check(int tc, int n, const vector<Clause> &formula) {
  SAT got(n);
  for (auto [a, b] : formula) got.add_clause(a, b);
  bool want = brute_sat(n, formula);
  bool have = got.solve();
  if (have != want) {
    cerr << "2SAT edge-case failure tc=" << tc
         << ": satisfiability differs n=" << n
         << " m=" << formula.size() << '\n';
    for (auto [a, b] : formula) cerr << a << ' ' << b << '\n';
    exit(1);
  }
  if (!have) return;
  for (auto [a, b] : formula) {
    bool va = a < n ? got.istrue[a] : !got.istrue[a - n];
    bool vb = b < n ? got.istrue[b] : !got.istrue[b - n];
    if (!(va || vb)) {
      cerr << "2SAT edge-case failure tc=" << tc
           << ": invalid witness\n";
      exit(1);
    }
  }
}

static int neg(int literal, int n) {
  return literal < n ? literal + n : literal - n;
}

int main() {
  int tc = 0;
  check(tc++, 0, {});
  for (int n = 1; n <= 10; ++n) {
    check(tc++, n, {});
    for (int i = 0; i < n; ++i) {
      int j = neg(i, n);
      check(tc++, n, {{i, i}});             // unit x_i
      check(tc++, n, {{j, j}});             // unit !x_i
      check(tc++, n, {{i, j}});             // tautology
      check(tc++, n, {{j, i}, {i, j}});     // duplicated tautology in reverse
      check(tc++, n, {{i, i}, {j, j}});     // direct contradiction
      check(tc++, n, {{i, i}, {i, i}, {j, j}}); // duplicate unit plus conflict
    }
  }

  mt19937 rng(0x2A7ED6);
  for (int it = 0; it < 9000; ++it) {
    int n = 1 + rng() % 10;
    int m = rng() % 90;
    vector<Clause> formula;
    for (int j = 0; j < m; ++j) {
      int a, b;
      int kind = rng() % 10;
      if (kind < 2) {
        int x = rng() % n;
        a = x; b = neg(x, n);              // tautology
      } else if (kind < 4) {
        a = rng() % (2 * n); b = a;         // unit clause
      } else if (kind == 4 && !formula.empty()) {
        tie(a, b) = formula[rng() % formula.size()]; // exact duplicate
      } else {
        a = rng() % (2 * n); b = rng() % (2 * n);
      }
      formula.push_back({a, b});
    }
    if (it % 3 == 0) {
      int x = rng() % n;
      formula.push_back({x, x});
      formula.push_back({neg(x, n), neg(x, n)});
    }
    check(tc++, n, formula);
  }

  // Generate formulas that are guaranteed satisfiable, while still mixing
  // duplicate clauses, units, and tautologies into the instance.
  for (int it = 0; it < 5000; ++it) {
    int n = 1 + rng() % 10;
    int assignment = rng() % (1 << n);
    vector<Clause> formula;
    int m = rng() % 100;
    for (int j = 0; j < m; ++j) {
      int a = rng() % (2 * n), b = rng() % (2 * n);
      if (rng() % 5 == 0) b = a;
      if (!eval_literal(a, assignment, n) && !eval_literal(b, assignment, n))
        b = (assignment >> (rng() % n) & 1) ? int(rng() % n) : n + int(rng() % n);
      formula.push_back({a, b});
      if (rng() % 8 == 0) formula.push_back({a, b});
    }
    check(tc++, n, formula);
  }

  cout << "2SAT edge-case PASS " << tc << " formulas\n";
}
