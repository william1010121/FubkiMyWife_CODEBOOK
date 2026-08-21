// Requires VoronoiPoint and voronoi_cell_areas from Triangulation_Vonoroi.cpp.
vector<long double> solve_voronoi(const vector<VoronoiPoint> &points) {
  return voronoi_cell_areas(points);
}
