#include <bits/stdc++.h>
using namespace std;
constexpr int N = 13;
#include "../../codebook/3_Data_Structure/BIT_kth.cpp"

static void check(const vector<int> &freq) {
  fill(begin(bit), end(bit), 0);
  vector<int> values;
  for (int i = 1; i <= N; ++i) {
    for (int j = i; j <= N; j += j & -j) bit[j] += freq[i];
    for (int j = 0; j < freq[i]; ++j) values.push_back(i);
  }
  for (int k = 1; k <= (int)values.size(); ++k)
    if (query_kth(k) != values[k - 1])
      throw runtime_error("non-power-of-two BIT rank");
}
int main() {
  vector<int> freq(N + 1);
  for (int i = 1; i <= N; ++i) {
    fill(freq.begin(), freq.end(), 0);
    freq[i] = 2;
    check(freq);
  }
  mt19937 rng(721);
  for (int tc = 0; tc < 1000; ++tc) {
    for (int i = 1; i <= N; ++i) freq[i] = rng() % 8;
    check(freq);
  }
  cout << "PASS arbitrary BIT N=" << N << '\n';
}
