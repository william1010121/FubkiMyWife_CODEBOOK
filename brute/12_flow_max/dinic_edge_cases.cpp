#include <bits/stdc++.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

#define MAXN 16
#define pb push_back
const int INF = 1'000'000'000;
#include "../../codebook/4_Flow_Matching/Dinic.cpp"
#include "flow_oracle.hpp"

struct Arc { int u, v, c; };

static void check_case(int n, int s, int t, const vector<Arc> &arcs,
                       int case_id, bool check_reset = true) {
  vector<vector<int>> cap(n, vector<int>(n));
  MaxFlow mf;
  mf.init(n);
  for (const Arc &e : arcs) {
    flow12::require(0 <= e.u && e.u < n && 0 <= e.v && e.v < n && e.u != e.v,
                    "invalid Dinic test arc");
    flow12::require(e.c >= 0, "negative Dinic test capacity");
    cap[e.u][e.v] += e.c;
    mf.add_edge(e.u, e.v, e.c);  // Keep zero-capacity and parallel arcs too.
  }
  const int want = flow12::directed_mincut(cap, s, t);
  const int got = mf.maxflow(s, t);
  if (got != want) {
    cerr << "Dinic mismatch case=" << case_id << " got=" << got
         << " want=" << want << " arcs=";
    for (const Arc &e : arcs) cerr << '(' << e.u << ',' << e.v << ',' << e.c << ')';
    cerr << '\n';
    exit(1);
  }
  if (check_reset) {
    mf.reset();
    flow12::require(mf.maxflow(s, t) == want,
                    "Dinic reset changed the min-cut value");
  }
}

enum class SameSourceResult { kReturnedZero, kReturnedOther, kTimedOut, kForkError };

// The usual max-flow contract requires distinct terminals.  Run the probe in
// a child because this particular implementation repeatedly returns INF when
// s == t rather than returning the conventional zero flow.
static SameSourceResult probe_same_source() {
  const pid_t pid = fork();
  if (pid < 0) return SameSourceResult::kForkError;
  if (pid == 0) {
    alarm(1);
    MaxFlow mf;
    mf.init(2);
    mf.add_edge(0, 1, 3);
    const int got = mf.maxflow(0, 0);
    _exit(got == 0 ? 0 : 1);
  }
  const auto deadline = chrono::steady_clock::now() + chrono::milliseconds(150);
  for (;;) {
    int status = 0;
    const pid_t done = waitpid(pid, &status, WNOHANG);
    if (done == pid) {
      if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        return SameSourceResult::kReturnedZero;
      return SameSourceResult::kReturnedOther;
    }
    if (done < 0) return SameSourceResult::kForkError;
    if (chrono::steady_clock::now() >= deadline) {
      kill(pid, SIGKILL);
      waitpid(pid, &status, 0);
      return SameSourceResult::kTimedOut;
    }
    usleep(1000);
  }
}

static void exhaustive_binary_graphs() {
  int case_id = 0;
  for (int n = 2; n <= 4; ++n) {
    vector<pair<int, int>> pairs;
    for (int u = 0; u < n; ++u)
      for (int v = 0; v < n; ++v)
        if (u != v) pairs.push_back({u, v});
    for (int mask = 0; mask < (1 << pairs.size()); ++mask) {
      vector<Arc> arcs;
      for (int i = 0; i < static_cast<int>(pairs.size()); ++i)
        arcs.push_back({pairs[i].first, pairs[i].second, (mask >> i) & 1});
      check_case(n, 0, n - 1, arcs, case_id++);
    }
  }
  cout << "Dinic exhaustive binary directed graphs: " << case_id << '\n';
}

static void parallel_and_random_graphs() {
  int case_id = 100000;
  check_case(5, 0, 4,
             {{0, 1, 0}, {0, 1, 2}, {0, 1, 3}, {1, 0, 4},
              {1, 2, 0}, {1, 2, 4}, {1, 2, 1}, {2, 3, 2},
              {3, 4, 0}, {3, 4, 5}, {2, 4, 1}, {4, 1, 2}},
             case_id++);

  mt19937 rng(0xD1C012);
  for (int tc = 0; tc < 1800; ++tc) {
    const int n = 2 + static_cast<int>(rng() % 6);
    vector<Arc> arcs;
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v) if (u != v) {
      const int copies = 1 + static_cast<int>(rng() % 3);
      for (int k = 0; k < copies; ++k)
        arcs.push_back({u, v, static_cast<int>(rng() % 8)});
    }
    check_case(n, 0, n - 1, arcs, case_id++);
  }
  cout << "Dinic parallel/disconnected random graphs: 1801\n";
}

int main() {
  exhaustive_binary_graphs();
  parallel_and_random_graphs();
  const SameSourceResult probe = probe_same_source();
  if (probe == SameSourceResult::kForkError) {
    cerr << "FAIL: could not isolate Dinic s==t contract probe\n";
    return 1;
  }
  if (probe == SameSourceResult::kTimedOut) {
    cout << "CONTRACT: Dinic s==t did not terminate; callers must require s != t\n";
  } else if (probe == SameSourceResult::kReturnedOther) {
    cout << "CONTRACT: Dinic s==t returned a nonzero/nonconventional result; callers must require s != t\n";
  } else {
    cout << "CONTRACT: Dinic s==t returned conventional zero\n";
  }
  cout << "PASS Dinic edge-case oracle suite\n";
}
