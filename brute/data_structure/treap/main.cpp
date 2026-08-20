#include <bits/stdc++.h>
using namespace std;
#include "../../../codebook/3_Data_Structure/Treap.cpp"

int main() {
  srand(9);
  mt19937 rng(9);
  for (int tc = 0; tc < 500; ++tc) {
    node *root = nullptr;
    multiset<int> ref;
    vector<string> ops;
    for (int q = 0; q < 200; ++q) {
      int x = (int)(rng() % 31) - 15;
      if (rng() % 3) insert(root, x), ref.insert(x), ops.push_back("ins " + to_string(x));
      else {
        bool a = erase(root, x), b = false;
        auto it = ref.find(x);
        if (it != ref.end()) b = true, ref.erase(it);
        ops.push_back("del " + to_string(x));
        assert(a == b);
      }
      if (sz(root) != (int)ref.size()) {
        cerr << "treap size mismatch tc=" << tc << " q=" << q
             << " x=" << x << " got=" << sz(root) << " want=" << ref.size() << '\n';
        for (const string &op : ops) cerr << op << ' '; cerr << '\n';
        return 1;
      }
      int k = 1;
      for (int v : ref) assert(kth(root, k++)->data == v);
      for (int z = -17; z <= 17; ++z)
        assert(Rank(root, z) == distance(ref.begin(), ref.lower_bound(z)));
    }
  }
  return 0;
}
