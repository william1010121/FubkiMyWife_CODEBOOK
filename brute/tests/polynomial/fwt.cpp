#include <bits/stdc++.h>
using namespace std;
#include "codebook/7_Polynomial/Fast_Walsh_Transform.cpp"

int main() {
  mt19937 rng(0xf17);
  for (int L = 0; L <= 7; ++L) {
    int n = 1 << L;
    vector<int> a(n), b(n);
    for (int &x : a) x = rng() % 11;
    for (int &x : b) x = rng() % 11;
    vector<int> want(n);
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j) want[i | j] += a[i] * b[j];
    fwt(a.data(), n, 1); fwt(b.data(), n, 1);
    for (int i = 0; i < n; ++i) a[i] *= b[i];
    fwt(a.data(), n, -1);
    if (a != want) { cerr << "FWT OR mismatch\n"; return 1; }
  }
  for (int L = 0; L <= 6; ++L) {
    int n = 1 << L;
    vector<int> a(n), b(n), got(n), want(n);
    for (int &x : a) x = rng() % 13;
    for (int &x : b) x = rng() % 13;
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
        if (!(i & j)) want[i | j] += a[i] * b[j];
    subset_convolution(a.data(), b.data(), got.data(), L);
    if (got != want) { cerr << "subset convolution mismatch\n"; return 1; }
  }
  cout << "fwt: PASS\n";
}
