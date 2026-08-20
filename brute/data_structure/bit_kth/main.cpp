#include <bits/stdc++.h>
using namespace std;
#define N 32
#include "../../../codebook/3_Data_Structure/BIT_kth.cpp"

int main() {
  mt19937 rng(2);
  for (int tc = 0; tc < 1000; ++tc) {
    fill(bit, bit + N + 1, 0);
    vector<int> freq(N + 1);
    for (int i = 1; i <= N; ++i) {
      freq[i] = rng() % 4;
      for (int j = i; j <= N; j += j & -j) bit[j] += freq[i];
    }
    int total = accumulate(freq.begin(), freq.end(), 0);
    for (int k = 1; k <= total; ++k) {
      int need = k, want = 1;
      while (need > freq[want]) need -= freq[want++];
      assert(query_kth(k) == want);
    }
  }
}
