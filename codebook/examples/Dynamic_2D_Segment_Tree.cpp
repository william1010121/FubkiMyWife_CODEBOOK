// A: rectangle add + rectangle sum; initially all zero.
Seg2D st(n, m); // 0-based, half-open coordinates
st.update(xl, xr, yl, yr, v); // rectangle add
int sum = st.query(xl, xr, yl, yr); // rectangle sum
