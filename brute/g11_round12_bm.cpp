#include <bits/stdc++.h>
using namespace std;
#define SZ(a) ((int)(a).size())
struct F2 {
  unsigned char v;
  F2(int x = 0) : v(x & 1) {}
  F2 &operator+=(F2 b) { v ^= b.v; return *this; }
  F2 operator-() const { return *this; }
  friend F2 operator*(F2 a, F2 b) { return a.v & b.v; }
  friend F2 operator/(F2 a, F2 b) { assert(b.v); return a; }
  friend bool operator==(F2 a, F2 b) { return a.v == b.v; }
};
#include "../codebook/6_Math/Berlekamp-Massey.cpp"

// The original interface needs assignment, not a copy constructor.
struct AssignOnly {
  int v;
  AssignOnly(int x = 0) : v(x & 1) {}
  AssignOnly(AssignOnly &&) = default;
  AssignOnly(const AssignOnly &) = delete;
  AssignOnly &operator=(const AssignOnly &) = default;
  AssignOnly &operator+=(const AssignOnly &b) { v ^= b.v; return *this; }
  AssignOnly operator-() const { return AssignOnly(v); }
  friend AssignOnly operator*(const AssignOnly &a, const AssignOnly &b) {
    return AssignOnly(a.v & b.v);
  }
  friend AssignOnly operator/(const AssignOnly &a, const AssignOnly &b) {
    assert(b.v); return AssignOnly(a.v);
  }
  friend bool operator==(const AssignOnly &a, const AssignOnly &b) {
    return a.v == b.v;
  }
};

// Independent packed dot products check every recurrence equation.
void check(const vector<F2> &a, int lo, int hi) {
  auto c = BerlekampMassey(a);
  int l = SZ(c), words = (l + 63) / 64;
  assert(lo <= l && l <= hi);
  vector<uint64_t> coef(words), window(words);
  for (int j = 0; j < l; ++j) {
    coef[j / 64] |= uint64_t(c[j].v) << (j % 64);
    window[j / 64] |= uint64_t(a[l - j - 1].v) << (j % 64);
  }
  for (int i = l; i < SZ(a); ++i) {
    int parity = 0;
    for (int j = 0; j < words; ++j)
      parity ^= popcount(window[j] & coef[j]) & 1;
    assert(parity == a[i].v);
    uint64_t carry = a[i].v;
    for (auto &word : window) {
      uint64_t next = word >> 63;
      word = (word << 1) | carry; carry = next;
    }
  }
}
int main() {
  mt19937 rng(1211);
  vector<F2> random(32768);
  for (auto &x : random) x = rng() & 1;
  check(random, 16381, 16387);
  vector<F2> periodic(200000);
  periodic[0] = periodic[1] = 1;
  for (int i = 2; i < SZ(periodic); ++i)
    periodic[i] = periodic[i - 1], periodic[i] += periodic[i - 2];
  check(periodic, 2, 2);
  vector<F2> impulse(4096); impulse.back() = 1;
  assert(SZ(BerlekampMassey(impulse)) == SZ(impulse));
  check(impulse, SZ(impulse), SZ(impulse));
  vector<AssignOnly> a(64); a[0].v = a[1].v = 1;
  for (int i = 2; i < SZ(a); ++i) a[i].v = a[i-1].v ^ a[i-2].v;
  auto c = BerlekampMassey(a);
  assert(SZ(c) == 2 && c[0].v == 1 && c[1].v == 1);
  cout << "round12 BM: dense F2, low order and delayed pivot PASS\n";
}
