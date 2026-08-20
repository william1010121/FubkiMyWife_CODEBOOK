prefft(); // MUST call once first
vector<int> c = multiply(a, b, true);
// true = trim trailing zeros
// double: safe when sum(a*b) < ~1e14
