#include <bits/stdc++.h>
#include <bits/extc++.h>
#include <ext/rope>
using namespace __gnu_pbds; using namespace __gnu_cxx;
typedef tree<int, null_type, std::less<int>, rb_tree_tag, tree_order_statistics_node_update> tree_set; typedef cc_hash_table<int, int> umap; typedef priority_queue<int> heap;
#ifndef BLACK_MAGIC_NO_MAIN
int main() {
  // random
  mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
  auto get_rand = [&](int l, int r) {
    return uniform_int_distribution<int>(l, r)(rng);
  };
  vector<int> v = {1, 2, 3};
  (void)get_rand(0, 10);
  shuffle(v.begin(), v.end(), rng);
  // rb tree
  tree_set s;
  s.insert(71); s.insert(22);
  assert(*s.find_by_order(0) == 22); assert(*s.find_by_order(1) == 71);
  assert(s.order_of_key(22) == 0); assert(s.order_of_key(71) == 1);
  s.erase(22);
  assert(*s.find_by_order(0) == 71); assert(s.order_of_key(71) == 0);
  // mergable heap
  __gnu_pbds::priority_queue<int> a, b;
  a.push(1), b.push(2), a.join(b);
  // persistant
  rope<char> r[2];
  r[1] = r[0];
  r[1].insert(0, "abc");
  r[1].erase(1, 1);
  std::cout << r[1].substr(0, 2) << std::endl;
}
#endif
