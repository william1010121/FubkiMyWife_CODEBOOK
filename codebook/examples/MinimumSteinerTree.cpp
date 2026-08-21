static SteinerTree st; st.init(n); st.vcst[i] = nodeCost;
st.add_edge(u, v, w); // DIRECTED
int c = st.solve(terminals);
