sort(p, p+n, [](auto a, auto b) { return a.x < b.x; });
double d = closest_pair(0, n-1);
