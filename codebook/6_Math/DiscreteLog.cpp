// m>0, residues in [0,m); expected O(sqrt(m)) time/space.
int DiscreteLog(int s, int x, int y, int m) {
  int step = sqrt(m), b = 1; if (1LL * step * step < m) ++step;
  unordered_map<int, int> p; p.reserve(step);
  for (int i = 0; i < step; ++i)
    p[y] = i, y = 1LL * y * x % m, b = 1LL * b * x % m;
  for (long long i = 0; i < 1LL * m + 10; i += step) {
    s = 1LL * s * b % m;
    if (auto it = p.find(s); it != p.end()) {
      long long ans = i + step - it->second;
      return ans <= INT_MAX ? (int)ans : -1;
    }
  } return -1;
}
int DiscreteLog(int x, int y, int m) {
  if (m == 1) return 0; int s = 1; // skip non-coprime transient
  for (int i = 0; i < 100; ++i, s = 1LL * s * x % m)
    if (s == y) return i;
  if (s == y) return 100; int tail = DiscreteLog(s, x, y, m);
  long long ans = 100LL + tail;
  return tail < 0 || ans > INT_MAX || fpow(x, (int)ans, m) != y
    ? -1 : (int)ans;
}
