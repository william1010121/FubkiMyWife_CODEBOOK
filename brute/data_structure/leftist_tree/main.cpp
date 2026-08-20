#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#include "../../../codebook/3_Data_Structure/Leftist_Tree.cpp"

int main() {
  mt19937 rng(4);
  for (int tc = 0; tc < 1000; ++tc) {
    node *root = nullptr;
    vector<ll> values;
    for (int i = 0; i < 100; ++i) {
      ll x = (int)(rng() % 101) - 50;
      root = merge(root, new node(x));
      values.push_back(x);
      assert(root->data == *max_element(values.begin(), values.end()));
      assert(root->sz == (ll)values.size());
      assert(root->sum == accumulate(values.begin(), values.end(), 0LL));
    }
    while (root) {
      ll mx = *max_element(values.begin(), values.end());
      assert(root->data == mx);
      auto it = find(values.begin(), values.end(), mx);
      values.erase(it);
      pop(root);
      if (root) {
        assert(root->sz == (ll)values.size());
        assert(root->sum == accumulate(values.begin(), values.end(), 0LL));
      }
    }
  }
}
