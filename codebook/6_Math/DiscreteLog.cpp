int DiscreteLog(int s, int x, int y, int m) {
	int kStep = sqrt(m);
	if (1LL * kStep * kStep < m) ++kStep;
	unordered_map<int, int> p;
	int b = 1; for (int i = 0; i < kStep; ++i) {
		p[y] = i, y = 1LL * y * x % m, b = 1LL * b * x % m;
  } for (long long i = 0; i < 1LL * m + 10; i += kStep) {
		s = 1LL * s * b % m;
		if (p.find(s) != p.end()) {
			long long ans = i + kStep - p[s];
			return ans <= INT_MAX ? (int)ans : -1;
		}
  } return -1;
}
int DiscreteLog(int x, int y, int m) {
	if (m == 1) return 0; int s = 1;
	for (int i = 0; i < 100; ++i) { if (s == y) return i;
	  s = 1LL * s * x % m; } if (s == y) return 100;
	int tail = DiscreteLog(s, x, y, m);
	if (tail < 0) return -1;
	long long ans = 100LL + tail;
	if (ans > INT_MAX || fpow(x, (int)ans, m) != y) return -1;
  return (int)ans;
}
