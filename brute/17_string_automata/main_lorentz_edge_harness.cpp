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
  for (auto &v : rep) v.clear();
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
