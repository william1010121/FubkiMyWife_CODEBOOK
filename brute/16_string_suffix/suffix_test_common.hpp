#pragma once

#include <bits/stdc++.h>
using namespace std;

namespace suffix_edge {

[[noreturn]] inline void fail(const string &message) {
  cerr << "FAIL: " << message << '\n';
  exit(1);
}

inline void expect(bool ok, const string &message) {
  if (!ok) fail(message);
}

template <class Seq>
vector<int> oracle_sa(const Seq &s) {
  vector<int> sa(s.size());
  iota(sa.begin(), sa.end(), 0);
  sort(sa.begin(), sa.end(), [&](int a, int b) {
    return lexicographical_compare(s.begin() + a, s.end(),
                                   s.begin() + b, s.end());
  });
  return sa;
}

template <class Seq>
vector<int> oracle_lcp(const Seq &s, const vector<int> &sa) {
  vector<int> h(sa.size());
  for (int i = 1; i < (int)sa.size(); ++i)
    while (sa[i] + h[i] < (int)s.size() &&
           sa[i - 1] + h[i] < (int)s.size() &&
           s[sa[i] + h[i]] == s[sa[i - 1] + h[i]]) ++h[i];
  return h;
}

inline vector<int> inverse_sa(const vector<int> &sa) {
  vector<int> rank(sa.size());
  for (int i = 0; i < (int)sa.size(); ++i) rank[sa[i]] = i;
  return rank;
}

template <class Seq>
string describe(const Seq &s) {
  string out = "[";
  for (int i = 0; i < (int)s.size(); ++i) {
    if (i) out += ',';
    out += to_string((int)s[i]);
  }
  return out + ']';
}

template <class Seq>
void expect_vector(const vector<int> &got, const vector<int> &want,
                  const string &what, const Seq &s) {
  if (got != want) {
    string message = what + " on " + describe(s) + " got=" +
                     describe(got) + " want=" + describe(want);
    fail(message);
  }
}

template <class F>
void enumerate_strings(int max_len, int alphabet, F &&visit) {
  vector<int> s;
  function<void()> dfs = [&] {
    visit(s);
    if ((int)s.size() == max_len) return;
    for (int c = 1; c <= alphabet; ++c) {
      s.push_back(c);
      dfs();
      s.pop_back();
    }
  };
  dfs();
}

inline string min_rotation_oracle(const string &s) {
  if (s.empty()) return {};
  string answer = s;
  for (int i = 1; i < (int)s.size(); ++i)
    answer = min(answer, s.substr(i) + s.substr(0, i));
  return answer;
}

inline string as_string(const vector<int> &s) {
  string out;
  out.reserve(s.size());
  for (int x : s) {
    expect(1 <= x && x <= 26, "internal test alphabet out of range");
    out.push_back(char('a' + x - 1));
  }
  return out;
}

inline void add_periodic_cases(vector<vector<int>> &cases) {
  const vector<vector<int>> periods = {{1}, {1, 2}, {1, 1, 2},
                                       {1, 2, 1, 3}, {1, 2, 3, 2, 1}};
  for (const auto &period : periods)
    for (int n = 1; n <= 180; ++n) {
      vector<int> s(n);
      for (int i = 0; i < n; ++i) s[i] = period[i % period.size()];
      cases.push_back(move(s));
    }
  for (int n = 1; n <= 180; ++n) {
    cases.push_back(vector<int>(n, 1));
    vector<int> s(n, 1);
    s.back() = 2;
    cases.push_back(move(s));
    s.assign(n, 2);
    s[0] = 1;
    cases.push_back(move(s));
    s.assign(n, 1);
    if (n >= 2) s[n / 2] = 2;
    cases.push_back(move(s));
  }
}

}  // namespace suffix_edge
