#include "../../codebook/1_Basic/Default_code.cpp"
// Count allocations only inside BM; the field itself never allocates.
static bool measuring;
static size_t allocations;
void *operator new(size_t n) {
  if (measuring) ++allocations;
  if (void *p = malloc(max(n, size_t(1)))) return p;
  throw bad_alloc();
}
void operator delete(void *p) noexcept { free(p); }
void operator delete(void *p, size_t) noexcept { free(p); }
struct F2 {
  unsigned char v;
  F2(int x = 0) : v(x & 1) {}
  F2 &operator+=(F2 b) { v ^= b.v; return *this; }
  F2 operator-() const { return *this; }
  friend F2 operator*(F2 a, F2 b) { return a.v & b.v; }
  friend F2 operator/(F2 a, F2 b) { assert(b.v); return a; }
  friend bool operator==(F2 a, F2 b) { return a.v == b.v; }
};
#include "../../codebook/6_Math/Berlekamp-Massey.cpp"
// Independent packed dot products verify every recurrence equation.
void check(const vector<F2> &a) {
  allocations = 0; measuring = true;
  auto c = BerlekampMassey(a);
  measuring = false;
  int l = SZ(c), words = (l + 63) / 64;
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
  cout << "n=" << SZ(a) << " order=" << l
       << " allocations=" << allocations << '\n';
}
int main() {
  mt19937 rng(1607);
  for (int n : {512, 32768}) {
    vector<F2> a(n);
    for (auto &x : a) x = rng() & 1;
    check(a);
  }
  vector<F2> a(200000); a[0] = a[1] = 1;
  for (int i = 2; i < SZ(a); ++i)
    a[i] = a[i - 1], a[i] += a[i - 2];
  check(a);
  cout << "PASS dense/low-order BM with allocation counts\n";
}
