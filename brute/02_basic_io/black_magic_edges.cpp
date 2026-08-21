#include <bits/stdc++.h>
#include <bits/extc++.h>
#include <ext/rope>

#define BLACK_MAGIC_NO_MAIN
#include "../../codebook/1_Basic/black_magic.cpp"

using namespace std;

[[noreturn]] void fail(const string &message) {
  cerr << "FAIL black_magic: " << message << '\n';
  exit(1);
}

void check(bool condition, const string &message) {
  if (!condition) fail(message);
}

void check_tree(const tree_set &tree, const set<int> &oracle) {
  check(tree.size() == oracle.size(), "tree size");
  for (int probe : {-100000, -1, 0, 1, 7, 42, 100000})
    check(tree.order_of_key(probe) ==
              static_cast<size_t>(distance(oracle.begin(), oracle.lower_bound(probe))),
          "tree order_of_key");
  int index = 0;
  for (int value : oracle) {
    check(*tree.find_by_order(index) == value, "tree find_by_order");
    ++index;
  }
}

string rope_string(const __gnu_cxx::rope<char> &rope) {
  return string(rope.begin(), rope.end());
}

int main() {
  tree_set tree;
  set<int> tree_oracle;
  const vector<int> values = {INT_MIN, -100000, -1, 0, 1, 7, 42, INT_MAX,
                              7, 0, INT_MIN};
  for (int value : values) {
    tree.insert(value);
    tree_oracle.insert(value);
    check_tree(tree, tree_oracle);
  }

  mt19937 rng(0x6b1acULL);
  for (int tc = 0; tc < 1200; ++tc) {
    int value = static_cast<int>(rng() % 401) - 200;
    if (rng() & 1) {
      tree.insert(value);
      tree_oracle.insert(value);
    } else {
      tree.erase(value);
      tree_oracle.erase(value);
    }
    check_tree(tree, tree_oracle);
  }

  umap table;
  unordered_map<int, int> table_oracle;
  for (int tc = 0; tc < 1400; ++tc) {
    int key = static_cast<int>(rng() % 101) - 50;
    int value = static_cast<int>(rng());
    switch (rng() % 3) {
      case 0:
        table[key] = value;
        table_oracle[key] = value;
        break;
      case 1: {
        size_t erased = table.erase(key);
        size_t expected = table_oracle.erase(key);
        check(erased == expected, "hash erase result");
        break;
      }
      default: {
        auto got = table.find(key);
        auto want = table_oracle.find(key);
        check((got == table.end()) == (want == table_oracle.end()),
              "hash find presence");
        if (want != table_oracle.end()) check(got->second == want->second,
                                               "hash find value");
      }
    }
    check(table.size() == table_oracle.size(), "hash size");
  }

  __gnu_pbds::priority_queue<int> left, right;
  std::priority_queue<int> heap_oracle;
  for (int value : {INT_MIN, -4, 0, 0, 19, INT_MAX}) {
    left.push(value);
    heap_oracle.push(value);
  }
  for (int value : {-9, 3, INT_MAX, 11}) right.push(value), heap_oracle.push(value);
  left.join(right);
  check(right.empty(), "melded heap source emptied");
  while (!heap_oracle.empty()) {
    check(!left.empty() && left.top() == heap_oracle.top(), "melded heap order");
    left.pop();
    heap_oracle.pop();
  }
  check(left.empty(), "melded heap drained");

  __gnu_cxx::rope<char> rope;
  string rope_oracle;
  auto check_rope = [&] {
    check(rope_string(rope) == rope_oracle, "rope content");
    if (!rope_oracle.empty()) {
      size_t pos = rope_oracle.size() / 3;
      size_t len = rope_oracle.size() - pos;
      __gnu_cxx::rope<char> part = rope.substr(pos, len);
      check(rope_string(part) == rope_oracle.substr(pos, len), "rope substr");
    }
  };

  rope.insert(0, "middle");
  rope_oracle.insert(0, "middle");
  check_rope();
  rope.insert(0, "start-");
  rope_oracle.insert(0, "start-");
  check_rope();
  rope.insert(rope.size(), "-end");
  rope_oracle.insert(rope_oracle.size(), "-end");
  check_rope();
  __gnu_cxx::rope<char> snapshot = rope;
  rope.erase(0, 6);
  rope_oracle.erase(0, 6);
  check_rope();
  check(rope_string(snapshot) == "start-middle-end", "rope copy persistence");

  for (int tc = 0; tc < 300; ++tc) {
    if (rope_oracle.empty() || (rng() & 1)) {
      size_t pos = rng() % (rope_oracle.size() + 1);
      string add(1 + rng() % 7, 'a');
      for (char &ch : add) ch = static_cast<char>('a' + rng() % 5);
      rope.insert(pos, add.c_str());
      rope_oracle.insert(pos, add);
    } else {
      size_t pos = rng() % rope_oracle.size();
      size_t len = 1 + rng() % (rope_oracle.size() - pos);
      rope.erase(pos, len);
      rope_oracle.erase(pos, len);
    }
    check_rope();
  }

  cerr << "PASS black_magic: PBDS set/hash/heap and persistent/random rope\n";
}
