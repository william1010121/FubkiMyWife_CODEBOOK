int bitset_lcs(const vector<int> &a, const vector<int> &b) {
  int m = (int)b.size(), W = (m + 63) >> 6;
  unordered_map<int, vector<unsigned long long>> mask;
  for (int j = 0; j < m; ++j)
    mask[b[j]].resize(W), mask[b[j]][j >> 6] |= 1ULL << (j & 63);
  vector<unsigned long long> f(W), x(W), y(W), diff(W);
  for (int value : a) {
    auto it = mask.find(value);
    if (it == mask.end()) continue;
    unsigned long long carry = 1;
    for (int i = 0; i < W; ++i) {
      y[i] = (f[i] << 1) | carry;
      carry = f[i] >> 63, x[i] = f[i] | it->second[i];
    }
    unsigned long long borrow = 0;
    for (int i = 0; i < W; ++i) {
      unsigned long long rhs = y[i] + borrow;
      borrow = (rhs < y[i]) || (x[i] < rhs);
      diff[i] = x[i] - rhs;
    }
    for (int i = 0; i < W; ++i) f[i] = x[i] & ~diff[i];
    if (m & 63) f.back() &= (1ULL << (m & 63)) - 1;
  }
  int ans = 0;
  for (auto word : f) ans += __builtin_popcountll(word);
  return ans;
}
