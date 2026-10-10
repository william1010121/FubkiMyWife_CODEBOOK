cnt.clear(); // Important: cnt is global and accumulates between calls.
PollardRho(n);
for (auto [p, e] : cnt) // n = product(p^e)
  use_factor(p, e);
