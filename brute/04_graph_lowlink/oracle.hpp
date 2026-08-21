#pragma once

#include <bits/stdc++.h>

namespace lowlink_test {

using Edge = std::pair<int, int>;

inline bool connected(int n, const std::vector<Edge> &edges, int ban_vertex,
                      int ban_edge, int source, int target) {
  if (source == ban_vertex || target == ban_vertex) return false;
  if (source == target) return true;
  std::vector<char> seen(n);
  std::queue<int> q;
  seen[source] = 1;
  q.push(source);
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int i = 0; i < (int)edges.size(); ++i) {
      if (i == ban_edge) continue;
      auto [a, b] = edges[i];
      if (a == ban_vertex || b == ban_vertex) continue;
      int v = -1;
      if (a == u) v = b;
      else if (b == u) v = a;
      if (v != -1 && !seen[v]) {
        seen[v] = 1;
        if (v == target) return true;
        q.push(v);
      }
    }
  }
  return false;
}

inline int components(int n, const std::vector<Edge> &edges, int ban_vertex,
                      int ban_edge) {
  std::vector<char> seen(n);
  int answer = 0;
  for (int source = 0; source < n; ++source) {
    if (source == ban_vertex || seen[source]) continue;
    ++answer;
    std::queue<int> q;
    seen[source] = 1;
    q.push(source);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int i = 0; i < (int)edges.size(); ++i) {
        if (i == ban_edge) continue;
        auto [a, b] = edges[i];
        if (a == ban_vertex || b == ban_vertex) continue;
        int v = -1;
        if (a == u) v = b;
        else if (b == u) v = a;
        if (v != -1 && !seen[v]) {
          seen[v] = 1;
          q.push(v);
        }
      }
    }
  }
  return answer;
}

inline bool has_nonloop_edge(const std::vector<Edge> &edges, int u) {
  for (auto [a, b] : edges)
    if (a != b && (a == u || b == u)) return true;
  return false;
}

inline bool same_block_pair(int n, const std::vector<Edge> &edges, int u,
                            int v) {
  if (u == v || !connected(n, edges, -1, -1, u, v)) return u == v;
  for (int w = 0; w < n; ++w) {
    if (w == u || w == v) continue;
    if (!connected(n, edges, w, -1, u, v)) return false;
  }
  return true;
}

inline std::vector<std::vector<int>> canonical_blocks(
    std::vector<std::vector<int>> blocks) {
  for (auto &block : blocks) std::sort(block.begin(), block.end());
  std::sort(blocks.begin(), blocks.end());
  return blocks;
}

inline std::vector<std::vector<int>> vertex_blocks(
    int n, const std::vector<Edge> &edges) {
  std::vector<std::vector<int>> answer;
  if (n >= 31) std::abort();
  const int limit = 1 << n;
  for (int mask = 1; mask < limit; ++mask) {
    std::vector<int> vertices;
    for (int u = 0; u < n; ++u)
      if (mask >> u & 1) vertices.push_back(u);

    bool clique = true;
    for (int i = 0; i < (int)vertices.size() && clique; ++i)
      for (int j = i + 1; j < (int)vertices.size(); ++j)
        if (!same_block_pair(n, edges, vertices[i], vertices[j])) {
          clique = false;
          break;
        }
    if (!clique) continue;

    bool maximal = true;
    for (int x = 0; x < n && maximal; ++x) {
      if (mask >> x & 1) continue;
      bool can_add = true;
      for (int y : vertices)
        if (!same_block_pair(n, edges, x, y)) {
          can_add = false;
          break;
        }
      if (can_add) maximal = false;
    }
    if (!maximal) continue;

    // A vertex with no non-loop incident edge is the singleton block. A
    // self-loop carries no vertex-connectivity information for this API.
    if (vertices.size() > 1 || !has_nonloop_edge(edges, vertices[0]))
      answer.push_back(vertices);
  }
  return canonical_blocks(std::move(answer));
}

inline std::vector<char> articulation_points(
    int n, const std::vector<Edge> &edges) {
  const int base = components(n, edges, -1, -1);
  std::vector<char> answer(n);
  for (int u = 0; u < n; ++u)
    answer[u] = components(n, edges, u, -1) > base;
  return answer;
}

inline std::vector<char> bridge_edges(int n, const std::vector<Edge> &edges) {
  const int base = components(n, edges, -1, -1);
  std::vector<char> answer(edges.size());
  for (int i = 0; i < (int)edges.size(); ++i)
    answer[i] = components(n, edges, -1, i) > base;
  return answer;
}

struct Dsu {
  std::vector<int> p;
  explicit Dsu(int n) : p(n, -1) {}
  int find(int u) {
    return p[u] < 0 ? u : p[u] = find(p[u]);
  }
  void unite(int u, int v) {
    u = find(u), v = find(v);
    if (u == v) return;
    if (p[u] > p[v]) std::swap(u, v);
    p[u] += p[v], p[v] = u;
  }
};

inline std::vector<int> component_signature(const std::vector<int> &id) {
  std::map<int, int> renumber;
  std::vector<int> answer;
  answer.reserve(id.size());
  for (int x : id) {
    if (!renumber.count(x)) renumber[x] = renumber.size();
    answer.push_back(renumber[x]);
  }
  return answer;
}

inline std::vector<int> edge_connected_signature(
    int n, const std::vector<Edge> &edges, const std::vector<char> &is_bridge) {
  Dsu dsu(n);
  for (int i = 0; i < (int)edges.size(); ++i)
    if (!is_bridge[i]) dsu.unite(edges[i].first, edges[i].second);
  std::vector<int> answer(n);
  for (int u = 0; u < n; ++u) answer[u] = dsu.find(u);
  return component_signature(answer);
}

template <class Callback>
inline void enumerate_multigraphs(int n, int max_multiplicity,
                                  Callback callback) {
  std::vector<Edge> slots;
  for (int u = 0; u < n; ++u)
    for (int v = u; v < n; ++v) slots.push_back({u, v});
  std::vector<Edge> edges;
  std::function<void(int)> visit = [&](int at) {
    if (at == (int)slots.size()) {
      callback(edges);
      return;
    }
    for (int copies = 0; copies <= max_multiplicity; ++copies) {
      for (int k = 0; k < copies; ++k) edges.push_back(slots[at]);
      visit(at + 1);
      for (int k = 0; k < copies; ++k) edges.pop_back();
    }
  };
  visit(0);
}

inline std::string describe(int n, const std::vector<Edge> &edges) {
  std::ostringstream out;
  out << "n=" << n << " m=" << edges.size() << " edges=";
  for (auto [u, v] : edges) out << '(' << u << ',' << v << ')';
  return out.str();
}

}  // namespace lowlink_test
