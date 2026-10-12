vector<P> hull = convex(points); // CCW, drops collinear
CH ch(hull); bool inside = ch.contain(q); // SZ(hull)>=3
