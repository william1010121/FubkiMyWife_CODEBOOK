#include <bits/stdc++.h>
using namespace std;

using pii = pair<int, int>;
#define X first
#define Y second
#include "../../codebook/3_Data_Structure/IntervalContainer.cpp"

[[noreturn]] static void fail(const string &what) {
  cerr << "IntervalContainer mismatch: " << what << '\n';
  exit(1);
}

static void verify(const set<pii> &got, const set<int> &want, int tc) {
  pii previous{};
  bool first = true;
  for (auto it = got.begin(); it != got.end(); ++it) {
    if (it->first >= it->second)
      fail("non-empty interval invariant at tc=" + to_string(tc));
    if (!first && previous.second >= it->first)
      fail("intervals are not strictly separated at tc=" + to_string(tc));
    previous = *it;
    first = false;
  }
  // All generated endpoints are in this small universe, so this is a direct
  // point-set oracle rather than a second interval implementation.
  for (int x = -24; x <= 24; ++x) {
    bool covered = false;
    for (auto [l, r] : got) covered |= l <= x && x < r;
    if (covered != want.count(x))
      fail("coverage at x=" + to_string(x) + " tc=" + to_string(tc));
  }
}

static void apply_add(set<pii> &got, set<int> &want, int l, int r, int tc) {
  addInterval(got, l, r);
  for (int x = l; x < r; ++x) want.insert(x);
  verify(got, want, tc);
}

static void apply_remove(set<pii> &got, set<int> &want, int l, int r, int tc) {
  removeInterval(got, l, r);
  for (int x = l; x < r; ++x) want.erase(x);
  verify(got, want, tc);
}

static void deterministic_edges() {
  set<pii> got;
  set<int> want;
  int tc = 0;
  apply_add(got, want, 0, 0, tc++);   // empty add is a no-op
  apply_remove(got, want, 0, 0, tc++);
  apply_add(got, want, 0, 4, tc++);
  apply_add(got, want, 4, 8, tc++);   // touching intervals must merge
  apply_add(got, want, 2, 6, tc++);   // contained duplicate coverage
  apply_add(got, want, -3, 0, tc++);
  apply_add(got, want, -3, 0, tc++);  // exact duplicate
  apply_remove(got, want, 1, 7, tc++); // split an existing interval
  apply_remove(got, want, -4, -2, tc++); // disjoint removal
  apply_remove(got, want, -2, 2, tc++);
  apply_add(got, want, -24, 24, tc++); // whole tested universe
  apply_remove(got, want, -24, 24, tc++);
}

static void extreme_coordinates() {
  set<pii> got;
  addInterval(got, numeric_limits<int>::min(), numeric_limits<int>::min() + 2);
  addInterval(got, numeric_limits<int>::min() + 2,
              numeric_limits<int>::min() + 4);
  if (got != set<pii>{{numeric_limits<int>::min(), numeric_limits<int>::min() + 4}})
    fail("INT_MIN touching intervals");
  removeInterval(got, numeric_limits<int>::min() + 1,
                 numeric_limits<int>::min() + 3);
  if (got != set<pii>{{numeric_limits<int>::min(), numeric_limits<int>::min() + 1},
                      {numeric_limits<int>::min() + 3,
                       numeric_limits<int>::min() + 4}})
    fail("INT_MIN split removal");
}

int main() {
  deterministic_edges();
  extreme_coordinates();

  mt19937 random_source(0x1A7E);
  for (int tc = 0; tc < 9000; ++tc) {
    set<pii> got;
    set<int> want;
    for (int q = 0; q < 180; ++q) {
      int l = (int)(random_source() % 49) - 24;
      int r = (int)(random_source() % 49) - 24;
      if (l > r) swap(l, r);
      if (random_source() & 1)
        apply_add(got, want, l, r, tc);
      else
        apply_remove(got, want, l, r, tc);
    }
  }
  return 0;
}
