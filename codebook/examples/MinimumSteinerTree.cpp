static SteinerTree st; st.init(n); st.vcst[i] = nodeCost;
st.add_edge(u, v, w); // directed; 0<=edge/vertex costs<INF
int c = st.solve(terminals); // once/init; no shortest_path
// c==INF: disconnected or cost>=INF; empty terminals: 0.
