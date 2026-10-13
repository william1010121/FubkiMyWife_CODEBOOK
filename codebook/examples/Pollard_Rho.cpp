cnt.clear(); PollardRho(n); // cnt accumulates otherwise
for (auto [p, e] : cnt) // n = product(p^e)
  use_factor(p, e);
