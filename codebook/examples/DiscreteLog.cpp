// Needs ModArithmetic for this verification helper.
int fpow(int x, int k, int m) {
  return (int)ModArithmetic(m).pow(x, k);
}
int k = DiscreteLog(2, 8, 13); // k=3; -1 if none
