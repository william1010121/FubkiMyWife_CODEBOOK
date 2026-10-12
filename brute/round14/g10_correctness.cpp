#include "../../codebook/1_Basic/Default_code.cpp"
#include <sys/wait.h>
#include <unistd.h>
namespace queens {
#include "../../codebook/9_Else/NQueens.cpp"
}
namespace aho {
constexpr int len = 128, sigma = 2;
#include "../../codebook/5_String/Aho-Corasick_Automatan.cpp"
}
namespace mmc {
constexpr int N = 7;
constexpr ll INF = LLONG_MAX;
#include "../../codebook/2_Graph/MinimumMeanCycle.cpp"
}
namespace sat {
#include "../../codebook/2_Graph/SCC.cpp"
#include "../../codebook/2_Graph/2SAT.cpp"
}
int fpow(int x, int k, int m) {
  ll r = 1 % m;
  for (; k; k >>= 1, x = 1LL * x * x % m)
    if (k & 1) r = r * x % m;
  return r;
}
#include "../../codebook/6_Math/DiscreteLog.cpp"
namespace fourier {
#include "../../codebook/7_Polynomial/FFT_chrislaiisme.cpp"
}
#include "../../codebook/9_Else/BitsetLCS.cpp"
// Observe the actual sweep's two int swaps per event, without changing it.
namespace sweep {
vector<pii> points;
vector<int> observed;
set<pii> pairs;
pll last;
bool second;
void swap(int &a, int &b) {
  if (!second) {
    int u = a, v = b;
    int i = find(ALL(observed), u) - observed.begin();
    int j = find(ALL(observed), v) - observed.begin();
    assert(abs(i - j) == 1);
    assert(pairs.insert(minmax(u, v)).second);
    ll x = 1LL * points[u].X - points[v].X;
    ll y = 1LL * points[u].Y - points[v].Y;
    if (x < 0 || (!x && y < 0)) x = -x, y = -y;
    if (pairs.size() > 1)
      assert((__int128)last.Y * x <= (__int128)y * last.X);
    last = {x, y};
    std::swap(observed[i], observed[j]);
  }
  second = !second;
  std::swap(a, b);
}
#include "../../codebook/8_Geometry/rotatingSweepLine.cpp"
}
#define fread_unlocked fread
#include "../../codebook/1_Basic/readchar.cpp"
#undef fread_unlocked
mt19937 rng(140010);

void check_queens() {
  vector<int> ret{INT_MIN, INT_MAX};
  for (int n = 0; n <= 1000; ++n) {
    auto old = ret; size_t offset = ret.size();
    queens::solve(ret, n);
    assert(equal(ALL(old), ret.begin()));
    if (n == 2 || n == 3) { assert(ret == old); continue; }
    assert(ret.size() == offset + n);
    vector<bool> col(n + 1), up(2 * n + 1), down(2 * n + 1);
    for (int i = 0; i < n; ++i) {
      int c = ret[offset + i];
      assert(1 <= c && c <= n);
      assert(!col[c] && !up[i + c] && !down[i - c + n]);
      col[c] = up[i + c] = down[i - c + n] = true;
    }
  }
}
void check_aho() {
  vector<string> patterns{string()};
  for (int n = 1; n <= 6; ++n) for (int b = 0; b < (1 << n); ++b) {
    string s(n, 'A');
    for (int i = 0; i < n; ++i) s[i] += (b >> i) & 1;
    patterns.pb(s);
  }
  for (int t = 0; t < 30; ++t) {
    shuffle(ALL(patterns), rng); aho::ac.init();
    vector<int> end;
    for (auto &s : patterns) end.pb(aho::ac.input(s));
    for (int i = 0; i < SZ(patterns); ++i)
      assert(aho::ac.input(patterns[i]) == end[i]);
    assert(aho::ac.top == aho::len); // Exact capacity.
    aho::ac.make_fl();
    string text(t ? 511 + t : 0, 'A');
    for (char &c : text) c += rng() & 1;
    if (t % 3 == 1) fill(ALL(text), 'B');
    int state = 1; aho::ac.cnt[1] = 1;
    for (char c : text) ++aho::ac.cnt[state = aho::ac.rnx[state][c-'A']];
    aho::ac.solve();
    for (int i = 0; i < SZ(patterns); ++i) {
      int want = 0;
      for (size_t j = 0; j + patterns[i].size() <= text.size(); ++j)
        want += text.compare(j, patterns[i].size(), patterns[i]) == 0;
      assert(aho::ac.cnt[end[i]] == want);
    }
  }
}
pll cycle_oracle(int n) {
  pll ans{-1, -1}; vector<bool> used(n);
  function<void(int,int,int,__int128)> visit =
    [&](int root, int u, int k, __int128 total) {
      for (int v = 0; v < n; ++v) if (mmc::road[u][v] != mmc::INF) {
        auto sum = total + mmc::road[u][v];
        if (v == root) {
          if (ans.Y < 0 || sum * ans.Y < (__int128)ans.X * (k+1))
            ans = {ll(sum), k+1};
        } else if (!used[v]) {
          used[v] = true; visit(root, v, k+1, sum); used[v] = false;
        }
      }
    };
  for (int i = 0; i < n; ++i) {
    used[i] = true; visit(i, i, 0, 0); used[i] = false;
  }
  if (ans.Y > 0) { ll g = gcd(ans.X, ans.Y); ans.X /= g; ans.Y /= g; }
  return ans;
}
void check_cycles() {
  mmc::MinimumMeanCycle s;
  for (int t = 0; t < 300; ++t) {
    int n = t % 8; s.init(n);
    // Every <=n-edge walk has absolute sum <INF/2; differences fit ll.
    ll h = mmc::INF / (4 * mmc::N);
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
      mmc::road[u][v] = rng()%3 ? ll(rng()%3-1LL)*h + int(rng()%7)-3
                              : mmc::INF;
    pll want = cycle_oracle(n);
    assert(s.solve() == want && s.solve() == want);
    // Transpose preserves all directed cycle weights and lengths.
    for (int u = 0; u < n; ++u) for (int v = 0; v < u; ++v)
      std::swap(mmc::road[u][v], mmc::road[v][u]);
    assert(s.solve() == want);
  }
}
void check_sat() {
  assert(sat::SAT(0).solve());
  for (int t = 0; t < 60; ++t) {
    int n = 512; sat::SAT s(n); vector<pii> clauses;
    vector<int> truth(n), order(n); iota(ALL(order), 0);
    shuffle(ALL(order), rng);
    for (int i = 0; i < n; ++i) truth[i] = rng() & 1;
    auto lit = [&](int i) { return i + (truth[i] ? 0 : n); };
    auto add = [&](int a, int b) {
      s.add_clause(a, b); clauses.pb({a,b});
    };
    add(lit(order[0]), lit(order[0]));
    for (int i = 1; i < n; ++i) {
      int a = lit(order[i-1]), b = lit(order[i]);
      add(s.rv(a), b); add(s.rv(b), a); add(s.rv(a), b);
      add(b, s.rv(b)); // Tautology, duplicates, shuffled variable IDs.
    }
    if (t & 1) add(s.rv(lit(order.back())), s.rv(lit(order.back())));
    for (int pass = 0; pass < 3; ++pass) {
      assert(s.solve() == !(t & 1));
      if (!(t & 1)) {
        for (int i = 0; i < n; ++i)
          assert(s.istrue[i] == truth[i] && s.istrue[i+n] != truth[i]);
        for (auto [a,b] : clauses) assert(s.istrue[a] || s.istrue[b]);
      }
    }
  }
}
void check_logs() {
  for (int m : {1, 2, 72, 1000, 6561, 59049, 655360, 1048576})
    for (int x : {0, 1 % m, 2 % m, 3 % m, 6 % m, m-1}) {
      unordered_map<int,int> first; vector<int> orbit;
      for (int y = 1 % m; !first.count(y); y = 1LL*y*x%m) {
        first[y] = SZ(orbit); orbit.pb(y);
      }
      vector<int> targets{0, m-1, m/2};
      int step = ceil(sqrt(m));
      for (int k : {0,1,30,31,32,99,100,101,step-1,step,step+1})
        targets.pb(fpow(x,k,m));
      for (int t = 0; t < 8; ++t) targets.pb(rng()%m);
      for (int y : targets) {
        auto it = first.find(y);
        assert(DiscreteLog(x,y,m) ==
               (it == first.end() ? -1 : it->second));
      }
    }
}
void check_fft() {
  for (int n : {1,2,4,8,16,64,256}) {
    vector<fourier::cd> a(n);
    for (auto &x : a) x = {double(int(rng()%31)-15),
                           double(int(rng()%31)-15)};
    auto b = a; fourier::fft(b); fourier::fft(b);
    for (int i = 0; i < n; ++i)
      assert(abs(b[i]-double(n)*a[(n-i)%n]) < 1e-7);
    fourier::ifft(b); fourier::ifft(b);
    for (int i = 0; i < n; ++i) assert(abs(a[i]-b[i]) < 1e-9);
  }
  for (int n : {0,1,31,32,33,63,64,65,127,128,129}) {
    vector<int> a(n), b(n ? 5 : 0);
    for (int &x : a) x = int(rng()%65537)-32768;
    for (int &x : b) x = int(rng()%5)-2;
    vector<int> want(n ? n+4 : 0);
    for (int i = 0; i < n; ++i) for (int j = 0; j < SZ(b); ++j)
      want[i+j] += a[i]*b[j];
    assert(fourier::mult(a,b) == want);
    assert(fourier::mult(b,a) == want);
    // Aliased small signed coefficients with exact int-sized results.
    for (int &x : a) x %= 7;
    want.assign(n ? 2*n-1 : 0, 0);
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
      want[i+j] += a[i]*a[j];
    assert(fourier::mult(a,a) == want);
  }
}
void check_lcs() {
  for (int t = 0; t < 600; ++t) {
    int n = rng()%260, m = t%3 ? rng()%260 : 64*(t%4)+1;
    vector<int> a(n), b(m);
    const int values[] = {INT_MIN, INT_MAX, -1, 0, 1};
    for (int &x : a) x = values[rng()%5];
    for (int &x : b) x = values[rng()%5];
    if (t%5 == 0) sort(ALL(a));
    if (t%7 == 0) fill(ALL(b), INT_MIN);
    vector<int> dp(m+1);
    for (int x : a) {
      int prev = 0;
      for (int j = 0; j < m; ++j) {
        int old = dp[j+1];
        dp[j+1] = x == b[j] ? prev+1 : max(dp[j+1],dp[j]);
        prev = old;
      }
    }
    assert(bitset_lcs(a,b) == dp[m] && bitset_lcs(b,a) == dp[m]);
  }
}
void check_sweep() {
  for (int t = 0; t < 400; ++t) {
    vector<pii> ps;
    int n = t%20;
    for (int i = 0; i < n; ++i) ps.pb({i*100000, i*i*100000});
    if (t >= 396) {
      ps = {{INT_MIN,INT_MIN}, {INT_MIN,INT_MAX},
            {INT_MAX,INT_MIN}, {INT_MAX,INT_MAX}};
      n = ps.size();
    }
    // Parabola/square: no three collinear; parallel events.
    if (t&1) for (auto &p : ps) std::swap(p.X, p.Y);
    if ((t&2) && t < 396) for (auto &p : ps) p.X = -p.X;
    if ((t&4) && t < 396) for (auto &p : ps) p.Y = -p.Y;
    shuffle(ALL(ps), rng);
    sweep::points = ps; sweep::pairs.clear(); sweep::second = false;
    sweep::observed.resize(n); iota(ALL(sweep::observed),0);
    sort(ALL(sweep::observed), [&](int a,int b) { return ps[a]<ps[b]; });
    auto want = sweep::observed; reverse(ALL(want));
    auto got = sweep::rotatingSweepLineOrder(ps);
    assert(got == want && got == sweep::observed);
    assert(SZ(sweep::pairs) == n*(n-1)/2 && !sweep::second);
  }
}
void check_reader() {
  // Pipe: tiny producer writes, binary bytes, partial refill.
  vector<unsigned char> bytes(2*65536+1);
  for (size_t i = 0; i < bytes.size(); ++i) bytes[i] = rng()%256;
  for (int i : {0,65535,65536,131071,131072}) bytes[i] = 255;
  int fd[2]; assert(pipe(fd) == 0);
  pid_t child = fork(); assert(child >= 0);
  if (!child) {
    close(fd[0]);
    for (size_t i = 0; i < bytes.size();) {
      size_t count = min(bytes.size()-i, size_t(1+i%257));
      ssize_t done = write(fd[1], bytes.data()+i, count);
      if (done <= 0) _exit(1);
      i += done;
    }
    close(fd[1]); _exit(0);
  }
  close(fd[1]); assert(dup2(fd[0],STDIN_FILENO) >= 0);
  close(fd[0]); clearerr(stdin);
  for (int c : bytes) assert(readchar() == c);
  for (int i = 0; i < 20; ++i) assert(readchar() == EOF);
  int status; assert(waitpid(child,&status,0) == child);
  assert(WIFEXITED(status) && WEXITSTATUS(status) == 0);
}
int main() {
  check_queens(); check_aho(); check_cycles(); check_sat(); check_logs();
  check_fft(); check_lcs(); check_sweep(); check_reader();
  cout << "PASS round14 g10: nine adversarial audits\n";
}
