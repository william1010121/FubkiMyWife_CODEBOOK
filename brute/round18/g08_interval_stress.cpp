#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/3_Data_Structure/IntervalContainer.cpp"
int main() {
  const int n = 200000;
  set<pii> intervals;
  for (int i = 0; i < n; ++i) addInterval(intervals, 4*i, 4*i+2);
  for (int i = 0; i < n; ++i)
    removeInterval(intervals, 4*i+2, 4*i+4); // Adjacent gaps.
  assert(intervals.size() == n);
  int i = 0;
  for (auto p : intervals) assert(p == pii(4*i, 4*i+2)), ++i;
  for (i = 0; i < n; ++i) removeInterval(intervals, 4*i, 4*i+1);
  i = 0;
  for (auto p : intervals) assert(p == pii(4*i+1, 4*i+2)), ++i;
  assert(i == n);
  removeInterval(intervals, 2, 4*n-3); // Erase many; preserve endpoints.
  assert(intervals == set<pii>({{1, 2}, {4*n-3, 4*n-2}}));
  addInterval(intervals, 2, 4*n-3);
  assert(intervals == set<pii>({{1, 4*n-2}}));
  removeInterval(intervals, 10, 20); // Split one interval into two.
  assert(intervals == set<pii>({{1, 10}, {20, 4*n-2}}));
  removeInterval(intervals, INT_MIN, INT_MAX);
  assert(intervals.empty());
  cout << "round18 interval stress PASS\n";
}
