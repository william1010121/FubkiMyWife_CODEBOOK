#include <bits/stdc++.h>
using namespace std;
#define RANGE_CHMIN_CHMAX_ADD_RANGE_SUM_NO_MAIN
#include "../../codebook/3_Data_Structure/Range_Chmin_Chmax_Add_Range_Sum.cpp"

namespace {

struct Op {
  int type, l, r;  // 0=min, 1=max, 2=add, 3=query
  ll x;
};

[[noreturn]] void mismatch(const string &name, int step, int l, int r,
                           ll got, ll want) {
  cerr << "beats mismatch case=" << name << " step=" << step << " range=["
       << l << ',' << r << "] got=" << got << " want=" << want << '\n';
  exit(1);
}

void check_all(const string &name, int step, const vector<ll> &ref) {
  int n = static_cast<int>(ref.size()) - 1;
  for (int l = 1; l <= n; ++l) {
    ll want = 0;
    for (int r = l; r <= n; ++r) {
      want += ref[r];
      ll got = query(l, r, 1, n, 1);
      if (got != want) mismatch(name, step, l, r, got, want);
    }
  }
}

void run_case(const string &name, const vector<ll> &initial,
              const vector<Op> &ops) {
  int n = static_cast<int>(initial.size()) - 1;
  vector<ll> ref = initial;
  for (int i = 1; i <= n; ++i) a[i] = ref[i];
  build(1, n, 1);
  check_all(name, -1, ref);

  for (int step = 0; step < static_cast<int>(ops.size()); ++step) {
    Op op = ops[step];
    if (op.type == 0) {
      modifymin(op.l, op.r, 1, n, 1, op.x);
      for (int i = op.l; i <= op.r; ++i) ref[i] = min(ref[i], op.x);
    } else if (op.type == 1) {
      modifymax(op.l, op.r, 1, n, 1, op.x);
      for (int i = op.l; i <= op.r; ++i) ref[i] = max(ref[i], op.x);
    } else if (op.type == 2) {
      modifyadd(op.l, op.r, 1, n, 1, op.x);
      for (int i = op.l; i <= op.r; ++i) ref[i] += op.x;
    } else {
      ll want = 0;
      for (int i = op.l; i <= op.r; ++i) want += ref[i];
      ll got = query(op.l, op.r, 1, n, 1);
      if (got != want) mismatch(name, step, op.l, op.r, got, want);
    }
    check_all(name, step, ref);
  }
}

vector<Op> edge_ops(int n) {
  vector<Op> ops;
  auto add = [&](int type, int l, int r, ll x = 0) {
    ops.push_back({type, l, r, x});
  };
  add(0, 1, n, 100);   // no-op chmin
  add(1, 1, n, -100);  // no-op chmax
  add(2, 1, n, 0);     // zero add
  for (int l = 1; l <= n; ++l)
    for (int r = l; r <= n; ++r) {
      add(0, l, r, 0);
      add(1, l, r, 0);
      add(2, l, r, 3);
      add(2, l, r, -5);
      add(3, l, r);
      add(0, l, r, -7);
      add(1, l, r, 11);
    }
  add(0, 1, n, -1000);
  add(1, 1, n, 1000);
  add(3, 1, n);
  return ops;
}

void exhaustive_small() {
  for (int n = 1; n <= 6; ++n) {
    int cases = 1;
    for (int i = 0; i < n; ++i) cases *= 3;
    for (int code = 0; code < cases; ++code) {
      vector<ll> initial(n + 1);
      int z = code;
      for (int i = 1; i <= n; ++i) {
        initial[i] = (z % 3) * 2 - 2;
        z /= 3;
      }
      vector<Op> ops;
      for (int l = 1; l <= n; ++l)
        for (int r = l; r <= n; ++r)
          for (ll x : {-3LL, 0LL, 3LL}) {
            ops.push_back({0, l, r, x});
            ops.push_back({1, l, r, x});
            ops.push_back({2, l, r, x});
            ops.push_back({3, l, r, 0});
          }
      run_case("exhaustive-n" + to_string(n) + "-case" + to_string(code),
               initial, ops);
    }
  }
}

}  // namespace

int main() {
  run_case("singleton", {0, 0}, edge_ops(1));
  run_case("all-equal", vector<ll>(9, 7), edge_ops(8));
  run_case("duplicate-extrema", {0, 5, 5, 5, -3, -3, 10, 10, 10},
           edge_ops(8));
  run_case("alternating", {0, -20, 20, -20, 20, -20, 20, -20}, edge_ops(7));
  run_case("monotone", {0, -30, -20, -10, 0, 10, 20, 30}, edge_ops(7));

  vector<ll> extreme = {0, -1000000000000000LL, 1000000000000000LL,
                        -999999999999999LL, 999999999999999LL};
  run_case("large-values", extreme,
           {{2, 1, 4, 100000000000000LL},
            {0, 1, 3, -500000000000000LL},
            {1, 2, 4, 700000000000000LL},
            {3, 1, 4, 0},
            {2, 2, 2, -300000000000000LL},
            {3, 2, 2, 0}});

  exhaustive_small();

  mt19937_64 rng(0x11BEA75);
  const ll choices[] = {-100, -20, -3, 0, 3, 20, 100};
  for (int tc = 0; tc < 220; ++tc) {
    int n = 1 + rng() % 35;
    vector<ll> initial(n + 1);
    for (int i = 1; i <= n; ++i) initial[i] = choices[rng() % 7];
    vector<Op> ops;
    for (int q = 0; q < 260; ++q) {
      int l = 1 + rng() % n, r = 1 + rng() % n;
      if (l > r) swap(l, r);
      int type = rng() % 4;
      ll x = choices[rng() % 7];
      ops.push_back({type, l, r, x});
    }
    run_case("random-" + to_string(tc), initial, ops);
  }
  cout << "range chmin/chmax/add/sum edge cases: PASS\n";
}
