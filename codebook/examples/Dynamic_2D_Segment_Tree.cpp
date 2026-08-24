Seg2D st(n, m); // coordinates are half-open
st.update(xl, xr, yl, yr, v); // rectangle add
int sum = st.query(xl, xr, yl, yr); // rectangle sum
