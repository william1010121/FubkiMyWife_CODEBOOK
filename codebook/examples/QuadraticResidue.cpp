Jacobi(2, 15); // 1; for composite m, +1 need not mean a square
int r = QuadraticResidue(10, 13); // 6 or 7
if (r != -1) { // prime p only; -1 means no square root
  int other = (13 - r) % 13;
}
