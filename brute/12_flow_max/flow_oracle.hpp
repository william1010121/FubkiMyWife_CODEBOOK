#pragma once

#include <bits/stdc++.h>

namespace flow12 {

inline int directed_mincut(const std::vector<std::vector<int>> &cap,
                           int s, int t) {
  if (s == t) return 0;
  const int n = static_cast<int>(cap.size());
  int answer = INT_MAX;
  for (int mask = 0; mask < (1 << n); ++mask) {
    if (((mask >> s) & 1) == 0 || ((mask >> t) & 1) != 0) continue;
    int cut = 0;
    for (int u = 0; u < n; ++u) if ((mask >> u) & 1)
      for (int v = 0; v < n; ++v) if (((mask >> v) & 1) == 0)
        cut += cap[u][v];
    answer = std::min(answer, cut);
  }
  return answer;
}

inline int undirected_mincut(const std::vector<std::vector<int>> &weight) {
  const int n = static_cast<int>(weight.size());
  if (n <= 1) return 0;
  int answer = INT_MAX;
  for (int mask = 1; mask + 1 < (1 << n); ++mask) {
    int cut = 0;
    for (int u = 0; u < n; ++u)
      for (int v = u + 1; v < n; ++v)
        if (((mask >> u) & 1) != ((mask >> v) & 1)) cut += weight[u][v];
    answer = std::min(answer, cut);
  }
  return answer;
}

inline int undirected_pair_mincut(const std::vector<std::vector<int>> &weight,
                                  int s, int t) {
  if (s == t) return 0;
  const int n = static_cast<int>(weight.size());
  int answer = INT_MAX;
  for (int mask = 0; mask < (1 << n); ++mask) {
    if (((mask >> s) & 1) == 0 || ((mask >> t) & 1) != 0) continue;
    int cut = 0;
    for (int u = 0; u < n; ++u)
      for (int v = u + 1; v < n; ++v)
        if (((mask >> u) & 1) != ((mask >> v) & 1)) cut += weight[u][v];
    answer = std::min(answer, cut);
  }
  return answer;
}

inline void require(bool condition, const std::string &message) {
  if (!condition) {
    std::cerr << "FAIL: " << message << '\n';
    std::exit(1);
  }
}

inline std::string matrix_string(const std::vector<std::vector<int>> &a) {
  std::ostringstream out;
  for (const auto &row : a) {
    out << '[';
    for (int x : row) out << x << ' ';
    out << "] ";
  }
  return out.str();
}

}  // namespace flow12
