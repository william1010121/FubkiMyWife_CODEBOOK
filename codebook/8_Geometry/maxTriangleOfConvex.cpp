double ConvexHullMaxTriangleArea(Point p[], int res[], int chnum) {
  double area = 0;
  for (int i = 0; i < chnum; ++i) for (int j = i + 1; j < chnum; ++j)
    for (int k = j + 1; k < chnum; ++k)
      area = max(area, fabs(Cross(p[res[j]] - p[res[i]], p[res[k]] - p[res[i]])));
  return area / 2;
}
