#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#define ALL(x) (x).begin(), (x).end()
#include "../../../codebook/3_Data_Structure/link_cut_tree.cpp"

int main() {
  const int n = 12;
  vector<Splay> node(n + 1);
  for (int i = 1; i <= n; ++i) node[i].val = node[i].sum = i;
  for (int i = 2; i <= n; ++i) link(&node[i], &node[i - 1]);
  for (int l = 1; l <= n; ++l) for (int r = l; r <= n; ++r) {
    int want = 0;
    for (int i = l; i <= r; ++i) want ^= i;
    assert(query(&node[l], &node[r]) == want);
  }
  change(&node[5], 99);
  int want = 0;
  for (int i = 3; i <= 8; ++i) want ^= (i == 5 ? 99 : i);
  assert(query(&node[3], &node[8]) == want);
  cut(&node[6], &node[5]);
  assert(!conn(&node[1], &node[12]));
  link(&node[6], &node[5]);
  assert(conn(&node[1], &node[12]));
  return 0;
}
