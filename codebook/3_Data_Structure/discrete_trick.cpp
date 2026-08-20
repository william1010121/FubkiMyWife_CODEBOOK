#include <algorithm>
#include <vector>
using namespace std;

// The caller supplies an already sorted list. No sort/unique is performed.
int index_of(const vector<int> &val, int x) { return lower_bound(val.begin(), val.end(), x) - val.begin(); }
int count_le(const vector<int> &val, int x) { return upper_bound(val.begin(), val.end(), x) - val.begin(); }
int count_lt(const vector<int> &val, int x) { return lower_bound(val.begin(), val.end(), x) - val.begin(); }
