cnt.clear();
PollardRho(n);
for (auto [p, e] : cnt) // n = product(p^e)
  use_factor(p, e);
