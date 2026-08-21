int B = max(1, (int)pow(n, 2.0/3));
qs.emplace_back(l, r, updates_seen, B, id);
solve(qs,
  [&](int t,int L,int R){ apply_update(t,L,R); },
  [&](int t,int L,int R){ rollback_update(t,L,R); },
  [&](int i){ add_pos(i); }, [&](int i){ remove_pos(i); },
  [&](const Query &q){ ans[q.id] = current_answer(); });
