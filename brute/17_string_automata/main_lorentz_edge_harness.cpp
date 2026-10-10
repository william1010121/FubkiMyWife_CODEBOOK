#include <bits/stdc++.h>
using namespace std;

const int kN = 256;
static vector<int> Zalgo(const string &s) {
  vector<int> z(s.size());
  if (s.empty()) return z;
  z[0] = (int)s.size();
  for (int i = 1, l = 0, r = 0; i < (int)s.size(); ++i) {
    if (i <= r) z[i] = min(r - i + 1, z[i - l]);
    while (i + z[i] < (int)s.size() && s[z[i]] == s[i + z[i]]) ++z[i];
    if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
  }
  return z;
}
#define ALL(v) (v).begin(), (v).end()
#include "../../codebook/5_String/MainLorentz.cpp"

static void fail(const string &message) {
  cerr << "FAIL main-lorentz-edge: " << message << '\n';
  exit(1);
}

static set<pair<int, int>> brute(const string &s, int offset) {
  set<pair<int, int>> result;
  for (int length = 1; 2 * length <= (int)s.size(); ++length)
    for (int p = 0; p + 2 * length <= (int)s.size(); ++p)
      if (s.compare(p, length, s, p + length, length) == 0)
        result.emplace(length, p + offset);
  return result;
}

static void check(const string &s, int offset = 0) {
  main_lorentz(s, offset);
  set<pair<int, int>> got;
  for (int length = 0; length < kN; ++length)
    for (const auto &[left, right] : rep[length]) {
      const int local_left = left - offset;
      const int local_right = right - offset;
      if (length <= 0 || 2 * length > (int)s.size() ||
          local_left < 0 || local_right < local_left ||
          local_right + 2 * length > (int)s.size())
        fail("repeat range out of bounds");
      for (int p = left; p <= right; ++p) {
        const int local = p - offset;
        if (!got.emplace(length, p).second)
          fail("duplicate tandem repeat");
        if (s.compare(local, length, s, local + length, length) != 0)
          fail("false tandem repeat");
      }
    }
  if (got != brute(s, offset)) fail("missing tandem repeat");
}

static void check_consecutive_calls() {
  // The second input has no repeats. It must not inherit the first
  // call's ranges, even when both calls use nonzero coordinate offsets.
  main_lorentz("aaaa", 7);
  main_lorentz("ab", 19);
  for (const auto &ranges : rep)
    if (!ranges.empty())
      fail("stale tandem repeat after consecutive call");
}

static void check_accumulation() {
  string a = "abababaabababa", b({char(0), char(255), char(0), char(255)});
  main_lorentz(a, -100);
  vector<vector<pair<int, int>>> old(begin(rep), end(rep));
  main_lorentz_impl(b, 100);
  set<pair<int, int>> got, want = brute(a, -100);
  auto more = brute(b, 100);
  want.insert(more.begin(), more.end());
  for (int l = 0; l < kN; ++l) {
    if (rep[l].size() < old[l].size() ||
        !equal(old[l].begin(), old[l].end(), rep[l].begin()))
      fail("helper changed earlier report intervals");
    for (auto [left, right] : rep[l])
      for (int p = left; p <= right; ++p)
        if (!got.emplace(l, p).second) fail("helper duplicate repeat");
  }
  if (got != want) fail("helper accumulation mismatch");
  old.assign(begin(rep), end(rep));
  main_lorentz_impl("", INT_MAX);
  for (int l = 0; l < kN; ++l)
    if (rep[l] != old[l]) fail("empty helper changed report");
}

static void enumerate_words(int alphabet, int length,
                            const function<void(const string &)> &visit) {
  long long total = 1;
  for (int i = 0; i < length; ++i) total *= alphabet;
  for (long long code = 0; code < total; ++code) {
    long long x = code;
    string s(length, 'a');
    for (int i = 0; i < length; ++i) {
      s[i] = char('a' + x % alphabet);
      x /= alphabet;
    }
    visit(s);
  }
}

int main() {
  int cases = 0;
  check(""), ++cases;
  check("a"), ++cases;
  check("aa"), ++cases;
  check("ab"), ++cases;
  check("aaaa", 7), ++cases;
  check("abababab", 11), ++cases;
  check("abcabcabc", 13), ++cases;
  check("abacabaabababa", 17), ++cases;
  check("##", 19), ++cases;
  check("#a#a#a", 21), ++cases;
  check(string("\0\0", 2), 23), ++cases;
  check(string("\0#\0#\0", 5), 25), ++cases;
  check_consecutive_calls(), ++cases;
  check_accumulation(), ++cases;

  // Period 255 at the exact capacity boundary, with every byte value
  // appearing in a mutation; alternates large/small scratch lifetimes.
  string alphabet;
  for (int c = 0; c < 255; ++c) alphabet += char(c);
  for (int shift : {0, 1, 127, 254}) {
    rotate(alphabet.begin(), alphabet.begin() + shift, alphabet.end());
    string s = alphabet + alphabet;
    for (int p : {0, 254, 255, 509}) {
      check(s, INT_MIN), ++cases;
      string mutated = s;
      mutated[p] = char(255);
      check(mutated, INT_MAX - 509), ++cases;
      check("aa"), ++cases;
    }
    s += s[0]; check(s, INT_MAX - 510), ++cases;
  }

  // Every shifted input position fits int, including both limit values.
  for (int n : {2, 3, 4, 5, 2 * kN - 2, 2 * kN - 1})
    for (int offset : {INT_MIN, INT_MAX - (n - 1)})
      check(string(n, char(255)), offset), ++cases;
  // Force long cross-half matches with unequal half lengths and bytes.
  const string bytes({char(0), char(128), char(255)});
  for (int n : {127, 128, 129, 509, 510, 511}) {
    string s;
    for (int i = 0; i < n; ++i) s += bytes[i % bytes.size()];
    check(s, -700), ++cases;
  }

  // Break one match near each recursive split, with odd/even half caps.
  for (int period : {1, 2, 3, 7})
    for (int n : {63, 64, 65, 253, 254, 255, 509, 510, 511}) {
      string s(n, '\0');
      for (int i = 0; i < n; ++i) s[i] = bytes[(i % period) % 3];
      for (int p : {0, n / 4, n / 2 - 1, n / 2, n / 2 + 1,
                    3 * n / 4, n - 1}) {
        string t = s;
        t[p] = char(17);
        check(t, p % 2 ? INT_MIN : INT_MAX - (n - 1)), ++cases;
      }
    }

  for (int n = 0; n <= 8; ++n)
    enumerate_words(3, n, [&](const string &s) { check(s), ++cases; });

  mt19937 rng(0x10AEEEDu);
  for (int tc = 0; tc < 650; ++tc) {
    const int n = rng() % 97;
    const int alphabet = 1 + rng() % 4;
    string s(n, 'a');
    for (char &c : s) c = char('a' + rng() % alphabet);
    check(s, rng() % 23), ++cases;
  }
  for (int tc = 0; tc < 300; ++tc) {
    string s(rng() % 97, '\0');
    for (char &c : s) c = static_cast<char>(rng() & 255);
    check(s, rng() % 23), ++cases;
  }
  cout << "PASS main-lorentz-edge cases=" << cases
       << " oracle=all-square-periods-with-offsets\n";
}
