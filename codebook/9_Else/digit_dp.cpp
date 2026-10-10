// 32-bit int, C++20; count [0,x] without adjacent 1 bits; x<0 -> 0.
int calc(int x) { // O(log x) time, O(1) space.
  if (x < 0) return 0;
  static constexpr auto f = [] { array<int, 32> f{1, 2};
    for (int i = 2; i < 32; ++i) f[i] = f[i-1] + f[i-2];
    return f; }(); int ans = 0;
  for (int i = bit_width(unsigned(x)); --i >= 0;) if (x >> i & 1) {
    ans += f[i]; if ((x >> i & 3) == 3) return ans; // Adjacent ones.
  } return ans + 1;
}
