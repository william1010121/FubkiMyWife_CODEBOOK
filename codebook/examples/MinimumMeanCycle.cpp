// fill road[i][j], INF if absent
static MinimumMeanCycle mmc; mmc.init(n);
auto [p, q] = mmc.solve(); // p/q reduced
// (-1,-1) if no cycle
