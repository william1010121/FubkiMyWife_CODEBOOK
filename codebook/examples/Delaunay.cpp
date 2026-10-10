vector<delaunay::Point> p(n);  // distinct integer points
auto tri = delaunay::Triangulate(p);  // vector<array<Point, 3>>, CCW
// Delaunay edges contain the Euclidean MST; Voronoi vertices are
// the circumcentres of these triangles.
