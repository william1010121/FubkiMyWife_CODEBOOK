Point TriangleCircumCenter(Point a, Point b, Point c) {
    Point u = b - a, v = c - a;
    double x = u.x*u.x + u.y*u.y, y = v.x*v.x + v.y*v.y;
    double d = 2 * (u.x*v.y - u.y*v.x);
    return a + Point((v.y*x - u.y*y) / d, (u.x*y - v.x*x) / d);
}
