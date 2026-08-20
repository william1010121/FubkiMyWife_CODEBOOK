#include <bits/stdc++.h>
using namespace std;
#define MAXN 128
#include "../../../codebook/3_Data_Structure/Sparse_table.cpp"

int main() {
  mt19937 rng(16);
  for (int tc = 0; tc < 1000; ++tc) {
    int n = 1 + rng() % 100;
    int a[MAXN];
    for (int i = 0; i < n; ++i) a[i] = (int)(rng() % 201) - 100;
    Sparse_table st;
    st.init(n, a);
    for (int q = 0; q < 300; ++q) {
      int l = rng() % n, r = rng() % n;
      if (l > r) swap(l, r);
      int want = *max_element(a + l, a + r + 1), got = st.query(l, r);
      if (got != want) {
        cerr << "sparse mismatch tc=" << tc << " n=" << n << " l=" << l
             << " r=" << r << " got=" << got << " want=" << want << '\n';
        return 1;
      }
    }
  }
}
