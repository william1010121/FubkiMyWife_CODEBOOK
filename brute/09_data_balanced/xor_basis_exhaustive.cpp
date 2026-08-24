#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;

// The PDF template is intentionally a fragment: it expects ull and a
// zero-initialized global basis supplied by the including program.
#include "../../codebook/3_Data_Structure/XOR_Basis.cpp"

[[noreturn]] static void fail(const string &message) {
  cerr << "xor_basis_exhaustive: " << message << '\n';
  exit(1);
}

#define CHECK(condition)                                                       \
  do {                                                                         \
    if (!(condition))                                                          \
      fail(string(__FILE__) + ":" + to_string(__LINE__) + ": " #condition); \
  } while (false)

using BasisState = array<ull, 64>;

static void clear_basis() { fill(begin(basis), end(basis), 0ULL); }

static BasisState state() {
  BasisState result{};
  copy(begin(basis), end(basis), result.begin());
  return result;
}

static bool represented(ull x) {
  for (int i = 63; i >= 0; --i)
    if ((x >> i & 1ULL) != 0) x ^= basis[i];
  return x == 0;
}

// A deliberately simple oracle for small-dimensional spans.  It stores every
// reachable XOR value, so it does not duplicate the implementation's Gaussian
// elimination logic.
struct SpanOracle {
  vector<ull> values{0};
  unordered_set<ull> present{0};

  bool contains(ull x) const {
    return present.find(x) != present.end();
  }

  void insert(ull x) {
    if (contains(x)) return;
    const size_t old_size = values.size();
    values.reserve(old_size * 2);
    present.reserve(old_size * 4);
    for (size_t i = 0; i < old_size; ++i) {
      ull y = values[i] ^ x;
      values.push_back(y);
      present.insert(y);
    }
  }
};

static void check_invariants(const SpanOracle &oracle) {
  int rank = 0;
  for (int i = 0; i < 64; ++i) {
    if (basis[i] == 0) continue;
    ++rank;
    CHECK((basis[i] >> i & 1ULL) != 0);
    if (i < 63) CHECK((basis[i] >> (i + 1)) == 0);
    CHECK(oracle.contains(basis[i]));
  }
  CHECK((size_t(1) << rank) == oracle.values.size());

  // Every oracle value must be representable by the template, and no value
  // outside that oracle span may be accepted.
  for (ull x : oracle.values) CHECK(represented(x));
}

static void run_sequence(const vector<ull> &sequence, bool audit_each_step = true) {
  clear_basis();
  SpanOracle oracle;
  check_invariants(oracle);
  for (ull x : sequence) {
    const BasisState before = state();
    const bool dependent = oracle.contains(x);
    const size_t old_size = oracle.values.size();
    ins(x);
    oracle.insert(x);
    if (dependent) {
      // The documented contract says a dependent value is discarded, which
      // means it must not alter any pivot vector.
      CHECK(state() == before);
      CHECK(oracle.values.size() == old_size);
    } else {
      CHECK(oracle.values.size() == old_size * 2);
    }
    if (audit_each_step) check_invariants(oracle);
  }
  check_invariants(oracle);
}

static void enumerate_sequences(vector<ull> &sequence, int at, ull value_count) {
  if (at == (int)sequence.size()) {
    run_sequence(sequence);
    return;
  }
  for (ull x = 0; x < value_count; ++x) {
    sequence[at] = x;
    enumerate_sequences(sequence, at + 1, value_count);
  }
}

static void deterministic_edges() {
  run_sequence({0, 0, 0, numeric_limits<ull>::max(),
                numeric_limits<ull>::max(), 1, 1});

  // Exercise every bit, especially the sign-bit position when ull is passed
  // through expressions that would be dangerous for a signed integer.
  clear_basis();
  for (int i = 0; i < 64; ++i) {
    ins(1ULL << i);
    CHECK(basis[i] == (1ULL << i));
  }
  for (int i = 0; i < 64; ++i) CHECK(basis[i] == (1ULL << i));

  const BasisState full = state();
  const array<ull, 8> dependent = {
      0ULL, numeric_limits<ull>::max(), 1ULL << 63,
      (1ULL << 63) | 7ULL, 0x0123456789abcdefULL,
      0xfedcba9876543210ULL, 0xaaaaaaaaaaaaaaaaULL,
      0x5555555555555555ULL};
  for (ull x : dependent) {
    ins(x);
    CHECK(state() == full);
  }

  // A descending sequence of pivots catches accidental low-to-high
  // elimination and verifies that lower bits are retained correctly.
  vector<ull> descending;
  for (int i = 63; i >= 0; --i)
    descending.push_back((1ULL << i) | (i == 0 ? 0ULL : (1ULL << (i - 1))));
  clear_basis();
  for (ull x : descending) ins(x);
  for (int i = 0; i < 64; ++i) CHECK(basis[i] != 0);
}

static void exhaustive_small_widths() {
  // All ordered insertion histories for these small domains.  This includes
  // zero, duplicates, dependent combinations, and every possible span.
  vector<ull> sequence(6);
  enumerate_sequences(sequence, 0, 8); // 8^6 = 262,144 histories.
  sequence.resize(5);
  enumerate_sequences(sequence, 0, 16); // 16^5 = 1,048,576 histories.
}

static void randomized_histories() {
  mt19937_64 rng(0x584f525f42415349ULL);
  for (int tc = 0; tc < 1800; ++tc) {
    const int width = 1 + rng() % 18;
    const int shift = rng() % (65 - width);
    const ull mask = (1ULL << width) - 1;
    const int length = rng() % 13;
    vector<ull> sequence;
    sequence.reserve(length);
    for (int i = 0; i < length; ++i) {
      ull x = (rng() & mask) << shift;
      if (i % 7 == 0) x = 0;
      if (i % 11 == 0) x = mask << shift;
      sequence.push_back(x);
    }
    // Random histories are audited after the complete history.  Every step
    // still checks dependent-discard behavior; postponing the span walk keeps
    // this independent oracle practical while retaining thousands of cases.
    run_sequence(sequence, false);
  }
}

int main() {
  deterministic_edges();
  exhaustive_small_widths();
  randomized_histories();
  return 0;
}
