#include <bits/stdc++.h>
using namespace std;

#define MAXN 128
#include "../../codebook/3_Data_Structure/Sparse_table.cpp"

[[noreturn]] static void fail(int tc, int l, int r, int got, int want) {
  cerr << "Sparse_table mismatch: tc=" << tc << " range=[" << l << ',' << r
       << "] got=" << got << " want=" << want << '\n';
  exit(1);
}

static void check_all(Sparse_table &st, const vector<int> &a, int tc) {
  const int n = (int)a.size();
  for (int l = 0; l < n; ++l) {
    int want = numeric_limits<int>::min();
    for (int r = l; r < n; ++r) {
      want = max(want, a[r]);
      int got = st.query(l, r);
      if (got != want) fail(tc, l, r, got, want);
    }
  }
}

static void run_case(const vector<int> &a, int tc) {
  int data[MAXN] = {};
  copy(a.begin(), a.end(), data);
  Sparse_table st;
  st.init((int)a.size(), data);
  check_all(st, a, tc);  // includes singleton, full, duplicate, and negative ranges
}

static void enumerate(vector<int> &a, int at, int &tc) {
  if (at == (int)a.size()) {
    run_case(a, tc++);
    return;
  }
  for (int x : {-4, -1, 0, 3}) {
    a[at] = x;
    enumerate(a, at + 1, tc);
  }
}

int main() {
  int tc = 0;
  for (int n = 1; n <= 6; ++n) {
    vector<int> a(n);
    enumerate(a, 0, tc);
  }
  run_case({numeric_limits<int>::min(), numeric_limits<int>::min(),
            numeric_limits<int>::max()}, tc++);

  mt19937 rng(0x5A9E);
  for (int test = 0; test < 500; ++test) {
    int n = 1 + (int)(rng() % 96);
    vector<int> a(n);
    for (int &x : a) x = (int)(rng() % 4001) - 2000;
    run_case(a, tc++);
  }
  return 0;
}
