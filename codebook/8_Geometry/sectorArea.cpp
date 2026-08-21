// calc area of sector which include a, b
double SectorArea(P a, P b, double r) {
  double o = abs(remainder(a.angle() - b.angle(), 2 * pi));
  return r * r * o / 2;
}
