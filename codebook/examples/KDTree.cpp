kdt::init(pts);
long long d2 = kdt::nearest(q);
// squared distance; SKIPS points at distance 0 (i.e. excludes q itself)
