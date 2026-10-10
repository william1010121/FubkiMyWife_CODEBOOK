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

static void large_frequencies() {
  mt19937 rng(0xB175);
  for (int tc = 0; tc < 1000; ++tc) {
    vector<int> freq(N + 1);
    int left = INT_MAX;
    for (int i = 1; i < N; ++i) {
      freq[i] = tc == 0 ? 0 : int(rng() % (unsigned(left) + 1));
      left -= freq[i];
    }
    freq[N] = left; // Exact total INT_MAX, including long zero runs.
    fill(begin(bit), end(bit), 0);
    long long prefix = 0;
    for (int i = 1; i <= N; ++i) {
      for (int j = i; j <= N; j += j & -j) bit[j] += freq[i];
    }
    for (int i = 1; i <= N; ++i) {
      if (freq[i]) for (long long k :
          {prefix + 1, prefix + freq[i], prefix + 1 + freq[i] / 2})
        if (query_kth(int(k)) != i)
          throw runtime_error("BIT INT_MAX rank boundaries");
      prefix += freq[i];
    }
  }
}
static void incremental_updates() {
  mt19937 rng(0xB178);
  vector<int> freq(N + 1);
  fill(begin(bit), end(bit), 0);
  int total = 0;
  for (int step = 0; step < 20000; ++step) {
    int index = 1 + rng() % N;
    int value = step % 3 ? rng() % 1000000 : 0;
    int delta = value - freq[index];
    freq[index] = value, total += delta;
    for (int j = index; j <= N; j += j & -j) bit[j] += delta;
    if (!total) continue; // Empty multiset has no valid rank query.
    for (int k : {1, total, int(1 + rng() % total)}) {
      int prefix = 0, want = 0;
      while (prefix < k) prefix += freq[++want];
      if (query_kth(k) != want)
        throw runtime_error("BIT mixed positive/negative updates");
    }
  }
}
int main() {
  incremental_updates();
  large_frequencies();
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
