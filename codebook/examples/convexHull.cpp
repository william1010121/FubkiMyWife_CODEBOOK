vector<P> hull = convex(points); // CCW; drops collinear points
CH ch(hull); bool inside = ch.contain(q); // requires hull.size() >= 3
