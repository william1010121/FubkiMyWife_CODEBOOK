#include <bits/stdc++.h>
using namespace std;

#include "../../codebook/1_Basic/Default_code.cpp"

// Texas_holdem.cpp is intended to be pasted after the common codebook
// helpers.  Supply the one helper macro that is not in Default_code.cpp.
#define FILL(a, value) fill(begin(a), end(a), (value))
#include "../../codebook/1_Basic/Texas_holdem.cpp"

struct Score {
  int category;
  vector<int> tie;
};

static void init_rank_table() {
  fill(begin(rk), end(rk), -1);
  for (int i = 0; i < 13; ++i)
    rk[static_cast<unsigned char>(ranks[i])] = i;
  for (int i = 0; i < 4; ++i)
    rk[static_cast<unsigned char>(suit[i])] = i;
}

static cards make_hand(const array<int, 5> &hand, bool exercise_string_insert) {
  cards result;
  result.reset();
  for (int i = 0; i < 5; ++i) {
    const int id = hand[i];
    const char s = suit[id / 13];
    const char r = ranks[id % 13];
    if (exercise_string_insert && i == 0)
      result.insert(string{s, r});
    else
      result.insert(s, r);
  }
  result.ready();
  return result;
}

static Score oracle(const array<int, 5> &hand) {
  int rank_count[13]{};
  int suit_count[4]{};
  for (int id : hand) {
    ++rank_count[id % 13];
    ++suit_count[id / 13];
  }

  vector<int> present;
  for (int r = 12; r >= 0; --r)
    if (rank_count[r]) present.push_back(r);

  bool straight = false;
  int straight_high = -1;
  if (present.size() == 5) {
    if (present.front() - present.back() == 4) {
      straight = true;
      straight_high = present.front();
    } else if (present == vector<int>{12, 3, 2, 1, 0}) {
      // Ace is low only in A-2-3-4-5.
      straight = true;
      straight_high = 3;
    }
  }
  const bool flush = *max_element(suit_count, suit_count + 4) == 5;

  vector<pair<int, int>> groups;
  for (int r = 0; r < 13; ++r)
    if (rank_count[r]) groups.emplace_back(rank_count[r], r);
  sort(groups.begin(), groups.end(), greater<pair<int, int>>());

  if (straight && flush) return {1, {straight_high}};
  if (groups[0].first == 4)
    return {2, {groups[0].second, groups[1].second}};
  if (groups[0].first == 3 && groups[1].first == 2)
    return {3, {groups[0].second, groups[1].second}};
  if (flush) return {4, present};
  if (straight) return {5, {straight_high}};
  if (groups[0].first == 3) {
    vector<int> tie{groups[0].second};
    for (const auto &[count, rank] : groups)
      if (count == 1) tie.push_back(rank);
    return {6, tie};
  }
  if (groups[0].first == 2 && groups[1].first == 2)
    return {7, {groups[0].second, groups[1].second, groups[2].second}};
  if (groups[0].first == 2) {
    vector<int> tie{groups[0].second};
    for (const auto &[count, rank] : groups)
      if (count == 1) tie.push_back(rank);
    return {8, tie};
  }
  return {9, present};
}

static bool better(const Score &a, const Score &b) {
  if (a.category != b.category) return a.category < b.category;
  return a.tie > b.tie;
}

static bool equal_score(const Score &a, const Score &b) {
  return a.category == b.category && a.tie == b.tie;
}

static string hand_text(const array<int, 5> &hand) {
  string result;
  for (int id : hand) {
    if (!result.empty()) result += ' ';
    result += suit[id / 13];
    result += ranks[id % 13];
  }
  return result;
}

static int check_one(const array<int, 5> &hand, bool string_insert) {
  const Score want = oracle(hand);
  const cards got = make_hand(hand, string_insert);
  if (got.hands != want.category) {
    cerr << "category mismatch hand=" << hand_text(hand)
         << " got=" << got.hands << " want=" << want.category << '\n';
    return 1;
  }
  return 0;
}

static int check_pair(const array<int, 5> &left, const array<int, 5> &right) {
  const Score left_score = oracle(left);
  const Score right_score = oracle(right);
  const cards left_cards = make_hand(left, false);
  const cards right_cards = make_hand(right, false);
  const bool want_left = better(left_score, right_score);
  const bool want_right = better(right_score, left_score);
  const bool got_left = left_cards > right_cards;
  const bool got_right = right_cards > left_cards;
  if (got_left != want_left || got_right != want_right ||
      (equal_score(left_score, right_score) && (got_left || got_right))) {
    cerr << "comparison mismatch\n  left=" << hand_text(left)
         << "\n right=" << hand_text(right)
         << "\n  got=" << got_left << '/' << got_right
         << " want=" << want_left << '/' << want_right << '\n';
    return 1;
  }
  return 0;
}

int main() {
  init_rank_table();

  // Exhaust all C(52,5) distinct five-card hands.  This independently checks
  // every category boundary, including wheel straights and all suit patterns.
  long long checked = 0;
  for (int a = 0; a < 52; ++a)
    for (int b = a + 1; b < 52; ++b)
      for (int c = b + 1; c < 52; ++c)
        for (int d = c + 1; d < 52; ++d)
          for (int e = d + 1; e < 52; ++e) {
            array<int, 5> hand{a, b, c, d, e};
            if (check_one(hand, false)) return 1;
            ++checked;
          }

  // Exercise the string overload, reset/reuse, and arbitrary insertion order.
  mt19937_64 rng(0x01BA51C5EEDULL);
  vector<int> deck(52);
  iota(deck.begin(), deck.end(), 0);
  vector<array<int, 5>> random_hands;
  for (int iteration = 0; iteration < 20000; ++iteration) {
    shuffle(deck.begin(), deck.end(), rng);
    array<int, 5> hand{deck[0], deck[1], deck[2], deck[3], deck[4]};
    random_hands.push_back(hand);
    if (check_one(hand, true)) return 1;

    cards reused = make_hand(hand, true);
    shuffle(deck.begin(), deck.end(), rng);
    array<int, 5> second{deck[0], deck[1], deck[2], deck[3], deck[4]};
    reused.reset();
    for (int id : second) reused.insert(suit[id / 13], ranks[id % 13]);
    reused.ready();
    if (reused.hands != oracle(second).category) {
      cerr << "reset/reuse mismatch hand=" << hand_text(second) << '\n';
      return 1;
    }
  }

  // Adjacent random pairs plus all self-pairs ensure comparator antisymmetry,
  // equality, and every category/tie-break branch get exercised.
  for (size_t i = 0; i < random_hands.size(); ++i) {
    if (check_pair(random_hands[i], random_hands[i])) return 1;
    if (check_pair(random_hands[i], random_hands[(i * 7919 + 17) % random_hands.size()]))
      return 1;
  }

  cout << "PASS Texas_holdem exhaustive hands=" << checked
       << " randomized=" << random_hands.size() << "\n";
}
