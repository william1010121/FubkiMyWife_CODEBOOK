#include <bits/stdc++.h>
using namespace std;
#include "../../../codebook/3_Data_Structure/Segment_Tree.cpp"

int main() {
  mt19937 rng(14);
  for (int tc = 0; tc < 500; ++tc) {
    int n = 1 + rng() % 50;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) a[i] = (int)(rng() % 101) - 50;
    Segment_Tree st;
    st.init(1, n, a.data());
    for (int q = 0; q < 300; ++q) {
      int l = 1 + rng() % n, r = 1 + rng() % n;
      if (l > r) swap(l, r);
      if (rng() & 1) {
        int x = (int)(rng() % 31) - 15;
        st.modify(l, r, x);
        for (int i = l; i <= r; ++i) a[i] += x;
      } else {
        int want = *max_element(a.begin() + l, a.begin() + r + 1);
        assert(st.query(l, r) == want);
      }
    }
  }
}
