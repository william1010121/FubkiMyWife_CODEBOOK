Poly f(base, y0); // f(base+i) = poly[i]
f.poly = {y0, y1, y2}; // consecutive values
mint v = f.get_val(x); // any x, O(n)
f.raise(); // f becomes prefix sum, deg+1
