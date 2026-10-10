// O(m+(n+D)ceil(m/64)) expected time; D distinct values in b.
int bitset_lcs(const vector<int> &a, const vector<int> &b) {
  if (a.empty() || b.empty()) return 0;
  int m = (int)b.size(), W = (m + 63) >> 6;
  unordered_map<int, vector<unsigned long long>> mask;
  for (int j = 0; j < m; ++j) {
    auto &v = mask[b[j]];
    v.resize(W), v[j >> 6] |= 1ULL << (j & 63);
  }
  vector<unsigned long long> f(W);
  for (int value : a) {
    auto it = mask.find(value);
    if (it == mask.end()) continue;
    unsigned long long carry = 1, borrow = 0;
    for (int i = 0; i < W; ++i) {
      auto x = f[i] | it->second[i], y = (f[i] << 1) | carry;
      carry = f[i] >> 63;
      auto rhs = y + borrow;
      borrow = (rhs < y) || (x < rhs);
      f[i] = x & ~(x - rhs);
    }
  } // Padding bits stay zero: every new bit is a subset of x.
  int ans = 0;
  for (auto word : f) ans += popcount(word);
  return ans;
}
