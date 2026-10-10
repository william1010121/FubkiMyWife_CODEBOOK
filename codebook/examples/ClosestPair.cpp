sort(p, p+n, [](auto a, auto b) { return a.x < b.x; });
// Inclusive range; O(n log n), preserves p; n < 2 returns 1e9.
double d = closest_pair(0, n-1);
