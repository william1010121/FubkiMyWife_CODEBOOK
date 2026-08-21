#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#define RANGE_CHMIN_CHMAX_ADD_RANGE_SUM_NO_MAIN
#include "../../codebook/3_Data_Structure/Range_Chmin_Chmax_Add_Range_Sum.cpp"

struct Op {
  int type, l, r;
  ll x;
};

static ll scan_sum(const vector<ll> &v, int l, int r) {
  ll ans = 0;
  for (int i = l; i <= r; ++i) ans += v[i];
  return ans;
}

static void check_all(const vector<ll> &ref, int n,
                      const string &name, int step) {
  for (int l = 1; l <= n; ++l) {
    for (int r = l; r <= n; ++r) {
      ll got = query(l, r, 1, n, 1);
      ll want = scan_sum(ref, l, r);
      if (got != want) {
        cerr << "range-beats mismatch case=" << name
             << " step=" << step << " interval=[" << l
             << ',' << r << "] got=" << got
             << " want=" << want << '\n';
        exit(1);
      }
    }
  }
}

static void run_ops(const vector<ll> &initial,
                    const vector<Op> &ops, const string &name) {
  int n = (int)initial.size() - 1;
  vector<ll> ref = initial;
  for (int i = 1; i <= n; ++i) a[i] = ref[i];
  build(1, n, 1);
  check_all(ref, n, name, -1);
  for (size_t step = 0; step < ops.size(); ++step) {
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
      ll got = query(op.l, op.r, 1, n, 1);
      ll want = scan_sum(ref, op.l, op.r);
      if (got != want) {
        cerr << "range-beats explicit query mismatch case=" << name
             << " step=" << step << '\n';
        exit(1);
      }
    }
    check_all(ref, n, name, (int)step);
  }
}

static Op op(int type, int l, int r, ll x = 0) {
  return {type, l, r, x};
}

int main() {
  run_ops({0, 0}, {
      op(0, 1, 1, 0), op(1, 1, 1, 0), op(2, 1, 1, 5),
      op(0, 1, 1, 3), op(1, 1, 1, 9), op(3, 1, 1)},
      "single-element");

  run_ops({0, -5, 5}, {
      op(0, 1, 2, 5), op(1, 1, 2, -5), op(2, 1, 2, 10),
      op(0, 1, 1, 3), op(1, 2, 2, 20), op(3, 1, 2)},
      "two-element-equal-extrema");

  run_ops({0, 7, 7, 7, 7, 7, 7, 7, 7}, {
      op(0, 1, 8, 7), op(1, 1, 8, 7), op(0, 2, 7, 6),
      op(1, 3, 6, 10), op(2, 1, 8, -4), op(0, 1, 4, 2),
      op(1, 5, 8, 20), op(3, 1, 8), op(0, 1, 8, -100),
      op(1, 1, 8, 100), op(2, 2, 7, 11), op(3, 2, 7)},
      "all-equal-and-full-ranges");

  run_ops({0, -1000000000000LL, 5, 5, 17, -3, 17, 5,
               -1000000000000LL, 1000000000000LL},
      {
          op(0, 1, 9, 5), op(1, 2, 8, 5), op(2, 1, 9, 1000000000000LL),
          op(0, 3, 7, 1000000000000LL),
          op(1, 1, 9, -1000000000000LL),
          op(2, 4, 6, -2000000000000LL), op(0, 1, 1, -3000000000000LL),
          op(1, 9, 9, 3000000000000LL), op(3, 1, 9),
          op(0, 2, 8, -3000000000000LL),
          op(1, 2, 8, 3000000000000LL), op(3, 3, 7)},
      "large-values-and-boundaries");

  mt19937_64 rng(0x7b4e9a21ULL);
  for (int tc = 0; tc < 140; ++tc) {
    int n = 1 + (int)(rng() % 36);
    vector<ll> initial(n + 1);
    for (int i = 1; i <= n; ++i) {
      if (i % 5 == 0) initial[i] = 0;
      else initial[i] = (ll)(rng() % 2001) - 1000;
    }
    vector<Op> ops;
    for (int q = 0; q < 300; ++q) {
      int l = 1 + (int)(rng() % n), r = 1 + (int)(rng() % n);
      if (l > r) swap(l, r);
      int type = (int)(rng() % 4);
      ll x;
      if (q % 17 == 0) x = 0;
      else if (q % 19 == 0) x = 1000000000000LL;
      else if (q % 23 == 0) x = -1000000000000LL;
      else x = (ll)(rng() % 4001) - 2000;
      ops.push_back(op(type, l, r, x));
    }
    run_ops(initial, ops, "seeded-random-" + to_string(tc));
  }
  cout << "range_beats_edges: PASS\n";
}
