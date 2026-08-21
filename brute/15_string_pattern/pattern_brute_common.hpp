#pragma once

#include <bits/stdc++.h>

namespace string_pattern_brute {

using std::cerr;
using std::exit;
using std::function;
using std::mt19937;
using std::ostringstream;
using std::string;
using std::vector;

inline vector<string> words(const string &alphabet, int max_len) {
  vector<string> out;
  string cur;
  function<void()> dfs = [&] {
    if (!cur.empty()) out.push_back(cur);
    if ((int)cur.size() == max_len) return;
    for (char c : alphabet) {
      cur.push_back(c);
      dfs();
      cur.pop_back();
    }
  };
  out.push_back("");
  dfs();
  return out;
}

inline string random_word(mt19937 &rng, const string &alphabet, int n) {
  string out(n, alphabet[0]);
  for (char &c : out) c = alphabet[rng() % alphabet.size()];
  return out;
}

inline string shown(const string &s) { return s.empty() ? "<empty>" : s; }

template <class T>
inline string vector_text(const vector<T> &v) {
  ostringstream out;
  out << '[';
  for (int i = 0; i < (int)v.size(); ++i) {
    if (i) out << ',';
    out << v[i];
  }
  out << ']';
  return out.str();
}

template <class T>
inline void expect_equal(const string &name, const string &input,
                         const vector<T> &got, const vector<T> &want) {
  if (got == want) return;
  cerr << "FAIL " << name << " input=" << input << "\n"
       << "  got = " << vector_text(got) << "\n"
       << "  want= " << vector_text(want) << '\n';
  exit(1);
}

inline void expect(bool ok, const string &name, const string &input) {
  if (ok) return;
  cerr << "FAIL " << name << " input=" << input << '\n';
  exit(1);
}

}  // namespace string_pattern_brute
