#include <bits/stdc++.h>

using namespace std;

#define BITSET_NO_MAIN
#include "../../codebook/1_Basic/bitset.cpp"

[[noreturn]] void fail(const string &message) {
  cerr << "FAIL bitset: " << message << '\n';
  exit(1);
}

void check(bool condition, const string &message) {
  if (!condition) fail(message);
}

template<size_t Width> void word_edges() {
  mt19937 rng(1704 + Width);
  bitset<Width> bits; vector<bool> want(Width);
  for (int step = 0; step < 500; ++step) {
    if constexpr (Width) {
      size_t at = rng() % Width;
      bits.flip(at); want[at] = !want[at];
    }
    size_t shift = step % 17 ? rng() % (Width + 2) : SIZE_MAX;
    bool left = rng() & 1; vector<bool> next(Width);
    for (size_t i = 0; i < Width; ++i)
      if (left ? shift <= i : shift < Width - i)
        next[i] = want[left ? i - shift : i + shift];
    bits = left ? bits << shift : bits >> shift;
    want = next;
    for (size_t i = 0; i < Width; ++i)
      check(bits[i] == want[i], "random word-boundary shift/flip");
    check(bits.count() == count(want.begin(), want.end(), true),
          "word-boundary popcount");
    check(bits == bitset<Width>(bits.to_string()), "string round trip");
  }
}

int main() {
  word_edges<0>(); word_edges<1>(); word_edges<63>(); word_edges<64>();
  word_edges<65>(); word_edges<127>(); word_edges<128>();
  word_edges<129>();
  for (unsigned mask = 0; mask < 256; ++mask) {
    bitset<8> b(mask);
    check(b.to_ulong() == mask, "constructor/to_ulong");
    check(b.count() == static_cast<size_t>(__builtin_popcount(mask)),
          "count");
    check(b.any() == (mask != 0), "any");
    check(b.none() == (mask == 0), "none");
    check(b.all() == (mask == 255), "all");

    string expected;
    for (int bit = 7; bit >= 0; --bit)
      expected += ((mask >> bit) & 1) ? '1' : '0';
    check(b.to_string() == expected, "to_string");
    string decorated;
    for (int bit = 7; bit >= 0; --bit)
      decorated += ((mask >> bit) & 1) ? 'H' : 'L';
    check(b.to_string('L', 'H') == decorated, "custom to_string");
  }

  // Check the operations against integer arithmetic, including shifts past
  // the complete width and all bitwise combinations.
  for (unsigned mask = 0; mask < 256; ++mask) {
    unsigned other = (mask * 73 + 19) & 255;
    bitset<8> a(mask), b(other);
    check((a & b).to_ulong() == (mask & other), "and");
    check((a | b).to_ulong() == (mask | other), "or");
    check((a ^ b).to_ulong() == (mask ^ other), "xor");
    check((~a).to_ulong() == ((~mask) & 255), "not");
    for (int shift = 0; shift <= 12; ++shift) {
      unsigned left = shift >= 8 ? 0 : ((mask << shift) & 255);
      unsigned right = shift >= 8 ? 0 : (mask >> shift);
      check((a << shift).to_ulong() == left, "left shift");
      check((a >> shift).to_ulong() == right, "right shift");
    }

    bitset<8> mutated = a;
    mutated.set();
    check(mutated.all(), "set all");
    mutated.reset();
    check(mutated.none(), "reset all");
    mutated.flip();
    check(mutated.to_ulong() == 255, "flip all");
    for (int bit = 0; bit < 8; ++bit) {
      bitset<8> one;
      one.set(bit);
      check(one[bit] && one.count() == 1, "set bit");
      one.reset(bit);
      check(one.none(), "reset bit");
    }
  }

  bitset<0> empty;
  check(empty.none() && !empty.any() && empty.all(), "zero-width bitset");
  empty.set().flip().reset();
  check(empty.count() == 0 && empty.to_string().empty(),
        "empty mutations");
  bitset<65> wide;
  wide.set(0).set(64);
  check(wide.count() == 2 && wide[0] && wide[64], "wide endpoints");
  check(wide.to_string().front() == '1' && wide.to_string().back() == '1',
        "wide string endpoints");
  bool overflow = false, out_of_range_bit = false;
  try { (void)wide.to_ullong(); }
  catch (const overflow_error &) { overflow = true; }
  try { wide.set(65); }
  catch (const out_of_range &) { out_of_range_bit = true; }
  check(overflow && out_of_range_bit, "checked conversion/index bounds");
  bitset<65> copy = wide;
  check(hash<bitset<65>>{}(copy) == hash<bitset<65>>{}(wide),
        "equal bitsets hash equally");

  cerr << "PASS bitset: masks, bitwise/shift/mutations; "
          "widths 0/1/8/63/64/65/127/128/129\n";
}
