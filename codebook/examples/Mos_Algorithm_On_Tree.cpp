int B = max(1, (int)sqrt(2*n));
qs.emplace_back(u, v, B, in, out, get_lca, id);
solve(qs, ord, [&](int v){ toggle(v); },
  [&](const Query &q){ ans[q.id] = current_answer(); });
