#include <bits/stdc++.h>
using namespace std;

#define N 8
#include "../../codebook/3_Data_Structure/BIT_kth.cpp"

[[noreturn]] static void fail(const vector<int> &freq, int k, int got, int want) {
  cerr << "BIT_kth mismatch: k=" << k << " got=" << got << " want=" << want
       << " freq=";
  for (int i = 1; i <= N; ++i) cerr << ' ' << freq[i];
  cerr << '\n';
  exit(1);
}

static void check(vector<int> freq) {
  fill(bit, bit + N + 1, 0);
  for (int i = 1; i <= N; ++i)
    for (int j = i; j <= N; j += j & -j) bit[j] += freq[i];

  int total = accumulate(freq.begin(), freq.end(), 0);
  for (int k = 1; k <= total; ++k) {
    int want = 1;
    int left = k;
    while (left > freq[want]) left -= freq[want++];
    int got = query_kth(k);
    if (got != want) fail(freq, k, got, want);
  }
}

static void enumerate(vector<int> &freq, int at) {
  if (at > N) {
    check(freq);
    return;
  }
  // 0 includes the empty multiset; 1/2 exercise duplicate coordinates.
  for (int x = 0; x <= 2; ++x) {
    freq[at] = x;
    enumerate(freq, at + 1);
  }
}

int main() {
  vector<int> freq(N + 1);
  enumerate(freq, 1);  // 3^8 frequency vectors, every valid kth rank

  mt19937 random_source(0xB17C);
  for (int tc = 0; tc < 2000; ++tc) {
    for (int i = 1; i <= N; ++i) freq[i] = random_source() % 9;
    check(freq);
  }
  return 0;
}
