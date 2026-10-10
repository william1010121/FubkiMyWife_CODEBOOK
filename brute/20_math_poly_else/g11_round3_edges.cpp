#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define SZ(a) ((int)a.size())
#define ALL(v) v.begin(), v.end()
#define pb push_back
#include "../../codebook/9_Else/CDQ_Dominance.cpp"
namespace mo_update {
#include "../../codebook/9_Else/Mos_Algorithm_With_modification.cpp"
}
namespace mo_tree {
#include "../../codebook/9_Else/Mos_Algorithm_On_Tree.cpp"
}

void require(bool ok, const char *msg) {
  if (!ok) { cerr << msg << '\n'; exit(1); }
}
void mo() {
  // Large virtual ranges require no allocation; parity must not overflow.
  for (int l : {0, 1, INT_MAX/2, INT_MAX-2, INT_MAX-1})
    for (int r : {l, INT_MAX-1}) for (int block : {1, 2, 7}) {
      mo_update::Query a(l, r, 0, block), b(l, r, 1, block);
      bool odd = ((ll(l/block) + r/block) & 1);
      require((a < b) == !odd && (b < a) == odd && !(a < a),
              "Mo time-order parity / strict order");
    }
  // Explicit template arguments remain supported after abbreviation.
  auto time = [](int, int, int) {};
  auto position = [](int) {};
  auto answer = [](const mo_update::Query &) {};
  mo_update::solve<decltype(time), decltype(time), decltype(position),
                   decltype(position), decltype(answer)>
    ({}, time, time, position, position, answer);
  auto tree_answer = [](const mo_tree::Query &) {};
  mo_tree::solve<decltype(position), decltype(tree_answer)>
    ({}, {}, position, tree_answer);
}
void cdq() {
  require(dominance({}).empty(), "empty dominance input");
  mt19937 rng(31);
  for (int k = 0; k <= 32; ++k) {
    require(dominance({vector<int>(k, INT_MIN)}) == vector<ll>{0},
            "singleton dominance input");
    require(dominance(vector<vector<int>>(8, vector<int>(k, INT_MAX)))
              == vector<ll>(8, 7), "identical dominance rows");
  }
  for (int k = 0; k <= 6; ++k) {
    vector<vector<int>> a;
    for (int bits = 0; bits < (1 << k); ++bits) {
      vector<int> p(k);
      for (int j = 0; j < k; ++j) p[j] = bits >> j & 1;
      a.pb(p), a.pb(p);
    }
    shuffle(ALL(a), rng);
    auto got = dominance(a);
    for (int i = 0; i < SZ(a); ++i) {
      int ones = accumulate(ALL(a[i]), 0);
      require(got[i] == (2LL << ones)-1, "hypercube dominance ties");
    }
  }
  // Many empty/singleton cross lists and conflicting coordinates.
  const int n = 100000;
  vector<vector<int>> a(n, vector<int>(3));
  for (int i = 0; i < n; ++i) a[i] = {i, n-i, i};
  require(dominance(a) == vector<ll>(n), "large 3D antichain");
}
int main(int argc, char **argv) {
  string test = argc > 1 ? argv[1] : "all";
  if (test == "all" || test == "mo") mo();
  if (test == "all" || test == "cdq") cdq();
  cout << "g11 round3 edges PASS: " << test << '\n';
}
