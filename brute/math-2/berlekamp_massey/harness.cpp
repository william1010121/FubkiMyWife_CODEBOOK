#include <bits/stdc++.h>
using namespace std;

struct Mint {
  static constexpr int P = 998244353;
  int v;
  Mint(long long x = 0) : v((x % P + P) % P) {}
  Mint& operator+=(Mint b) { if ((v += b.v) >= P) v -= P; return *this; }
  Mint& operator-=(Mint b) { if ((v -= b.v) < 0) v += P; return *this; }
  Mint& operator*=(Mint b) { v = (long long)v * b.v % P; return *this; }
  Mint& operator/=(Mint b) { return *this *= power(b, P - 2); }
  Mint operator-() const { return Mint(-v); }
  friend Mint operator+(Mint a, Mint b) { return a += b; }
  friend Mint operator-(Mint a, Mint b) { return a -= b; }
  friend Mint operator*(Mint a, Mint b) { return a *= b; }
  friend Mint operator/(Mint a, Mint b) { return a /= b; }
  friend bool operator==(Mint a, Mint b) { return a.v == b.v; }
  friend bool operator!=(Mint a, Mint b) { return !(a == b); }
  static Mint power(Mint a, int e) {
    Mint r = 1;
    for (; e; e >>= 1, a *= a) if (e & 1) r *= a;
    return r;
  }
};
#define SZ(a) ((int)(a).size())
#define pb push_back
#include "../../../codebook/6_Math/Berlekamp-Massey.cpp"

vector<Mint> generate(const vector<Mint>& rec, vector<Mint> a, int n) {
  while ((int)a.size() < n) {
    Mint x = 0;
    for (int j = 0; j < (int)rec.size(); ++j)
      x += rec[j] * a[(int)a.size() - j - 1];
    a.push_back(x);
  }
  return a;
}

bool has_recurrence(const vector<Mint>& a, int l) {
  if (l == 0) return all_of(a.begin(), a.end(), [](Mint x) { return x == Mint(0); });
  vector<vector<Mint>> mat;
  for (int i = l; i < (int)a.size(); ++i) {
    vector<Mint> row(l + 1);
    for (int j = 0; j < l; ++j) row[j] = a[i - j - 1];
    row[l] = a[i];
    mat.push_back(row);
  }
  int r = 0;
  for (int c = 0; c < l && r < (int)mat.size(); ++c) {
    int pivot = r;
    while (pivot < (int)mat.size() && mat[pivot][c] == Mint(0)) ++pivot;
    if (pivot == (int)mat.size()) continue;
    swap(mat[pivot], mat[r]);
    Mint inv = Mint(1) / mat[r][c];
    for (int j = c; j <= l; ++j) mat[r][j] *= inv;
    for (int i = 0; i < (int)mat.size(); ++i) if (i != r && mat[i][c] != Mint(0)) {
      Mint q = mat[i][c];
      for (int j = c; j <= l; ++j) mat[i][j] -= q * mat[r][j];
    }
    ++r;
  }
  for (auto &row : mat) {
    bool lhs_zero = true;
    for (int j = 0; j < l; ++j) lhs_zero &= row[j] == Mint(0);
    if (lhs_zero && row[l] != Mint(0)) return false;
  }
  return true;
}

int main() {
  vector<pair<vector<Mint>, vector<Mint>>> tests = {
      {{}, {0, 0, 0}},
      {{2}, {3}},
      {{1, 1}, {0, 1}},
      {{0, 1}, {1, 2}},
      {{3, 0, 4}, {7, 11, 13}},
  };
  mt19937 rng(20260820);
  for (int order = 1; order <= 12; ++order) {
    vector<Mint> rec(order), init(order);
    for (auto &x : rec) x = (int)(rng() % 998244352) + 1;
    for (auto &x : init) x = (int)(rng() % 998244353);
    tests.push_back({rec, init});
  }
  long long checked = 0;
  for (auto &[rec, init] : tests) {
    auto seq = generate(rec, init, 2 * max(1, (int)rec.size()) + 30);
    auto got = BerlekampMassey(seq);
    int want_order = 0;
    while (want_order <= (int)seq.size() && !has_recurrence(seq, want_order)) ++want_order;
    if ((int)got.size() != want_order) {
      cerr << "order mismatch want=" << want_order << " got=" << got.size() << '\n';
      return 1;
    }
    for (int i = got.size(); i < (int)seq.size(); ++i) {
      Mint rhs = 0;
      for (int j = 0; j < (int)got.size(); ++j) rhs += got[j] * seq[i - j - 1];
      if (rhs != seq[i]) {
        cerr << "recurrence mismatch at index " << i << '\n';
        return 1;
      }
    }
    ++checked;
  }
  cout << "Berlekamp-Massey OK: " << checked << " sequence cases\n";
}
