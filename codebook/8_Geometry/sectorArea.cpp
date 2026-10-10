// Minor sector between nonzero vectors a, b; r >= 0; O(1).
double SectorArea(P a, P b, double r) {
  return r * r * abs(remainder(a.angle() - b.angle(), 2 * pi)) / 2;
}
