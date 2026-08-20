#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
#define SZ(x) int((x).size())
#define pb push_back
#define X first
#define Y second

inline void require_ok(bool ok, const string &where) {
  if (!ok) {
    cerr << "FAIL: " << where << '\n';
    exit(1);
  }
}
