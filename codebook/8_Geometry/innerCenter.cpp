Point TriangleInnerCenter(Point a, Point b, Point c) {
    double la = len(b - c);
    double lb = len(a - c);
    double lc = len(a - b);
    return {(la * a.x + lb * b.x + lc * c.x) / (la + lb + lc),
        (la * a.y + lb * b.y + lc * c.y) / (la + lb + lc)};
}
