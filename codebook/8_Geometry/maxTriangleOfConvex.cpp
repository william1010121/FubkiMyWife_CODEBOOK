// Convex hull indices in cyclic CW/CCW order; O(chnum^2).
double ConvexHullMaxTriangleArea(Point p[], int res[], int chnum) {
  double area = 0;
  auto twice_area = [&](int i, int j, int k) {
    return fabs(Cross(p[res[j]] - p[res[i]],
      p[res[k]] - p[res[i]]));
  };
  for (int i = 0; i + 2 < chnum; ++i) {
    int k = i + 2;
    for (int j = i + 1; j + 1 < chnum; ++j) {
      if (p[res[i]].x == p[res[j]].x &&
          p[res[i]].y == p[res[j]].y) continue;
      k = max(k, j + 1);
      double cur = twice_area(i, j, k), next;
      while (k + 1 < chnum &&
          (next = twice_area(i, j, k + 1)) >= cur) ++k, cur = next;
      area = max(area, cur);
    }
  } return area / 2;
}
