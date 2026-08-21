#include <bits/stdc++.h>
using namespace std;

#include "../../codebook/3_Data_Structure/KDTree_useful.cpp"

using Tree = kd_tree<long long, 2>;
using P = Tree::point;
using ll = long long;

static P point_at(ll x, ll y) {
  P p{{x, y}};
  return p;
}

static bool same(P a, P b) {
  return a.d[0] == b.d[0] && a.d[1] == b.d[1];
}

static ll distance_to(P a, P b) {
  return llabs(a.d[0] - b.d[0]) + llabs(a.d[1] - b.d[1]);
}

static ll expected_kth(const vector<P> &live, P q, int k) {
  vector<ll> distances;
  for (P p : live) distances.push_back(distance_to(p, q));
  sort(distances.begin(), distances.end());
  return distances[k - 1];
}

static vector<pair<ll, ll>> sorted_points(const vector<P> &v) {
  vector<pair<ll, ll>> ret;
  for (P p : v) ret.emplace_back(p.d[0], p.d[1]);
  sort(ret.begin(), ret.end());
  return ret;
}

static void check_ranges(
    Tree &tree, const vector<P> &live,
    const vector<pair<P, P>> &boxes, const string &name) {
  for (size_t i = 0; i < boxes.size(); ++i) {
    P lo = boxes[i].first, hi = boxes[i].second;
    const vector<P> &inside = tree.range(lo, hi);
    // range() returns a reference to internal storage, so copy it before the
    // next query can clear that storage.
    vector<P> got(inside.begin(), inside.end());
    vector<P> want;
    for (P p : live) {
      if (lo.d[0] <= p.d[0] && p.d[0] <= hi.d[0] &&
          lo.d[1] <= p.d[1] && p.d[1] <= hi.d[1])
        want.push_back(p);
    }
    if (sorted_points(got) != sorted_points(want)) {
      cerr << "KDTree_useful range mismatch case=" << name
           << " box=" << i << " got=" << got.size()
           << " want=" << want.size() << '\n';
      exit(1);
    }
  }
}

static void check_tree(
    Tree &tree, const vector<P> &live,
    const vector<P> &queries, const string &name) {
  if (tree.size() != (int)live.size()) {
    cerr << "KDTree_useful size mismatch case=" << name
         << " got=" << tree.size() << " want=" << live.size() << '\n';
    exit(1);
  }
  if (live.empty()) return;
  for (size_t qi = 0; qi < queries.size(); ++qi) {
    for (int k : {1, (int)(live.size() + 1) / 2, (int)live.size()}) {
      ll got = tree.nearest(queries[qi], k);
      ll want = expected_kth(live, queries[qi], k);
      if (got != want) {
        cerr << "KDTree_useful nearest mismatch case=" << name
             << " query=" << qi << " k=" << k
             << " got=" << got << " want=" << want
             << " size=" << live.size() << '\n';
        exit(1);
      }
    }
  }
  vector<pair<P, P>> boxes = {
      {point_at(-1000000, -1000000), point_at(1000000, 1000000)},
      {point_at(0, 0), point_at(0, 0)},
      {point_at(-17, -23), point_at(-17, -23)},
      {point_at(-1000000, -1), point_at(-1, 1000000)},
      {point_at(1, 1), point_at(1000000, 1000000)},
      {point_at(-2, -2), point_at(2, 2)},
  };
  check_ranges(tree, live, boxes, name);
}

static void erase_one(Tree &tree, vector<P> &live, P p,
                      const string &name) {
  bool got = tree.erase(p);
  auto it = find_if(live.begin(), live.end(),
                    [&](P x) { return same(x, p); });
  bool want = it != live.end();
  if (got != want) {
    cerr << "KDTree_useful erase mismatch case=" << name
         << " got=" << got << " want=" << want << '\n';
    exit(1);
  }
  if (it != live.end()) live.erase(it);
}

int main() {
  const ll BIG = 4000000000000000000LL;
  vector<P> queries = {
      point_at(-1000000, -1000000), point_at(-17, 23),
      point_at(-1, 1), point_at(0, 0), point_at(1, -1),
      point_at(1000000, 1000000)};

  // Constructor insertion, duplicate nodes, and a query exactly on a point.
  Tree fresh(BIG);
  vector<P> live;
  fresh.insert(point_at(0, 0));
  live.push_back(point_at(0, 0));
  check_tree(fresh, live, queries, "constructor-first-insert");
  for (int i = 0; i < 5; ++i) {
    fresh.insert(point_at(0, 0));
    live.push_back(point_at(0, 0));
  }
  check_tree(fresh, live, queries, "duplicate-inserts");
  erase_one(fresh, live, point_at(0, 0), "duplicate-erase-1");
  check_tree(fresh, live, queries, "duplicate-erase-1-check");
  erase_one(fresh, live, point_at(123, 456), "missing-erase");

  // Sorted insertions repeatedly trigger scapegoat rebuilds.  The live
  // vector is the only oracle and retains duplicate points as a multiset.
  Tree tree(BIG);
  live.clear();
  for (int i = 0; i < 180; ++i) {
    ll x = i - 90;
    ll y = (i % 7) - 3;
    P p = point_at(x, y);
    tree.insert(p);
    live.push_back(p);
    if (i % 9 == 0) {
      P dup = point_at(x, y);
      tree.insert(dup);
      live.push_back(dup);
    }
    check_tree(tree, live, queries, "sorted-insert-" + to_string(i));
  }
  mt19937 rng(0x3d6e8a11);
  for (int step = 0; step < 360; ++step) {
    if (!live.empty() && (rng() % 3 != 0)) {
      P p = live[rng() % live.size()];
      erase_one(tree, live, p, "adversarial-erase-" + to_string(step));
    } else {
      ll x = (ll)(rng() % 41) - 20;
      ll y = (ll)(rng() % 41) - 20;
      if (step % 11 == 0) x = y = 7;
      P p = point_at(x, y);
      tree.insert(p);
      live.push_back(p);
    }
    check_tree(tree, live, queries,
               "adversarial-step-" + to_string(step));
  }

  // Erase everything, including repeated coordinates, and verify that the
  // structure reports missing deletions without changing its size.
  while (!live.empty()) {
    erase_one(tree, live, live.back(), "drain");
    if (live.size() % 13 == 0) check_tree(tree, live, queries, "drain-check");
  }
  if (tree.size() != 0) {
    cerr << "KDTree_useful did not drain to zero\n";
    return 1;
  }
  tree.clear();
  cout << "kdtree_useful_edges: PASS\n";
}
