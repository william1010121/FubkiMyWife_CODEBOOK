vector<delaunay::Point> p(n);  // distinct integer points
auto tri = delaunay::Triangulate(p); // CCW triangles
// Delaunay contains the Euclidean MST; Voronoi vertices
// are the circumcentres of these triangles.
